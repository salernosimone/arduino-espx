#pragma once
#include <esp_camera.h>
#include <esp_heap_caps.h>
#include <esp32-hal-psram.h>
#include "../OpStatus.h"
#include "../threadx.h"
#include "./pixformat.h"
#include "./resolution.h"
#include "./model.h"
#include "./quality.h"
#include "./sensor.h"
#include "./image.h"

using espx::camx::Pixformat;
using espx::camx::Resolution;
using espx::camx::Sensor;
using espx::camx::Model;
using espx::camx::Quality;
using espx::camx::Image;

/**
 * Use camera with a fluent interface
 */
class Camx : public HasOpStatus {
public:
    camera_config_t config;
    camera_fb_t *fb;
    Pixformat pixformat;
    Resolution resolution;
    Model model;
    Quality quality;
    Sensor sensor;
    Image frame;
    QueueHandle_t queue;

    /**
     * Constructor
     */
    Camx() : fb(NULL), mutex(NULL), runningInBackground(false) {
        defaultConfig();
    }

    /**
     * Initialize camera
     * @return
     */
    Camx& begin() {
        // abort if model pins are not set
        if (model.pinout == NULL) {
            status.fail("Model not set");
            return *this;
        }

        config.pixel_format = pixformat.format;
        config.frame_size = resolution.framesize;
        config.jpeg_quality = quality.quality;
        config.pin_d0 = model.pinout->d0;
        config.pin_d1 = model.pinout->d1;
        config.pin_d2 = model.pinout->d2;
        config.pin_d3 = model.pinout->d3;
        config.pin_d4 = model.pinout->d4;
        config.pin_d5 = model.pinout->d5;
        config.pin_d6 = model.pinout->d6;
        config.pin_d7 = model.pinout->d7;
        config.pin_xclk = model.pinout->xclk;
        config.pin_pclk = model.pinout->pclk;
        config.pin_vsync = model.pinout->vsync;
        config.pin_href = model.pinout->href;
        config.pin_sccb_sda = model.pinout->sccb_sda;
        config.pin_sccb_scl = model.pinout->sccb_scl;
        config.pin_pwdn = model.pinout->pwdn;
        config.pin_reset = model.pinout->reset;

        if (psramFound() && config.fb_count == 0) {
            config.fb_count = 2;
            config.fb_location = CAMERA_FB_IN_PSRAM;
        }

        if (config.fb_count == 0)
            config.fb_count = 1;

        if (esp_camera_init(&config) != ESP_OK) {
            status.fail("esp_camera_init() failed");

            return *this;
        }

        mutex = xSemaphoreCreateMutex();
        queue = xQueueCreate(1, sizeof(Image*));
        status.succeed();

        sensor.begin(config.frame_size);

        return *this;
    }

    /**
     * Take picture
     */
    Image& grab() {
        if (!lock(30))
            return frame;

        free();

        fb = esp_camera_fb_get();

        if (fb != NULL) {
            frame.buf = fb->buf;
            frame.length = fb->len;
            frame.timestamp(fb->timestamp);
            // don't trust the fb dimensions!
            frame.width = resolution.width;
            frame.height = resolution.height;
            // TODO: push frame to queue
//            xQueueSendToFront(queue, (void *) (&frame), pdMS_TO_TICKS(10));
        }

        unlock();

        return frame;
    }

    /**
     * Release frame memory
     */
    void free() {
        if (fb != NULL) {
            esp_camera_fb_return(fb);
            fb = NULL;
        }

        frame.free();
    }

    /**
     * Run camera capture in the background
     */
    void runInBackground() {
        if (runningInBackground)
            return;

        threadx([](void *userdata) {
            Camx *self = (Camx*) userdata;
            size_t capturedAt = 0;

            while (true) {
                const size_t now = millis();

                // cap at 30 FPS
                if (now >= capturedAt && now < capturedAt + 33) {
                    delay(5);
                    continue;
                }

                self->grab();
                capturedAt = millis();
            }
        }, threadx.Important(), threadx.Userdata(this));

        runningInBackground = true;
    }

    /**
     * Peek frame from queue
     */
    void peek() {
        xQueuePeek(queue, &frame, pdMS_TO_TICKS(30));
    }

    /**
     * Acquire semaphore
     * @return
     */
    bool lock(size_t timeout = 100) {
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(timeout)) == pdTRUE)
            return true;

        return false;
    }

    /**
     * Release semaphore
     */
    void unlock() {
        xSemaphoreGive(mutex);
    }

protected:
    bool runningInBackground;
    SemaphoreHandle_t mutex;

    /**
     *
     */
    void defaultConfig() {
        config.pin_d0 = 0;
        config.pin_d1 = 0;
        config.fb_count = 0;
        config.xclk_freq_hz = 20000000ULL;
        config.grab_mode = CAMERA_GRAB_LATEST;
        config.ledc_channel = LEDC_CHANNEL_0;
        config.ledc_timer = LEDC_TIMER_0;
    }
};

// singleton
static Camx camx;

#pragma once
#include <esp_camera.h>
#include "../OpStatus.h"
#include "./pixformat.h"
#include "./resolution.h"
#include "./model.h"
#include "./quality.h"

using espx::camx::Pixformat;
using espx::camx::Resolution;
using espx::camx::Model;
using espx::camx::Quality;

/**
 * Use camera with a fluent interface
 */
class Camx : public HasOpStatus {
public:
    camera_config_t config;
    Pixformat pixformat;
    Resolution resolution;
    Model model;
    Quality quality;

    Camx() {
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

        if (esp_camera_init(&config) != ESP_OK) {
            status.fail("esp_camera_init() failed");

            return *this;
        }

        status.succeed();

        return *this;
    }

protected:

    /**
     *
     */
    void defaultConfig() {
        config.pin_d0 = 0;
        config.pin_d1 = 0;
        config.fb_count = 1;
        config.xclk_freq_hz = 20000000ULL;
        config.grab_mode = CAMERA_GRAB_LATEST;
        config.ledc_channel = LEDC_CHANNEL_0;
        config.ledc_timer = LEDC_TIMER_0;
    }
};

// singleton
static Camx camx;
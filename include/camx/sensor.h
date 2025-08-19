#pragma once
#include "../Str.h"
#include "../Intensity.h"

namespace espx::camx {
    /**
     * Configure camera sensor
     */
    class Sensor {
    public:
        sensor_t *sensor;

        /**
         * Constructor
         */
        Sensor() : sensor(NULL), q("") {

        }

        /**
         * Apply batched settings
         */
        void apply() {
            set(q);
        }

        /**
         *
         * @param s
         */
        void set(String& s) {
            Str ss(s);

            queue(ss);
        }

        /**
         *
         * @param s
         */
        void set(Str s) {
            ESP_LOGI("Sensor", "Applying... %s", s.value().c_str());

            if (s.contains("flip_v="))
                sensor->set_vflip(sensor, s.contains("flip_v=1"));

            if (s.contains("flip_h="))
                sensor->set_hmirror(sensor, s.contains("flip_h=1"));

            if (s.contains("sat=")) {
                Str sat(s.valueOf("sat", "base"));
                sensor->set_saturation(sensor, sat.intensity());
            }

            if (s.contains("bri=")) {
                Str bri(s.valueOf("bri", "base"));
                sensor->set_brightness(sensor, bri.intensity());
            }

            if (s.contains("awb="))
                sensor->set_whitebal(sensor, s.contains("awb=1"));

            if (s.contains("awbg="))
                sensor->set_awb_gain(sensor, s.contains("awbg=1"));

            if (s.contains("gctrl="))
                sensor->set_gain_ctrl(sensor, s.contains("gctrl=1"));

            if (s.contains("ectrl="))
                sensor->set_exposure_ctrl(sensor, s.contains("ectrl=1"));

            if (s.contains("agc="))
                sensor->set_gain_ctrl(sensor, s.contains("agc=1"));

            if (s.contains("aec2="))
                sensor->set_aec2(sensor, s.contains("aec2=1"));

            if (s.contains("dcw="))
                sensor->set_dcw(sensor, s.contains("dcw=1"));

            if (s.contains("bpc="))
                sensor->set_bpc(sensor, s.contains("bpc=1"));

            if (s.contains("wpc="))
                sensor->set_wpc(sensor, s.contains("wpc=1"));

            if (s.contains("mang="))
                sensor->set_agc_gain(sensor, s.valueOf("mang", 0));

            if (s.contains("aec="))
                sensor->set_aec_value(sensor, s.valueOf("aec", 0));

            if (s.contains("lc="))
                sensor->set_lenc(sensor, s.valueOf("lc", 0));

            if (s.contains("rgm="))
                sensor->set_raw_gma(sensor, s.valueOf("rgm", 0));

            if (s.contains("ceil="))
                sensor->set_gainceiling(sensor, (gainceiling_t) s.valueOf("ceil", 0));

            if (s.contains("eff="))
                sensor->set_special_effect(sensor, s.valueOf("eff", 0));

        }

        void flipVert(bool flip = true) {
            queue(Str::keyval("flip_v", (uint8_t) flip));
        }

        void flipHor(bool flip = true) {
            queue(Str::keyval("flip_h", (uint8_t) flip));
        }

        void automaticWhiteBalance(bool enabled = true) {
            queue(Str::keyval("awb", (uint8_t) enabled));
        }

        void automaticWhiteBalanceGain(bool enabled = true) {
            queue(Str::keyval("awbg", (uint8_t) enabled));
        }

        void automaticGainControl(bool enabled = true) {
            queue(Str::keyval("agc", (uint8_t) enabled));
        }

        void automaticExposureControl(bool enabled = true) {
            queue(Str::keyval("aec2", (uint8_t) enabled));
        }

        void gainControl(bool enabled = true) {
            queue(Str::keyval("gctrl", (uint8_t) enabled));
        }

        void exposureControl(bool enabled = true) {
            queue(Str::keyval("ectrl", (uint8_t) enabled));
        }

        void dcw(bool enabled = true) {
            queue(Str::keyval("dcw", (uint8_t) enabled));
        }

        void bpc(bool enabled = true) {
            queue(Str::keyval("bpc", (uint8_t) enabled));
        }

        void wpc(bool enabled = true) {
            queue(Str::keyval("wpc", (uint8_t) enabled));
        }

        void lensCorrection(bool enabled = true) {
            queue(Str::keyval("lc", (uint8_t) enabled));
        }

        void rawGamma(bool enabled = true) {
            queue(Str::keyval("rgm", (uint8_t) enabled));
        }

        void manualGain(uint8_t gain) {
            // gain in IDF goes from 0 to 30
            // this method accepts from 0 to 100
            queue(Str::keyval("mang", (int) map((float) gain, 0, 100, 0, 30)));
        }

        void manualExposure(uint8_t gain) {
            // gain in IDF goes from 0 to 1200
            // this method accepts from 0 to 100
            queue(Str::keyval("aec", (int) map((float) gain, 0, 100, 0, 1200)));
        }

        void gainCeiling(uint8_t ceiling) {
            // gain in IDF goes from 0 to 6
            queue(Str::keyval("ceil", ceiling));
        }

        void noSpecialEffect() {
            queue(Str::keyval("eff", 0));
        }

        void negative() {
            queue(Str::keyval("eff", 1));
        }

        void grayscale() {
            queue(Str::keyval("eff", 2));
        }

        void redTint() {
            queue(Str::keyval("eff", 3));
        }

        void greenTint() {
            queue(Str::keyval("eff", 4));
        }

        void blueTint() {
            queue(Str::keyval("eff", 5));
        }

        void sepia() {
            queue(Str::keyval("eff", 6));
        }

        void saturation(String s) {
            queue(Str::keyval("sat", s));
        }

        void saturation(Intensity sat) {
            queue(Str::keyval("sat", static_cast<int>(sat)));
        }

        void brightness(String s) {
            queue(Str::keyval("bri", s));
        }

        void brightness(Intensity bri) {
            queue(Str::keyval("bri", static_cast<int>(bri)));
        }

        /**
         * Init sensor instance
         */
        void begin(framesize_t framesize) {
            if (sensor == NULL)
                sensor = esp_camera_sensor_get();

            sensor->set_framesize(sensor, framesize);
        }

    protected:
        Str q;

        /**
         * Append setting to queue to batch apply later
         * @param setting
         */
        void queue(Str setting) {
            q.concat(' ', setting.value());
        }
    };
}
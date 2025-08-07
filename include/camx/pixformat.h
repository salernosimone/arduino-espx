#pragma once
#include <esp_camera.h>
#include "../Str.h"
#include "../prompts.h"

namespace espx::camx {
    /**
     * Set pixel format of camera
     */
    class Pixformat {
    public:
        pixformat_t format;

        /**
         * Constructor
         */
        Pixformat() {
            // default to jpeg
            jpeg();
        }

        /**
         *
         */
        void jpeg() {
            format = PIXFORMAT_JPEG;
        }

        /**
         *
         */
        void gray() {
            format = PIXFORMAT_GRAYSCALE;
        }

        /**
         *
         */
        void rgb() {
            format = PIXFORMAT_RGB565;
        }

        /**
         *
         */
        void raw() {
            format = PIXFORMAT_RGB888;
        }

        /**
         * Set from const char*
         * @param format
         */
        void set(const char *format) {
            String fmt(format);

            set(fmt);
        }

        /**
         * Set from string
         * @param format
         */
        void set(String& format) {
            Str fmt = str(format).lower();

            if (fmt.contains("jpeg"))
                jpeg();
            else if (fmt.contains("gray"))
                gray();
            else if (fmt.contains("565"))
                rgb();
            else if (fmt.contains("888") or fmt.contains("raw"))
                raw();
            else {
                Serial.printf("Unknown pixel format: %s", format.c_str());
            }
        }

        /**
         * Choose from a menu
         */
        void prompt() {
            static const String choices[] = {
                    "JPEG",
                    "GRAYSCALE",
                    "RGB 565",
                    "RGB 888"
            };

            const int choice = promptChoice("Select a pixel format", choices, 4);
            const String format = choices[choice];

            remember(format.c_str());
        }

    protected:

        /**
         * Show the user a message to skip prompting in the future
         * @param format
         */
        void remember(const String& format) {
            Serial.printf("You can skip prompting next time with `camx.pixformat.%s()`\n", str(format).lower().c_str());
            set(format.c_str());
        }
    };
}
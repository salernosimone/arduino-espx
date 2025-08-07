#pragma once
#include "../Str.h"
#include "../prompts.h"

namespace espx::camx {
    /**
     * Set JPEG quality
     */
    class Quality {
    public:
        uint8_t quality;

        /**
         * Constructor
         */
        Quality() {
            // default to high
            high();
        }

        /**
         * Choose from a menu
         */
        void prompt() {
            static const String choices[] = {
                    "WORST",
                    "LOW",
                    "BASE",
                    "HIGH",
                    "BEST"
            };

            const int choice = promptChoice("Select a JPEG quality", choices, 5);
            const String quality = choices[choice];

            remember(quality.c_str());
        }

        /**
         *
         * @param q
         */
        void set(const char *q) {
            if (strcmp(q, "WORST") == 0)
                worst();
            else if (strcmp(q, "LOW") == 0)
                low();
            else if (strcmp(q, "BASE") == 0)
                base();
            else if (strcmp(q, "HIGH") == 0)
                high();
            else if (strcmp(q, "BEST") == 0)
                best();
            else {
                Serial.printf("Unknown JPEG quality: %s", q);
            }
        }

        /**
         *
         * @param q
         */
        void set(String& q) {
            set(q.c_str());
        }

        /**
         * Set from 0 (worst) to 100 (best)
         * @param q
         */
        void set(uint8_t q) {
            quality = constrain(map((float) q, 0, 100, 64, 10), 10, 64);
        }

        inline void worst() {
            set((uint8_t) 0);
        }

        inline void low() {
            set((uint8_t) 30);
        }

        inline void base() {
            set((uint8_t) 50);
        }

        inline void high() {
            set((uint8_t) 80);
        }

        inline void best() {
            set((uint8_t) 100);
        }

    protected:

        /**
         * Show the user a message to skip prompting in the future
         * @param quality
         */
        void remember(const char* quality) {
            const char *q = str(quality).lower().c_str();

            Serial.printf("You can skip prompting next time with `camx.quality.%s()`\n", q);
            set(q);
        }
    };
}
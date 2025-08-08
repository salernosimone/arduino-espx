#pragma once
#include "./Str.h"

namespace espx {
    /**
     * Event throttling,
     * mainly for internal use
     */
    class Throttle {
    public:
        /**
         * Constructor
         */
        Throttle() : interval(0) {

        }

        /**
         * Constructor
         * @param interv
         */
        Throttle(size_t interv) : interval(interv) {

        }

        /**
         * Constructor
         * @param interv
         */
        Throttle(String& interv) {
            parse(interv.c_str());
        }

        /**
         * Allow `object.throttle("...")` syntax
         * @param interv
         */
        void operator()(const char* interv) {
            parse(interv);
        }

        /**
         * Allow `object.throttle("...")` syntax
         * @param interv
         */
        void operator()(String& interv) {
            parse(interv.c_str());
        }

        /**
         * Test if operation is allowed
         * @return
         */
        bool allowed() const {
            const size_t now = millis();

            return interval == 0 || now < tickedAt || now > tickedAt + interval;
        }

        /**
         *
         */
        inline void touch() {
            tickedAt = millis();
        }

    protected:
        size_t tickedAt;
        size_t interval;

        /**
         *
         * @param interv
         */
        void parse(const char *interv) {
            const uint16_t ms = str(interv).millis();

            if (ms)
                interval = ms;
            else
                ESP_LOGW("Throttle", "Can't parse interval: %s", interv);
        }
    };
}
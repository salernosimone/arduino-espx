#pragma once
#include "../OpStatus.h"

namespace espx::camx {
    /**
     * A wrapper around a camera frame
     */
    class Image : public HasOpStatus {
    public:
        uint8_t *buf;
        size_t length;
        uint16_t width;
        uint16_t height;

        /**
         * Constructor
         * @param buf_
         * @param length_
         * @param width_
         * @param height_
         */
        Image(uint8_t *buf_, size_t length_, uint16_t width_, uint16_t height_) :
            buf(buf_),
            length(length_),
            width(width_),
            height(height_) {

            if (buf == NULL || length == 0)
                status.fail("Empty frame");
        }

        /**
         * Release frame memory
         */
        void free() {
            buf = NULL;
            length = 0;
            width = 0;
            height = 0;
        }
    protected:
    };
}
#pragma once
#if defined(__JPEGDEC__)
#include "../helpers.h"
#include "../ByteArray.h"
#include "../OpStatus.h"
#include "../Stopwatch.h"


namespace espx::camx {
    /**
     * JPEG decoder
     * (meant to be extended by custom decoders)
     */
    class Jdecoder : public HasOpStatus {
    public:
        JPEGDEC jpegdec;
        Stopwatch stopwatch;

        /**
         *
         */
        Jdecoder() {
            defaults();
        }

    protected:
        struct {
            uint16_t width;
            uint16_t height;
        } dimensions;
        struct {
            uint8_t scale;
            float stride;
            uint32_t options;
        } jconfig;

        /**
         * Set decode stride
         */
        void stride(float stride) {
            if (stride < 1) {
                ESP_LOGW("JpegDecHandler", "Bad stride: %2.f. Must be >= 1", stride);
                return;
            }

            jconfig.stride = stride;
        }

        /**
         * Set custom options
         */
        inline void decodeOptions(uint32_t options) {
            jconfig.options = options;
        }

        /**
         * Decode in grayscale
         */
        inline void grayscale() {
            decodeOptions(jconfig.options | JPEG_LUMA_ONLY);
        }

        /**
         * Set JPEG decoding scale
         * @param scale
         */
        void scale(uint8_t scale) {
            uint8_t s = pow(2, log2(scale > 1 ? scale : 1));

            if (s > 8) {
                ESP_LOGW("JpegDecHandler", "Max scale is 8. Will switch to scale + stride");
                s = 8;
            }

            jconfig.scale = s;
            stride(cast<float>(scale) / s);
        }

        /**
         *
         */
        OpStatus& decode(uint8_t *buf, size_t len) {
            dimensions.width = 0;
            dimensions.height = 0;
            stopwatch.start();

            // open
            bool openOk = openFlash(buf, len);

            if (!openOk) {
                decodeJpegdecError(jpegdec.getLastError());
                onOpenError();

                return status;
            }
            else {
                dimensions.width = jpegdec.getWidth();
                dimensions.height = jpegdec.getHeight();

                status.succeed();
                onOpenSuccess();
            }

            // decode
            jpegdec.setUserPointer((void*) this);
            bool decodeOk = jpegdec.decode(0, 0, getFlags());

            jpegdec.close();

            if (!decodeOk) {
                decodeJpegdecError(jpegdec.getLastError());
                onDecodeError();
            }
            else {
                status.succeed();
                onDecodeSuccess();
                stopwatch.stop();
            }

            return status;
        }

        /**
         * Get decode flags
         */
        uint32_t getFlags() const {
            uint32_t flags = jconfig.options;

            if (jconfig.scale > 1)
                flags |= jconfig.scale;

            return flags;
        }

        /**
         * Set defaults config
         */
        void defaults() {
            dimensions.width = 0;
            dimensions.height = 0;
            jconfig.scale = 1;
            jconfig.stride = 1;
            jconfig.options = 0;
        }

        /**
         *
         */
        size_t numOutputPixels() {
            return cast<size_t>(dimensions.width) * dimensions.height;
        }

        /**
         * Implement custom JPEGDEC.openFLASH call
         * @param buf
         * @param length
         * @return
         */
        virtual bool openFlash(uint8_t *buf, size_t length) = 0;

        /**
         * Process one MCU at a time
         * (can be overriden by children)
         */
        virtual bool handleMCU(JPEGDRAW *mcu) {
            static ByteArray rowbuffer;

            for (float i = 0; i < mcu->iHeight; i += jconfig.stride) {
                const float y = mcu->y + i;
                const uint16_t rowIndex = y / jconfig.stride;

                if (exitAtRow(rowIndex))
                    return 0;

                const size_t offset = ((size_t) i) * mcu->iWidthUsed;
                const uint8_t *row = ((uint8_t*) mcu->pPixels) + offset;
                size_t w = 0;

                rowbuffer.resize(mcu->iWidthUsed / jconfig.stride);

                // copy pixels to buffer (accouting for stride)
                for (float j = 0; j < mcu->iWidthUsed; j += jconfig.stride, w++) {
                    const float x = mcu->x + j;
                    const uint8_t pixel = row[cast<size_t>(j)];

                    rowbuffer.set(w, pixel);
                }

                if (!handleRow(rowIndex, &rowbuffer, mcu->x, mcu->x + w))
                    return 0;

                // TODO: handle the case of color JPEG
                for (uint16_t j = 0; j < rowbuffer.size; j++)
                    handlePixel(rowIndex, mcu->x + j, rowbuffer(j));
          }

          return 1;
        }

        /**
         * Early exit decoding process
         */
        virtual bool exitAtRow(uint16_t row) {
            return false;
        }

        /**
         * Process row.
         * Early exit if returns false.
         */
        virtual bool handleRow(uint16_t y, ByteArray *data, uint16_t xstart, uint16_t xend) {
            return true;
        }

        /**
         * Process pixel
         */
        virtual void handlePixel(uint16_t y, uint16_t x, uint16_t pixel) {

        }

        /**
         * Called if open succeeded
         */
        virtual void onOpenSuccess() {
        }

        /**
         * Called if open failed
         */
        virtual void onOpenError() {
        }

        /**
         * Called if open succeeded
         */
        virtual void onDecodeSuccess() {
        }

        /**
         * Called if open failed
         */
        virtual void onDecodeError() {
        }

    private:

        /**
         *
         */
        void decodeJpegdecError(int code) {
            if (code == 0) {
                status.succeed();
                return;
            }

            switch (code) {
                case JPEG_INVALID_PARAMETER:
                    status.fail("Invalid parameter");
                    break;
                case JPEG_DECODE_ERROR:
                    status.fail("Decode error");
                    break;
                case JPEG_UNSUPPORTED_FEATURE:
                    status.fail("Unsupported feature");
                    break;
                case JPEG_INVALID_FILE:
                    status.fail("Invalid file");
                    break;
            }

            status.fail("Unknown JPEGDEC error");
        }
    };
}
#endif
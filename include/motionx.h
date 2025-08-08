#pragma once
#include "./math.h"
#include "./Str.h"
#include "./ByteArray.h"
#include "./Throttle.h"
#include "./camx/jdecoder.h"

using espx::Throttle;
using espx::camx::Jdecoder;

/**
 * Camera motion detection
 */
class Motionx : public Jdecoder {
public:
    /**
     * If true, pixel is included in count
     */
    using Include = std::function<bool(size_t x, size_t y)>;

    /**
     * If true, pixel is considered as "changed" from old
     */
    using Compare = std::function<bool(uint8_t curr, uint8_t old)>;

    ByteArray background;
    Throttle throttle;

    /**
     * Constructor
     */
    Motionx()  {
        trainFor(5);
        smooth(0.8);
        diffBy(10);
        threshold("30%");
        resetState();
        grayscale();
        config.include = [](uint16_t x, uint16_t y) { return true; };
    }

    /**
     * Convert to bool
     * @return
     */
    explicit operator bool() {
        if (!happened())
            return false;

        throttle.touch();
        return true;
    }

    /**
     *
     * @param epochs
     */
    void trainFor(size_t epochs) {
        config.trainEpochs = epochs;
    }

    /**
     *
     * @param factor
     */
    void smooth(float factor) {
        config.smoothFactor = factor;
    }

    /**
     *
     * @param diff
     */
    void diffBy(const uint8_t diff) {
        config.compare = [diff](uint8_t curr, uint8_t old) { return math::absdiff(curr, old) > diff; };
    }

    /**
     *
     * @param percent
     */
    void threshold(String percent) {
        config.threshold = str(percent).percent();
    }

    /**
     *
     * @param include
     */
    void include(Include include) {
        config.include = include;
    }

    /**
     * Get moving point ratio
     * @return
     */
    inline float ratio() const {
        return state.total > 0 ? cast<float>(state.moving) / state.total : 0;
    }

    /**
     *
     * @tparam Frame
     * @param frame
     * @return
     */
    template<class Frame>
    OpStatus& process(Frame& frame) {
        return process(frame.buf, frame.length);
    }

    /**
     *
     * @param buf
     * @param len
     * @return
     */
    OpStatus& process(uint8_t *buf, size_t len) {
        decode(buf, len);

        return status;
    }


protected:
    struct {
        size_t trainEpochs;
        float smoothFactor;
        float threshold;
        Include include;
        Compare compare;
    } config;
    struct {
        size_t epoch;
        size_t total;
        size_t moving;
    } state;

    /**
     * Check if motion happened
     * @return
     */
    inline bool happened() const {
        return !failed() && throttle.allowed() && ratio() >= config.threshold;
    }

    /**
     *
     */
    void resetState() {
        state.epoch = 0;
        resetRuntime();
    }

    /**
     *
     */
    void resetRuntime() {
        state.total = 0;
        state.moving = 0;
    }

    /**
     * Implement custom JPEGDEC.openFLASH call
     * @param buf
     * @param length
     * @return
     */
    virtual bool openFlash(uint8_t *buf, size_t length) {
        return jpegdec.openFLASH(buf, length, [](JPEGDRAW *mcu) -> int {
            Motionx *self = (Motionx*) mcu->pUser;

            return self->handleMCU(mcu);
        });
    }

    /**
     *
     */
    void onOpenSuccess() {
        const uint16_t w = dimensions.width;

        resetRuntime();

        // choose scale based on input size
        if (w >= 1200) scale(16);
        else if (w >= 640) scale(8);
        else if (w >= 320) scale(4);
        else if (w >= 160) scale(2);
        else scale(1);

        background.resize(numOutputPixels() / pow(jconfig.scale * jconfig.stride, 2));
        background.resetIterator();

        ESP_LOGD("Motionx", "Got frame of size %dx%d. Decoding with scale=%d, stride=%.1f", dimensions.width, dimensions.height, jconfig.scale, jconfig.stride);

        if (state.epoch == 0)
            ESP_LOGD("Motionx", "First frame received, setting background...");
        else if (state.epoch < config.trainEpochs)
            ESP_LOGD("Motionx", "Updating background without detecting...");
    }

    /**
     *
     */
    void onDecodeSuccess() {
        state.epoch += 1;
    }

    /**
     *
     * @param y
     * @param x
     * @param pixel
     * @return
     */
    void handlePixel(uint16_t y, uint16_t x, uint16_t pixel) {
         if (!config.include(x, y))
           return;

        // on first frame, just set the background
        if (state.epoch == 0)
            background.append(pixel);
        // during training, only smooth background
        else if (state.epoch < config.trainEpochs)
            background.append(pixel, config.smoothFactor);
        else {
            // check moving pixels
            state.total += 1;
            state.moving += config.compare(pixel, background.tail()) ? 1 : 0;
            background.append(pixel, config.smoothFactor);
        }
    }
};

// singleton
Motionx motionx;
#pragma once

/**
 * A resizeable uint8_t buffer
 */
class ByteArray {
public:
    uint8_t *data;
    size_t size;
    size_t width;

    /**
     * Constructor
     */
    ByteArray() : data(NULL), size(0), width(0) {
        state.it = 0;
    }

    /**
     * Short for get()
     * @param i
     * @return
     */
    uint8_t operator()(size_t i) const {
        return get(i);
    }

    /**
     *
     */
    void resetIterator() {
        state.it = 0;
    }

    /**
     * Get current element
     * (when using iterator interface)
     * @return
     */
    uint8_t tail() {
        return get(state.it);
    }

    /**
     * Get element at index
     * @param i
     * @return
     */
    uint8_t get(size_t i) const {
        return i < size ? data[i] : 0;
    }

    /**
     * Get element at (row, column)
     * @param y
     * @param x
     * @return
     */
    uint8_t get(size_t y, size_t x) const {
        return get(y * width + x);
    }

    /**
     * Append value at the end
     * @param value
     */
    void append(uint8_t value) {
        set(state.it++, value);
    }

    /**
     * Append value at the end
     * with smoothing
     * @param value
     * @param smooth
     */
    void append(uint8_t value, float smooth) {
        this->smooth(state.it++, value, smooth);
    }

    /**
     * Update value at index
     * @param i
     * @param value
     */
    void set(size_t i, uint8_t value) {
        if (i < size)
            data[i] = value;
    }

    /**
     * Update value at (row, column)
     * @param y
     * @param x
     * @param value
     */
    void set(size_t y, size_t x, uint8_t value) {
        set(y * width + x, value);
    }

    /**
     * Update value at index with smooth factor
     * @param i
     * @param value
     * @param smooth
     */
    void smooth(size_t i, uint8_t value, float smooth) {
        set(i, value * (1 - smooth) + get(i) * smooth);
    }

    /**
     * Update value at (row, column) with smooth factor
     * @param y
     * @param x
     * @param value
     * @param smooth
     */
    void smooth(size_t y, size_t x, uint8_t value, float smooth) {
        this->smooth(y * width + x, value, smooth);
    }

    /**
     * Set all values to 0
     */
    void zero() {
        memset(data, 0, size);
    }

    /**
     * Resize (keep width)
     * @param size
     */
    void resize(size_t size) {
        resize(size, width);
    }

    /**
     * Resize (set new width)
     * @param size
     * @param width
     */
    void resize(size_t size, size_t width) {
        if (data == NULL) {
            malloc(size);
            zero();
        } else if (size > this->size) {
            realloc(size);
            this->size = size;
        }

        this->width = width;
    }

    /**
     * Dump data
     * @param printer
     * @param delimiter
     */
    void dump(Print& printer, const char delimiter = ',') {
        if (!size)
            return;

        printer.print(data[0]);

        for (size_t i = 1; i < size; i++) {
            printer.print(delimiter);
            printer.print(data[i]);
        }

        printer.println();
    }

protected:
    struct {
        size_t it;
    } state;

    /**
     * Alloc memory from scratch
     * @param size
     */
    void malloc(size_t size) {
        if (psramFound()) {
            ESP_LOGD("ByteArray", "Allocating %d bytes into PSRAM. May cause a reboot if not enough is available", size);
            data = (uint8_t *) ps_malloc(size);
        } else {
            ESP_LOGW("ByteArray", "Allocating %d bytes into RAM. May cause a reboot if not enough is available", size);
            data = (uint8_t *) ::malloc(size);
        }
    }

    /**
     * (Re)Alloc memory
     * @param size
     */
    void realloc(size_t size) {
        if (size <= this->size)
            return;

        if (data == NULL) {
            malloc(size);
            return;
        }

        if (psramFound()) {
            ESP_LOGI("ByteArray", "(Re)Allocating %d bytes into PSRAM. May cause a reboot if not enough is available",
                     size);
            data = (uint8_t *) ps_realloc(data, size);
        } else {
            ESP_LOGW("ByteArray", "(Re)Allocating %d bytes into RAM. May cause a reboot if not enough is available", size);
            data = (uint8_t *) ::realloc(data, size);
        }
    }
};

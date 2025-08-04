#pragma once

/**
 * A simplified Python's StringIO implementation
 */
class StringIO : public Print {
public:
    /**
     * Constructor
     * @param initialValue
     */
    StringIO(const String& initialValue = "") : s(initialValue) {

    }

    /**
     * Get value
     * @return
     */
    String value() {
        return s;
    }

    /**
     *
     */
    void clear() {
        s = "";
    }

    /**
     *
     * @param c
     * @return
     */
    size_t write(uint8_t c) {
        s += (char) c;

        return 1;
    }

    /**
     *
     * @param chars
     */
    void reserve(size_t chars) {
        s.reserve(chars);
    }

protected:
    String s;
};
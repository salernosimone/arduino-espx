#pragma once
#include <random>

/**
 * String utilities
 */
class Str {
public:
    /**
     * Constructor
     * @param initialValue
     */
    explicit Str(const char *initialValue) : s(initialValue) {

    }

    /**
     * Constructor
     * @param initialValue
     */
    explicit Str(const String& initialValue) : s(initialValue) {

    }

    /**
     * Generate random string of given length
     * @param length
     * @return
     */
    static String random(const uint8_t length = 16) {
        static const char alphanum[] =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789";

        String s;

        s.reserve(length);
        randomSeed(millis());

        // first character is always upper letter
        s += alphanum[::random(0, 26)];

        for (uint8_t i = 1; i < length; i++)
            s += alphanum[::random(0, 62)];

        return s;
    }

    /**
     * Test if string contains other string
     * @param other
     * @return
     */
    inline bool contains(const String &other) const {
        return s.indexOf(other) >= 0;
    }

    /**
     * Parse string as (file) weight (kb, mb...)
     * @return
     */
    size_t weight() {
        const size_t value = s.toInt();

        if (contains("g") || contains("G"))
            return value * 1073741824ULL;

        if (contains("m") || contains("M"))
            return value * 1048576ULL;

        if (contains("k") || contains("K"))
            return value * 1024;

        return value;
    }

protected:
    String s;
};

/**
 * Str factory
 * @param s
 * @return
 */
Str str(const String s) {
    return Str(s);
}
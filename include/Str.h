#pragma once
#include <random>

/**
 * String utilities
 */
class Str {
public:
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
        randomSeed(::millis());

        // first character is always upper letter
        s += alphanum[::random(0, 26)];

        for (uint8_t i = 1; i < length; i++)
            s += alphanum[::random(0, 62)];

        return s;
    }

    /**
     * Constructor
     * @param initialValue
     */
    explicit Str(int initialValue) : s(initialValue) {

    }

    /**
     * Constructor
     * @param initialValue
     */
    explicit Str(float initialValue) : s(initialValue) {

    }

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
     *
     * @return
     */
    String& value() {
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
     *
     * @param prefix
     * @return
     */
    inline bool startsWith(const String &prefix) const {
        return s.startsWith(prefix);
    }

    /**
     *
     * @param suffix
     * @return
     */
    inline bool endsWith(const String &suffix) const {
        return s.endsWith(suffix);
    }

    /**
     * Parse string as (file) weight (kb, mb...)
     * @return
     */
    size_t weight() {
        const float value = s.toFloat();
        Str other(s);

        other.s.toLowerCase();

        if (other.contains("g"))
            return value * 1073741824ULL;

        if (other.contains("m"))
            return value * 1048576ULL;

        if (other.contains("k"))
            return value * 1024;

        return value;
    }

    /**
     * Parse string as (file) weight (kb, mb...)
     * @return
     */
    size_t millis() {
        const float value = s.toFloat();
        Str other(s);

        other.s.toLowerCase();

        if (other.endsWith("ms") || other.contains("milli"))
            return value;

        if (other.endsWith("s") || other.contains("second"))
            return value * 1000;

        if (other.endsWith("m") || other.contains("minute"))
            return value * 1000 * 60;

        if (other.endsWith("h") || other.contains("hour"))
            return value * 1000 * 3600;

        return value;
    }

    /**
     * Join parts (stop condition)
     * @tparam T
     * @param glue
     * @param arg
     * @param args
     * @return
     */
    template<typename T>
    Str &join(String glue, T arg) {
        return concat(glue, arg);
    }

    /**
     * Join parts
     * @tparam T
     * @tparam Args
     * @param glue
     * @param arg
     * @param args
     * @return
     */
    template<typename T, typename... Args>
    Str &join(String glue, T arg, Args... args) {
        concat(glue, arg);

        return join(glue, args...);
    }

    /**
     * Concat stop condition (generic)
     * @tparam T
     * @param value
     * @return
     */
    template<typename T>
    Str &concat(T value) {
        s += value;

        return *this;
    }

    /**
     * Concat multiple values
     * @tparam T
     * @tparam Args
     * @param value
     * @param args
     * @return
     */
    template<typename T, typename... Args>
    Str &concat(T value, Args... args) {
        concat(value);

        return concat(args...);
    }

protected:
    String s;
};

/**
 * Str factory
 * @param s
 * @return
 */
 template<typename T>
Str str(T s) {
    return Str(s);
}
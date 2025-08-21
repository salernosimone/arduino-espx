#pragma once

/**
 * Track execution time
 */
class Stopwatch {
public:
    /**
     * Constructor
     */
    Stopwatch(): startedAt(0), stoppedAt(0) {

    }

    /**
     *
     */
    inline void start() {
        startedAt = ::micros();
    }

    /**
     *
     */
    inline void stop() {
        stoppedAt = ::micros();
    }

    /**
     *
     * @return
     */
    inline size_t micros() {
        return stoppedAt > startedAt ? stoppedAt - startedAt : 0;
    }

    /**
     *
     * @return
     */
    inline size_t millis() {
        return stoppedAt > startedAt + 999 ? (stoppedAt - startedAt) / 1000 : 0;
    }

protected:
    size_t startedAt;
    size_t stoppedAt;
};
#pragma once

namespace math {
    /**
     * Overflow-safe absolute difference
     * @tparam T
     * @param a
     * @param b
     * @return
     */
    float absdiff(float a, float b) {
        return a > b ? a - b : b - a;
    }
}
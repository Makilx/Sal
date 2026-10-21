#pragma once

#include <SalCore/Precompiled.hpp>

namespace sal {
    inline constexpr float PI = 3.14159265358979323846f;
    inline constexpr float EPSILON = 1e-6f;
    inline constexpr float INF = std::numeric_limits<float>::infinity();

    // --- //

    template <typename T>
    inline T Max(const T &x, const T &y) {
        return x > y ? x : y;
    }

    template <typename T>
    inline T Min(const T &x, const T &y) {
        return x < y ? x : y;
    }

    template <typename T>
    inline T Clamp(const T &min, const T &max, const T &value) {
        return value < min ? min : (value > max ? max : value);
    }

    template <typename T>
    inline T Clamp01(const T &value) {
        return value < T(0) ? T(0) : (value > T(1) ? T(1) : value);
    }

    template <typename T>
    inline T Lerp(const T &start, const T &end, float time) {
        return start + (end - start) * time;
    }

    template <typename T>
    inline T ILerp(const T &start, const T &end, float time) {
        return (time - start) / (end - start);
    }

    template <typename T>
    inline T SmoothStep(const T &start, const T &end, float time) {
        time = Clamp01(time);
        time = time * time * (3.0f - 2.0f * time);
        return Lerp(start, end, time);
    }

    // --- //

    template <typename T>
    inline T Sign(const T &value) {
        return (T(0) < value) - (value < T(0));
    }

    template <typename T>
    inline T Abs(const T &value) {
        return value < T(0) ? -value : value;
    }

    template <typename T>
    inline T Pow(const T &value) {
        return value * value;
    }

    template <typename T>
    inline T Pow(const T &value, const T &exponent) {
        return std::pow(value, exponent);
    }

    inline float Pow(float value, float exponent) {
        return std::powf(value, exponent);
    }

    template <typename T>
    inline T Sqrt(const T &value) {
        return std::sqrt(value);
    }

    inline float Sqrt(float value) { return std::sqrtf(value); }

    template <typename T>
    inline T ISqrt(const T &value) {
        return T(1) / Sqrt(value);
    }

    inline float ISqrt(float value) { return 1.0f / std::sqrtf(value); }

    // --- //

    inline float Rad(float degrees) { return degrees * (PI / 180.0f); }

    inline float Deg(float radians) { return radians * (180.0f / PI); }

    inline float Sin(float radians) { return std::sinf(radians); }

    inline float Cos(float radians) { return std::cosf(radians); }

    inline float Tan(float radians) { return std::tanf(radians); }

    // --- //

    inline int Round(float value) {
        return static_cast<int>(std::lroundf(value));
    }

    inline int Floor(float value) {
        return static_cast<int>(std::floorf(value));
    }

    inline int Ceil(float value) { return static_cast<int>(std::ceilf(value)); }

    // --- //

    template <typename T>
    inline T Snap(const T &value, const T &step) {
        return step == T(0) ? value : std::round(value / step) * step;
    }

    // --- //

    template <typename T>
    inline bool NearlyEqual(const T &x, const T &y,
                            const T &epsilon = T(EPSILON)) {
        return Abs(x - y) <= epsilon;
    }

    template <typename T>
    inline bool IsZero(const T &value, const T &epsilon = T(EPSILON)) {
        return Abs(value) <= epsilon;
    }

    template <typename T>
    inline T Mod(const T &x, const T &y) {
        return x - y * std::floor(x / y);
    }

    inline float Mod(float x, float y) { return std::fmodf(x, y); }

    inline float Wrap(float value, float min, float max) {
        const float range = max - min;

        if (range == 0.0f) {
            return min;
        }

        value = std::fmodf(value - min, range);

        if (value < 0.0f) {
            value += range;
        }

        return value + min;
    }
}
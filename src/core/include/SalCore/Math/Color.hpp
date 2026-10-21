#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Math.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    struct HSVCode {
        float h = 0.0f; // 0.0f to 1.0f
        float s = 0.0f; // 0.0f to 1.0f
        float v = 0.0f; // 0.0f to 1.0f
    };

    struct Color {
        float R = 1.0f, G = 1.0f, B = 1.0f, A = 1.0f;

        // Constants
        static const Color Transparent;
        static const Color Black;
        static const Color White;
        static const Color Gray;

        static const Color Red;
        static const Color Green;
        static const Color Blue;

        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;

        static const Color Orange;
        static const Color Purple;
        static const Color Pink;

        // Constructors
        constexpr Color() noexcept = default;

        explicit constexpr Color(const float value) noexcept
            : R(value), G(value), B(value), A(1.0f) {}

        constexpr Color(const float R, const float G, const float B,
                        const float A = 1.0f) noexcept
            : R(R), G(G), B(B), A(A) {}

        // Factory Methods
        static constexpr Color FromRGB255(const float value = 255.0f) noexcept {
            const float normalized = Clamp01(value / 255.0f);

            return Color(normalized, normalized, normalized, 1.0f);
        }

        static constexpr Color FromRGB255(const float R, const float G,
                                          const float B,
                                          const float A = 255.0f) noexcept {
            return Color(Clamp01(R / 255.0f), Clamp01(G / 255.0f),
                         Clamp01(B / 255.0f), Clamp01(A / 255.0f));
        }

        static Color FromHSV(const float h, const float s, const float v,
                             const float A = 1.0f) noexcept {
            float hue = std::fmod(h, 1.0f);

            if (hue < 0.0f) {
                hue += 1.0f;
            }

            hue *= 360.0f;

            const float sat = Clamp01(s);
            const float val = Clamp01(v);

            const float c = val * sat;
            const float X =
                c * (1.0f - Abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
            const float m = val - c;

            float r1 = 0.0f;
            float g1 = 0.0f;
            float b1 = 0.0f;

            if (hue < 60.0f) {
                r1 = c;
                g1 = X;
            } else if (hue < 120.0f) {
                r1 = X;
                g1 = c;
            } else if (hue < 180.0f) {
                g1 = c;
                b1 = X;
            } else if (hue < 240.0f) {
                g1 = X;
                b1 = c;
            } else if (hue < 300.0f) {
                r1 = X;
                b1 = c;
            } else {
                r1 = c;
                b1 = X;
            }

            return Color(r1 + m, g1 + m, b1 + m, Clamp01(A));
        }

        static Color FromHSV(const HSVCode code) noexcept {
            return FromHSV(code.h, code.s, code.v);
        }

        static Color FromHSV255(const float h, const float s, const float v,
                                const float A = 255.0f) noexcept {
            return FromHSV(h, s / 255.0f, v / 255.0f, A / 255.0f);
        }

        static Color FromHex(const StringView hexCode) noexcept {
            StringView target = hexCode;

            if (!target.empty() && target[0] == '#') {
                target.remove_prefix(1);
            }

            unsigned int hexValue = 0;

            for (const char c : target) {
                hexValue <<= 4;

                if (c >= '0' && c <= '9') {
                    hexValue += c - '0';
                } else if (c >= 'A' && c <= 'f') {
                    hexValue += c - 'A' + 10;
                } else if (c >= 'A' && c <= 'F') {
                    hexValue += c - 'A' + 10;
                }
            }

            if (target.length() == 6) {
                const float R =
                    static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;

                const float G =
                    static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;

                const float B = static_cast<float>(hexValue & 0xFF) / 255.0f;

                return Color(R, G, B, 1.0f);
            }

            if (target.length() == 8) {
                const float R =
                    static_cast<float>((hexValue >> 24) & 0xFF) / 255.0f;

                const float G =
                    static_cast<float>((hexValue >> 16) & 0xFF) / 255.0f;

                const float B =
                    static_cast<float>((hexValue >> 8) & 0xFF) / 255.0f;

                const float A = static_cast<float>(hexValue & 0xFF) / 255.0f;

                return Color(R, G, B, A);
            }

            return Color::White;
        }

        // Methods
        constexpr Color Mix(const Color target,
                            const float time) const noexcept {
            return Color(Lerp(R, target.R, time), Lerp(G, target.G, time),
                         Lerp(B, target.B, time), Lerp(A, target.A, time));
        }

        String ToHex() const {
            char buffer[10];

            std::snprintf(buffer, sizeof(buffer), "#%02X%02X%02X%02X",
                          static_cast<int>(R * 255.0f + 0.5f),
                          static_cast<int>(G * 255.0f + 0.5f),
                          static_cast<int>(B * 255.0f + 0.5f),
                          static_cast<int>(A * 255.0f + 0.5f));

            return String(buffer);
        }

        HSVCode ToHSV() const noexcept {
            const float maxValue = Max(R, Max(G, B));
            const float minValue = Min(R, Min(G, B));
            const float delta = maxValue - minValue;

            HSVCode result;
            result.v = maxValue;

            if (maxValue > 0.0f) {
                result.s = delta / maxValue;
            }

            if (delta == 0.0f) {
                result.h = 0.0f;
            } else {
                if (maxValue == R) {
                    result.h = (G - B) / delta + (G < B ? 6.0f : 0.0f);
                } else if (maxValue == G) {
                    result.h = (B - R) / delta + 2.0f;
                } else {
                    result.h = (R - G) / delta + 4.0f;
                }

                result.h /= 6.0f;
            }

            return result;
        }

        // ToString
        String ToString() const {
            StringStream stream;

            stream << std::fixed << std::setprecision(3) << R << ", " << G
                   << ", " << B << ", " << A;

            return stream.str();
        }
    };

    // Constants
    inline constexpr Color Color::Transparent{0.0f, 0.0f, 0.0f, 0.0f};
    inline constexpr Color Color::Black{0.0f, 0.0f, 0.0f, 1.0f};
    inline constexpr Color Color::White{1.0f, 1.0f, 1.0f, 1.0f};
    inline constexpr Color Color::Gray{0.5f, 0.5f, 0.5f, 1.0f};

    inline constexpr Color Color::Red{1.0f, 0.0f, 0.0f, 1.0f};
    inline constexpr Color Color::Green{0.0f, 1.0f, 0.0f, 1.0f};
    inline constexpr Color Color::Blue{0.0f, 0.0f, 1.0f, 1.0f};

    inline constexpr Color Color::Yellow{1.0f, 1.0f, 0.0f, 1.0f};
    inline constexpr Color Color::Cyan{0.0f, 1.0f, 1.0f, 1.0f};
    inline constexpr Color Color::Magenta{1.0f, 0.0f, 1.0f, 1.0f};

    inline constexpr Color Color::Orange{1.0f, 0.5f, 0.0f, 1.0f};
    inline constexpr Color Color::Purple{0.5f, 0.0f, 0.5f, 1.0f};
    inline constexpr Color Color::Pink{1.0f, 0.75f, 0.8f, 1.0f};

    Sal_Stringify_Prebuilt(Color);
}
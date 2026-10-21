#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Math.hpp>
#include <SalCore/Math/Vector3.hpp>
#include <SalCore/Math/Vector4.hpp>

namespace sal {
    struct Matrix4 {
        float elements[16] = {0.0f};

        // Constructors
        constexpr Matrix4() noexcept = default;

        constexpr Matrix4(float m00, float m10, float m20, float m30, float m01,
                          float m11, float m21, float m31, float m02, float m12,
                          float m22, float m32, float m03, float m13, float m23,
                          float m33) noexcept {
            elements[0] = m00;
            elements[1] = m10;
            elements[2] = m20;
            elements[3] = m30;

            elements[4] = m01;
            elements[5] = m11;
            elements[6] = m21;
            elements[7] = m31;

            elements[8] = m02;
            elements[9] = m12;
            elements[10] = m22;
            elements[11] = m32;

            elements[12] = m03;
            elements[13] = m13;
            elements[14] = m23;
            elements[15] = m33;
        }

        // Data Accessors
        const float *Data() const noexcept { return elements; }

        float *Data() noexcept { return elements; }

        // Factory Methods
        static constexpr Matrix4 Identity() noexcept {
            Matrix4 result;

            result.elements[0] = 1.0f;
            result.elements[5] = 1.0f;
            result.elements[10] = 1.0f;
            result.elements[15] = 1.0f;

            return result;
        }

        // Transformation Factories
        static constexpr Matrix4
        Translation(const Vector3 translation) noexcept {
            Matrix4 result = Identity();

            result.elements[12] = translation.X;
            result.elements[13] = translation.Y;
            result.elements[14] = translation.Z;

            return result;
        }

        static constexpr Matrix4 Scale(const Vector3 scale) noexcept {
            Matrix4 result = Identity();

            result.elements[0] = scale.X;
            result.elements[5] = scale.Y;
            result.elements[10] = scale.Z;

            return result;
        }

        // Projections
        static Matrix4 Perspective(float fovDegrees, float aspectRatio,
                                   float nearPlane, float farPlane) noexcept {
            Matrix4 result;

            float tanHalfFov = Tan(Rad(fovDegrees) * 0.5f);

            result.elements[0] = 1.0f / (aspectRatio * tanHalfFov);
            result.elements[5] = 1.0f / tanHalfFov;
            result.elements[10] =
                -(farPlane + nearPlane) / (farPlane - nearPlane);
            result.elements[11] = -1.0f;
            result.elements[14] =
                -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

            return result;
        }

        static constexpr Matrix4 Ortho(float left, float right, float bottom,
                                       float top, float nearPlane,
                                       float farPlane) noexcept {
            Matrix4 result = Identity();

            result.elements[0] = 2.0f / (right - left);
            result.elements[5] = 2.0f / (top - bottom);
            result.elements[10] = -2.0f / (farPlane - nearPlane);

            result.elements[12] = -(right + left) / (right - left);
            result.elements[13] = -(top + bottom) / (top - bottom);
            result.elements[14] =
                -(farPlane + nearPlane) / (farPlane - nearPlane);

            return result;
        }

        // Operators
        constexpr Matrix4 operator*(const Matrix4 other) const noexcept {
            Matrix4 result;

            for (int col = 0; col < 4; ++col) {
                for (int row = 0; row < 4; ++row) {
                    float sum = 0.0f;

                    for (int i = 0; i < 4; ++i) {
                        sum +=
                            elements[row + i * 4] * other.elements[i + col * 4];
                    }

                    result.elements[row + col * 4] = sum;
                }
            }

            return result;
        }

        constexpr Vector4 operator*(const Vector4 vector) const noexcept {
            return {elements[0] * vector.X + elements[4] * vector.Y +
                        elements[8] * vector.Z + elements[12] * vector.W,

                    elements[1] * vector.X + elements[5] * vector.Y +
                        elements[9] * vector.Z + elements[13] * vector.W,

                    elements[2] * vector.X + elements[6] * vector.Y +
                        elements[10] * vector.Z + elements[14] * vector.W,

                    elements[3] * vector.X + elements[7] * vector.Y +
                        elements[11] * vector.Z + elements[15] * vector.W};
        }

        // ToString
        String ToString() const {
            StringStream stream;

            stream << std::fixed << std::setprecision(3) << "  [" << elements[0]
                   << ", " << elements[4] << ", " << elements[8] << ", "
                   << elements[12] << "]\n"

                   << "  [" << elements[1] << ", " << elements[5] << ", "
                   << elements[9] << ", " << elements[13] << "]\n"

                   << "  [" << elements[2] << ", " << elements[6] << ", "
                   << elements[10] << ", " << elements[14] << "]\n"

                   << "  [" << elements[3] << ", " << elements[7] << ", "
                   << elements[11] << ", " << elements[15] << "]\n";

            return stream.str();
        }
    };

    Sal_Stringify_Prebuilt(Matrix4);
}
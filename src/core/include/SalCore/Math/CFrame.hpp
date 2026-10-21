#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Vector3.hpp>
#include <SalCore/Math/Vector4.hpp>
#include <SalCore/Math/Matrix4.hpp>

namespace sal {
    struct CFrameRotation {
        Vec3 right = {1.0f, 0.0f, 0.0f};
        Vec3 up = {0.0f, 1.0f, 0.0f};
        Vec3 lookAt = {0.0f, 0.0f, -1.0f};
    };

    struct CFrame {
        Vec3 position = {0.0f, 0.0f, 0.0f};
        Vec4 rotation = {0.0f, 0.0f, 0.0f, 1.0f};

        static const CFrame Identity;

        constexpr CFrame() noexcept = default;

        explicit constexpr CFrame(const Vec3 pos) noexcept
            : position(pos), rotation({0.0f, 0.0f, 0.0f, 1.0f}) {}

        constexpr CFrame(const float posX, const float posY,
                         const float posZ) noexcept
            : position({posX, posY, posZ}), rotation({0.0f, 0.0f, 0.0f, 1.0f}) {
        }

        constexpr CFrame(const Vec3 pos, const Vec4 quat) noexcept
            : position(pos), rotation(quat) {}

        constexpr CFrame(const float posX, const float posY, const float posZ,
                         const float qX, const float qY, const float qZ,
                         const float qW) noexcept
            : position({posX, posY, posZ}), rotation({qX, qY, qZ, qW}) {}

        static CFrame LookAt(const Vec3 &eye, const Vec3 &target,
                             const Vec3 &upVector = Vec3::YAxis) noexcept {
            const Vec3 look = (target - eye).Normalized();
            const Vec3 right = upVector.Cross(look).Normalized();
            const Vec3 up = look.Cross(right).Normalized();

            const float trace = right.X + up.Y - look.Z;

            Vec4 quaternion;

            if (trace > 0.0f) {
                const float s = Sqrt(trace + 1.0f) * 2.0f;

                quaternion.W = 0.25f * s;
                quaternion.X = (up.Z + look.Y) / s;
                quaternion.Y = (right.Z - look.X) / s;
                quaternion.Z = (right.Y - up.X) / s;
            } else if (right.X > up.Y && right.X > -look.Z) {
                const float s = Sqrt(1.0f + right.X - up.Y + look.Z) * 2.0f;

                quaternion.W = (up.Z + look.Y) / s;
                quaternion.X = 0.25f * s;
                quaternion.Y = (right.Y + up.X) / s;
                quaternion.Z = (right.Z - look.X) / s;
            } else if (up.Y > -look.Z) {
                const float s = Sqrt(1.0f - right.X + up.Y + look.Z) * 2.0f;

                quaternion.W = (right.Z - look.X) / s;
                quaternion.X = (right.Y + up.X) / s;
                quaternion.Y = 0.25f * s;
                quaternion.Z = (up.Z + look.Y) / s;
            } else {
                const float s = Sqrt(1.0f - right.X - up.Y - look.Z) * 2.0f;

                quaternion.W = (right.Y - up.X) / s;
                quaternion.X = (right.Z + look.X) / s;
                quaternion.Y = (up.Z + look.Y) / s;
                quaternion.Z = 0.25f * s;
            }

            return {eye, quaternion};
        }

        static CFrame FromEulerAnglesXYZ(const float X, const float Y,
                                         const float Z) noexcept {
            const float halfX = X * 0.5f;
            const float halfY = Y * 0.5f;
            const float halfZ = Z * 0.5f;

            const float sx = Sin(halfX);
            const float cx = Cos(halfX);
            const float sy = Sin(halfY);
            const float cy = Cos(halfY);
            const float sz = Sin(halfZ);
            const float cz = Cos(halfZ);

            const Vec4 quaternion = {
                sx * cy * cz - cx * sy * sz, cx * sy * cz + sx * cy * sz,
                cx * cy * sz - sx * sy * cz, cx * cy * cz + sx * sy * sz};

            return {Vec3::Zero, quaternion};
        }

        static CFrame FromEulerAnglesYXZ(const float X, const float Y,
                                         const float Z) noexcept {
            const float halfX = X * 0.5f;
            const float halfY = Y * 0.5f;
            const float halfZ = Z * 0.5f;

            const float sx = Sin(halfX);
            const float cx = Cos(halfX);
            const float sy = Sin(halfY);
            const float cy = Cos(halfY);
            const float sz = Sin(halfZ);
            const float cz = Cos(halfZ);

            const Vec4 quaternion = {
                sx * cy * cz - cx * sy * sz, cx * sy * cz + sx * cy * sz,
                cx * cy * sz + sx * sy * cz, cx * cy * cz - sx * sy * sz};

            return {Vec3::Zero, quaternion};
        }

        Matrix4 ToMatrix4() const noexcept {
            const float X = rotation.X;
            const float Y = rotation.Y;
            const float Z = rotation.Z;
            const float W = rotation.W;

            Matrix4 matrix = Matrix4::Identity();

            matrix.elements[0] = 1.0f - 2.0f * (Y * Y + Z * Z);
            matrix.elements[1] = 2.0f * (X * Y + Z * W);
            matrix.elements[2] = 2.0f * (X * Z - Y * W);

            matrix.elements[4] = 2.0f * (X * Y - Z * W);
            matrix.elements[5] = 1.0f - 2.0f * (X * X + Z * Z);
            matrix.elements[6] = 2.0f * (Y * Z + X * W);

            matrix.elements[8] = 2.0f * (X * Z + Y * W);
            matrix.elements[9] = 2.0f * (Y * Z - X * W);
            matrix.elements[10] = 1.0f - 2.0f * (X * X + Y * Y);

            matrix.elements[12] = position.X;
            matrix.elements[13] = position.Y;
            matrix.elements[14] = position.Z;

            return matrix;
        }

        constexpr bool operator==(const CFrame &cframe) const noexcept {
            return position == cframe.position && rotation == cframe.rotation;
        }

        constexpr bool operator!=(const CFrame &cframe) const noexcept {
            return !(*this == cframe);
        }

        CFrame operator*(const CFrame &cframe) const noexcept {
            const Vec3 rotatedPosition = MultiplyVector3(cframe.position);

            const Vec4 &A = rotation;
            const Vec4 &B = cframe.rotation;

            const Vec4 combinedRotation = {
                A.W * B.X + A.X * B.W + A.Y * B.Z - A.Z * B.Y,
                A.W * B.Y - A.X * B.Z + A.Y * B.W + A.Z * B.X,
                A.W * B.Z + A.X * B.Y - A.Y * B.X + A.Z * B.W,
                A.W * B.W - A.X * B.X - A.Y * B.Y - A.Z * B.Z};

            return {position + rotatedPosition, combinedRotation};
        }

        CFrame &operator*=(const CFrame &cframe) noexcept {
            *this = *this * cframe;
            return *this;
        }

        Vec3 operator*(const Vec3 &vector) const noexcept {
            return PointToWorldSpace(vector);
        }

        CFrame Inverse() const noexcept {
            const Vec4 inverseRotation = {-rotation.X, -rotation.Y, -rotation.Z,
                                          rotation.W};

            return {RotateVector(inverseRotation, -position), inverseRotation};
        }

        Vec3 MultiplyVector3(const Vec3 vector) const noexcept {
            return RotateVector(rotation, vector);
        }

        CFrame Lerp(const CFrame &cframe, const float time) const noexcept {
            Vec3 interpolatedPosition = position.Lerp(cframe.position, time);

            Vec4 targetRotation = cframe.rotation;

            float dot = rotation.Dot(targetRotation);

            if (dot < 0.0f) {
                targetRotation = -targetRotation;
                dot = -dot;
            }

            Vec4 interpolatedRotation;

            if (dot > 0.9995f) {
                interpolatedRotation =
                    rotation.Lerp(targetRotation, time).Normalized();
            } else {
                const float theta = std::acos(dot);
                const float sinTheta = std::sin(theta);

                const float weightA =
                    std::sin((1.0f - time) * theta) / sinTheta;
                const float weightB = std::sin(time * theta) / sinTheta;

                interpolatedRotation =
                    rotation * weightA + targetRotation * weightB;
            }

            return {interpolatedPosition, interpolatedRotation};
        }

        Vec3 PointToWorldSpace(const Vec3 point) const noexcept {
            return position + MultiplyVector3(point);
        }

        Vec3 PointToObjectSpace(const Vec3 point) const noexcept {
            return Inverse().MultiplyVector3(point);
        }

        Vec3 VectorToWorldSpace(const Vec3 vector) const noexcept {
            return MultiplyVector3(vector);
        }

        Vec3 VectorToObjectSpace(const Vec3 vector) const noexcept {
            return Inverse().MultiplyVector3(vector);
        }

        CFrame ToWorldSpace(const CFrame &cframe) const noexcept {
            return *this * cframe;
        }

        CFrame ToObjectSpace(const CFrame &cframe) const noexcept {
            return Inverse() * cframe;
        }

        String ToString() const {
            StringStream stream;

            stream << std::fixed << std::setprecision(3) << position.X << ", "
                   << position.Y << ", " << position.Z << ", " << rotation.X
                   << ", " << rotation.Y << ", " << rotation.Z << ", "
                   << rotation.W;

            return stream.str();
        }

    private:
        static Vec3 RotateVector(const Vec4 &quaternion,
                                 const Vec3 &vector) noexcept {
            const Vec3 q = {quaternion.X, quaternion.Y, quaternion.Z};

            const Vec3 uv = q.Cross(vector);
            const Vec3 uuv = q.Cross(uv);

            return vector + ((uv * quaternion.W) + uuv) * 2.0f;
        }
    };

    inline const CFrame CFrame::Identity = {{0.0f, 0.0f, 0.0f},
                                            {0.0f, 0.0f, 0.0f, 1.0f}};

    Sal_Stringify_Prebuilt(CFrame);
}
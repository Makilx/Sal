#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Math.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    struct Vector4;
    struct IVector4;

    struct Vector4 {
        float X = 0.0f, Y = 0.0f, Z = 0.0f, W = 0.0f;

        // Constants
        static const Vector4 Zero;
        static const Vector4 One;
        static const Vector4 XAxis;
        static const Vector4 YAxis;
        static const Vector4 ZAxis;
        static const Vector4 WAxis;

        // Constructors
        constexpr Vector4() = default;
        constexpr Vector4(float value)
            : X(value), Y(value), Z(value), W(value) {}
        constexpr Vector4(float X, float Y, float Z, float W)
            : X(X), Y(Y), Z(Z), W(W) {}

        // Operators
        constexpr Vector4 operator-() const { return {-X, -Y, -Z, -W}; }
        constexpr Vector4 operator+(const Vector4 &vector) const {
            return {X + vector.X, Y + vector.Y, Z + vector.Z, W + vector.W};
        }
        constexpr Vector4 operator-(const Vector4 &vector) const {
            return {X - vector.X, Y - vector.Y, Z - vector.Z, W - vector.W};
        }
        constexpr Vector4 operator*(const Vector4 &vector) const {
            return {X * vector.X, Y * vector.Y, Z * vector.Z, W * vector.W};
        }
        constexpr Vector4 operator/(const Vector4 &vector) const {
            return {X / vector.X, Y / vector.Y, Z / vector.Z, W / vector.W};
        }
        constexpr Vector4 &operator+=(const Vector4 &vector) {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            W += vector.W;
            return *this;
        }
        constexpr Vector4 &operator-=(const Vector4 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            W -= vector.W;
            return *this;
        }
        constexpr Vector4 &operator*=(const Vector4 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            Z *= vector.Z;
            W *= vector.W;
            return *this;
        }
        constexpr Vector4 &operator/=(const Vector4 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            Z /= vector.Z;
            W /= vector.W;
            return *this;
        }
        constexpr Vector4 operator+(float value) const {
            return {X + value, Y + value, Z + value, W + value};
        }
        constexpr Vector4 operator-(float value) const {
            return {X - value, Y - value, Z - value, W - value};
        }
        constexpr Vector4 operator*(float value) const {
            return {X * value, Y * value, Z * value, W * value};
        }
        constexpr Vector4 operator/(float value) const {
            return {X / value, Y / value, Z / value, W / value};
        }
        constexpr Vector4 &operator+=(float value) {
            X += value;
            Y += value;
            Z += value;
            W += value;
            return *this;
        }
        constexpr Vector4 &operator-=(float value) {
            X -= value;
            Y -= value;
            Z -= value;
            W -= value;
            return *this;
        }
        constexpr Vector4 &operator*=(float value) {
            X *= value;
            Y *= value;
            Z *= value;
            W *= value;
            return *this;
        }
        constexpr Vector4 &operator/=(float value) {
            X /= value;
            Y /= value;
            Z /= value;
            W /= value;
            return *this;
        }
        constexpr bool operator==(const Vector4 &vector) const {
            return X == vector.X && Y == vector.Y && Z == vector.Z &&
                   W == vector.W;
        }
        constexpr bool operator!=(const Vector4 &vector) const {
            return X != vector.X || Y != vector.Y || Z != vector.Z ||
                   W != vector.W;
        }
        constexpr bool operator>=(const Vector4 &vector) const {
            return X >= vector.X && Y >= vector.Y && Z >= vector.Z &&
                   W >= vector.W;
        }
        constexpr bool operator<=(const Vector4 &vector) const {
            return X <= vector.X && Y <= vector.Y && Z <= vector.Z &&
                   W <= vector.W;
        }
        constexpr bool operator>(const Vector4 &vector) const {
            return X > vector.X && Y > vector.Y && Z > vector.Z && W > vector.W;
        }
        constexpr bool operator<(const Vector4 &vector) const {
            return X < vector.X && Y < vector.Y && Z < vector.Z && W < vector.W;
        }
        constexpr bool operator==(float magnitude) const {
            return Magnitude() == magnitude;
        }
        constexpr bool operator!=(float magnitude) const {
            return Magnitude() != magnitude;
        }
        constexpr bool operator>=(float magnitude) const {
            return Magnitude() >= magnitude;
        }
        constexpr bool operator<=(float magnitude) const {
            return Magnitude() <= magnitude;
        }
        constexpr bool operator>(float magnitude) const {
            return Magnitude() > magnitude;
        }
        constexpr bool operator<(float magnitude) const {
            return Magnitude() < magnitude;
        }

        // Methods
        constexpr float MagnitudeSquared() const {
            return X * X + Y * Y + Z * Z + W * W;
        }

        float Magnitude() const { return Sqrt(MagnitudeSquared()); }

        IVector4 ToInteger() const;

        constexpr Vector4 Lerp(const Vector4 &vector, float time) const {
            return {sal::Lerp(X, vector.X, time), sal::Lerp(Y, vector.Y, time),
                    sal::Lerp(Z, vector.Z, time), sal::Lerp(W, vector.W, time)};
        }

        Vector4 Normalized() const {
            const float magnitude = Magnitude();

            if (magnitude <= EPSILON) {
                return Zero;
            }

            return *this / magnitude;
        }

        constexpr float Dot(const Vector4 &vector) const {
            return X * vector.X + Y * vector.Y + Z * vector.Z + W * vector.W;
        }

        constexpr float DistanceSquared(const Vector4 &vector) const {
            const float dx = X - vector.X;
            const float dy = Y - vector.Y;
            const float dz = Z - vector.Z;
            const float dw = W - vector.W;
            return dx * dx + dy * dy + dz * dz + dw * dw;
        }

        float Distance(const Vector4 &vector) const {
            return Sqrt(DistanceSquared(vector));
        }

        constexpr float Angle(const Vector4 &vector) const {
            const float denominator = Magnitude() * vector.Magnitude();

            if (denominator <= EPSILON) {
                return 0.0f;
            }

            return std::acos(Clamp(Dot(vector) / denominator, -1.0f, 1.0f));
        }

        constexpr Vector4 Reflect(const Vector4 &normal) const {
            return *this - normal * (2.0f * Dot(normal));
        }

        constexpr Vector4 Project(const Vector4 &normal) const {
            const float magnitude = normal.MagnitudeSquared();

            if (magnitude <= EPSILON) {
                return Zero;
            }

            return normal * (Dot(normal) / magnitude);
        }

        // ToString
        String ToString() const {
            StringStream stream;
            stream << std::fixed << std::setprecision(3) << X << ", " << Y
                   << ", " << Z << ", " << W;
            return stream.str();
        }
    };

    constexpr Vector4 operator+(float scale, const Vector4 &vector) {
        return Vector4(scale + vector.X, scale + vector.Y, scale + vector.Z,
                       scale + vector.W);
    }
    constexpr Vector4 operator-(float scale, const Vector4 &vector) {
        return Vector4(scale - vector.X, scale - vector.Y, scale - vector.Z,
                       scale - vector.W);
    }
    constexpr Vector4 operator*(float scale, const Vector4 &vector) {
        return Vector4(scale * vector.X, scale * vector.Y, scale * vector.Z,
                       scale * vector.W);
    }
    constexpr Vector4 operator/(float scale, const Vector4 &vector) {
        return Vector4(scale / vector.X, scale / vector.Y, scale / vector.Z,
                       scale / vector.W);
    }

    struct IVector4 {
        int X = 0, Y = 0, Z = 0, W = 0;

        // Constants
        static const IVector4 Zero;
        static const IVector4 One;
        static const IVector4 XAxis;
        static const IVector4 YAxis;
        static const IVector4 ZAxis;
        static const IVector4 WAxis;

        // Constructors
        constexpr IVector4() = default;
        constexpr IVector4(int value)
            : X(value), Y(value), Z(value), W(value) {}
        constexpr IVector4(int X, int Y, int Z, int W)
            : X(X), Y(Y), Z(Z), W(W) {}

        // Operators
        constexpr IVector4 operator-() const { return {-X, -Y, -Z, -W}; }
        constexpr IVector4 operator+(const IVector4 &vector) const {
            return {X + vector.X, Y + vector.Y, Z + vector.Z, W + vector.W};
        }
        constexpr IVector4 operator-(const IVector4 &vector) const {
            return {X - vector.X, Y - vector.Y, Z - vector.Z, W - vector.W};
        }
        constexpr IVector4 operator*(const IVector4 &vector) const {
            return {X * vector.X, Y * vector.Y, Z * vector.Z, W * vector.W};
        }
        constexpr IVector4 operator/(const IVector4 &vector) const {
            return {X / vector.X, Y / vector.Y, Z / vector.Z, W / vector.W};
        }
        constexpr IVector4 &operator+=(const IVector4 &vector) {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            W += vector.W;
            return *this;
        }
        constexpr IVector4 &operator-=(const IVector4 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            W -= vector.W;
            return *this;
        }
        constexpr IVector4 &operator*=(const IVector4 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            Z *= vector.Z;
            W *= vector.W;
            return *this;
        }
        constexpr IVector4 &operator/=(const IVector4 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            Z /= vector.Z;
            W /= vector.W;
            return *this;
        }
        constexpr IVector4 operator+(int value) const {
            return {X + value, Y + value, Z + value, W + value};
        }
        constexpr IVector4 operator-(int value) const {
            return {X - value, Y - value, Z - value, W - value};
        }
        constexpr IVector4 operator*(int value) const {
            return {X * value, Y * value, Z * value, W * value};
        }
        constexpr IVector4 operator/(int value) const {
            return {X / value, Y / value, Z / value, W / value};
        }
        constexpr IVector4 &operator+=(int value) {
            X += value;
            Y += value;
            Z += value;
            W += value;
            return *this;
        }
        constexpr IVector4 &operator-=(int value) {
            X -= value;
            Y -= value;
            Z -= value;
            W -= value;
            return *this;
        }
        constexpr IVector4 &operator*=(int value) {
            X *= value;
            Y *= value;
            Z *= value;
            W *= value;
            return *this;
        }
        constexpr IVector4 &operator/=(int value) {
            X /= value;
            Y /= value;
            Z /= value;
            W /= value;
            return *this;
        }
        constexpr bool operator==(const IVector4 &vector) const {
            return X == vector.X && Y == vector.Y && Z == vector.Z &&
                   W == vector.W;
        }
        constexpr bool operator!=(const IVector4 &vector) const {
            return X != vector.X || Y != vector.Y || Z != vector.Z ||
                   W != vector.W;
        }
        constexpr bool operator>=(const IVector4 &vector) const {
            return X >= vector.X && Y >= vector.Y && Z >= vector.Z &&
                   W >= vector.W;
        }
        constexpr bool operator<=(const IVector4 &vector) const {
            return X <= vector.X && Y <= vector.Y && Z <= vector.Z &&
                   W <= vector.W;
        }
        constexpr bool operator>(const IVector4 &vector) const {
            return X > vector.X && Y > vector.Y && Z > vector.Z && W > vector.W;
        }
        constexpr bool operator<(const IVector4 &vector) const {
            return X < vector.X && Y < vector.Y && Z < vector.Z && W < vector.W;
        }
        constexpr bool operator==(int magnitude) const {
            return Magnitude() == magnitude;
        }
        constexpr bool operator!=(int magnitude) const {
            return Magnitude() != magnitude;
        }
        constexpr bool operator>=(int magnitude) const {
            return Magnitude() >= magnitude;
        }
        constexpr bool operator<=(int magnitude) const {
            return Magnitude() <= magnitude;
        }
        constexpr bool operator>(int magnitude) const {
            return Magnitude() > magnitude;
        }
        constexpr bool operator<(int magnitude) const {
            return Magnitude() < magnitude;
        }

        // Methods
        constexpr int MagnitudeSquared() const {
            return X * X + Y * Y + Z * Z + W * W;
        }

        int Magnitude() const { return Sqrt(MagnitudeSquared()); }

        Vector4 ToFloat() const;

        constexpr IVector4 Lerp(const IVector4 &vector, float time) const {
            return {static_cast<int>(sal::Lerp(X, vector.X, time)),
                    static_cast<int>(sal::Lerp(Y, vector.Y, time)),
                    static_cast<int>(sal::Lerp(Z, vector.Z, time)),
                    static_cast<int>(sal::Lerp(W, vector.W, time))};
        }

        constexpr int Dot(const IVector4 &vector) const {
            return X * vector.X + Y * vector.Y + Z * vector.Z + W * vector.W;
        }

        constexpr int DistanceSquared(const IVector4 &vector) const {
            const int dx = X - vector.X;
            const int dy = Y - vector.Y;
            const int dz = Z - vector.Z;
            const int dw = W - vector.W;
            return dx * dx + dy * dy + dz * dz + dw * dw;
        }

        int Distance(const IVector4 &vector) const {
            return Sqrt(DistanceSquared(vector));
        }

        // ToString
        String ToString() const {
            StringStream stream;
            stream << std::fixed << std::setprecision(3) << X << ", " << Y
                   << ", " << Z << ", " << W;
            return stream.str();
        }
    };

    constexpr IVector4 operator+(int scale, const IVector4 &vector) {
        return IVector4(scale + vector.X, scale + vector.Y, scale + vector.Z,
                        scale + vector.W);
    }
    constexpr IVector4 operator-(int scale, const IVector4 &vector) {
        return IVector4(scale - vector.X, scale - vector.Y, scale - vector.Z,
                        scale - vector.W);
    }
    constexpr IVector4 operator*(int scale, const IVector4 &vector) {
        return IVector4(scale * vector.X, scale * vector.Y, scale * vector.Z,
                        scale * vector.W);
    }
    constexpr IVector4 operator/(int scale, const IVector4 &vector) {
        return IVector4(scale / vector.X, scale / vector.Y, scale / vector.Z,
                        scale / vector.W);
    }

    using Vec4 = Vector4;
    using IVec4 = IVector4;

    Sal_Stringify_Prebuilt(Vector4);
    Sal_Stringify_Prebuilt(IVector4);

    inline IVector4 Vector4::ToInteger() const { return IVector4(X, Y, Z, W); }
    inline Vector4 IVector4::ToFloat() const { return Vector4(X, Y, Z, W); }

    inline const Vector4 Vector4::Zero = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    inline const Vector4 Vector4::One = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    inline const Vector4 Vector4::XAxis = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    inline const Vector4 Vector4::YAxis = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    inline const Vector4 Vector4::ZAxis = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    inline const Vector4 Vector4::WAxis = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

    inline const IVector4 IVector4::Zero = IVector4(0, 0, 0, 0);
    inline const IVector4 IVector4::One = IVector4(1, 1, 1, 1);
    inline const IVector4 IVector4::XAxis = IVector4(1, 0, 0, 0);
    inline const IVector4 IVector4::YAxis = IVector4(0, 1, 0, 0);
    inline const IVector4 IVector4::ZAxis = IVector4(0, 0, 1, 0);
    inline const IVector4 IVector4::WAxis = IVector4(0, 0, 0, 1);
}
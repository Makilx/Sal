#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Math.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    struct Vector2;
    struct IVector2;

    struct Vector2 {
        float X = 0.0f, Y = 0.0f;

        // Constants
        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 XAxis;
        static const Vector2 YAxis;

        // Constructors
        constexpr Vector2() = default;
        constexpr Vector2(float value) : X(value), Y(value) {}
        constexpr Vector2(float X, float Y) : X(X), Y(Y) {}

        // Operators
        constexpr Vector2 operator-() const { return {-X, -Y}; }
        constexpr Vector2 operator+(const Vector2 &vector) const {
            return {X + vector.X, Y + vector.Y};
        }
        constexpr Vector2 operator-(const Vector2 &vector) const {
            return {X - vector.X, Y - vector.Y};
        }
        constexpr Vector2 operator*(const Vector2 &vector) const {
            return {X * vector.X, Y * vector.Y};
        }
        constexpr Vector2 operator/(const Vector2 &vector) const {
            return {X / vector.X, Y / vector.Y};
        }
        constexpr Vector2 &operator+=(const Vector2 &vector) {
            X += vector.X;
            Y += vector.Y;
            return *this;
        }
        constexpr Vector2 &operator-=(const Vector2 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            return *this;
        }
        constexpr Vector2 &operator*=(const Vector2 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            return *this;
        }
        constexpr Vector2 &operator/=(const Vector2 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            return *this;
        }
        constexpr Vector2 operator+(float value) const {
            return {X + value, Y + value};
        }
        constexpr Vector2 operator-(float value) const {
            return {X - value, Y - value};
        }
        constexpr Vector2 operator*(float value) const {
            return {X * value, Y * value};
        }
        constexpr Vector2 operator/(float value) const {
            return {X / value, Y / value};
        }
        constexpr Vector2 &operator+=(float value) {
            X += value;
            Y += value;
            return *this;
        }
        constexpr Vector2 &operator-=(float value) {
            X -= value;
            Y -= value;
            return *this;
        }
        constexpr Vector2 &operator*=(float value) {
            X *= value;
            Y *= value;
            return *this;
        }
        constexpr Vector2 &operator/=(float value) {
            X /= value;
            Y /= value;
            return *this;
        }
        constexpr bool operator==(const Vector2 &vector) const {
            return X == vector.X && Y == vector.Y;
        }
        constexpr bool operator!=(const Vector2 &vector) const {
            return X != vector.X || Y != vector.Y;
        }
        constexpr bool operator>=(const Vector2 &vector) const {
            return X >= vector.X && Y >= vector.Y;
        }
        constexpr bool operator<=(const Vector2 &vector) const {
            return X <= vector.X && Y <= vector.Y;
        }
        constexpr bool operator>(const Vector2 &vector) const {
            return X > vector.X && Y > vector.Y;
        }
        constexpr bool operator<(const Vector2 &vector) const {
            return X < vector.X && Y < vector.Y;
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
        constexpr float MagnitudeSquared() const { return X * X + Y * Y; }

        float Magnitude() const { return Sqrt(MagnitudeSquared()); }

        IVector2 ToInteger() const;

        constexpr Vector2 Lerp(const Vector2 &vector, float time) const {
            return {sal::Lerp(X, vector.X, time), sal::Lerp(Y, vector.Y, time)};
        }

        Vector2 Normalized() const {
            const float magnitude = Magnitude();

            if (magnitude <= EPSILON) {
                return Zero;
            }

            return *this / magnitude;
        }

        constexpr float Dot(const Vector2 &vector) const {
            return X * vector.X + Y * vector.Y;
        }

        // ToString
        String ToString() const {
            StringStream stream;
            stream << std::fixed << std::setprecision(3) << X << ", " << Y;
            return stream.str();
        }
    };

    constexpr Vector2 operator+(float scale, const Vector2 &vector) {
        return Vector2(scale + vector.X, scale + vector.Y);
    }
    constexpr Vector2 operator-(float scale, const Vector2 &vector) {
        return Vector2(scale - vector.X, scale - vector.Y);
    }
    constexpr Vector2 operator*(float scale, const Vector2 &vector) {
        return Vector2(scale * vector.X, scale * vector.Y);
    }
    constexpr Vector2 operator/(float scale, const Vector2 &vector) {
        return Vector2(scale / vector.X, scale / vector.Y);
    }

    struct IVector2 {
        int X = 0, Y = 0;

        // Constants
        static const IVector2 Zero;
        static const IVector2 One;
        static const IVector2 XAxis;
        static const IVector2 YAxis;

        // Constructors
        constexpr IVector2() = default;
        constexpr IVector2(int value) : X(value), Y(value) {}
        constexpr IVector2(int X, int Y) : X(X), Y(Y) {}

        // Operators
        constexpr IVector2 operator-() const { return {-X, -Y}; }
        constexpr IVector2 operator+(const IVector2 &vector) const {
            return {X + vector.X, Y + vector.Y};
        }
        constexpr IVector2 operator-(const IVector2 &vector) const {
            return {X - vector.X, Y - vector.Y};
        }
        constexpr IVector2 operator*(const IVector2 &vector) const {
            return {X * vector.X, Y * vector.Y};
        }
        constexpr IVector2 operator/(const IVector2 &vector) const {
            return {X / vector.X, Y / vector.Y};
        }
        constexpr IVector2 &operator+=(const IVector2 &vector) {
            X += vector.X;
            Y += vector.Y;
            return *this;
        }
        constexpr IVector2 &operator-=(const IVector2 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            return *this;
        }
        constexpr IVector2 &operator*=(const IVector2 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            return *this;
        }
        constexpr IVector2 &operator/=(const IVector2 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            return *this;
        }
        constexpr IVector2 operator+(int value) const {
            return {X + value, Y + value};
        }
        constexpr IVector2 operator-(int value) const {
            return {X - value, Y - value};
        }
        constexpr IVector2 operator*(int value) const {
            return {X * value, Y * value};
        }
        constexpr IVector2 operator/(int value) const {
            return {X / value, Y / value};
        }
        constexpr IVector2 &operator+=(int value) {
            X += value;
            Y += value;
            return *this;
        }
        constexpr IVector2 &operator-=(int value) {
            X -= value;
            Y -= value;
            return *this;
        }
        constexpr IVector2 &operator*=(int value) {
            X *= value;
            Y *= value;
            return *this;
        }
        constexpr IVector2 &operator/=(int value) {
            X /= value;
            Y /= value;
            return *this;
        }
        constexpr bool operator==(const IVector2 &vector) const {
            return X == vector.X && Y == vector.Y;
        }
        constexpr bool operator!=(const IVector2 &vector) const {
            return X != vector.X || Y != vector.Y;
        }
        constexpr bool operator>=(const IVector2 &vector) const {
            return X >= vector.X && Y >= vector.Y;
        }
        constexpr bool operator<=(const IVector2 &vector) const {
            return X <= vector.X && Y <= vector.Y;
        }
        constexpr bool operator>(const IVector2 &vector) const {
            return X > vector.X && Y > vector.Y;
        }
        constexpr bool operator<(const IVector2 &vector) const {
            return X < vector.X && Y < vector.Y;
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
        constexpr int MagnitudeSquared() const { return X * X + Y * Y; }

        int Magnitude() const { return Sqrt(MagnitudeSquared()); }

        Vector2 ToFloat() const;

        constexpr IVector2 Lerp(const IVector2 &vector, float time) const {
            return {sal::Lerp(X, vector.X, time), sal::Lerp(Y, vector.Y, time)};
        }

        constexpr int Dot(const IVector2 &vector) const {
            return X * vector.X + Y * vector.Y;
        }

        // ToString
        String ToString() const {
            StringStream stream;
            stream << std::fixed << std::setprecision(3) << X << ", " << Y;
            return stream.str();
        }
    };

    constexpr IVector2 operator+(int scale, const IVector2 &vector) {
        return IVector2(scale + vector.X, scale + vector.Y);
    }
    constexpr IVector2 operator-(int scale, const IVector2 &vector) {
        return IVector2(scale - vector.X, scale - vector.Y);
    }
    constexpr IVector2 operator*(int scale, const IVector2 &vector) {
        return IVector2(scale * vector.X, scale * vector.Y);
    }
    constexpr IVector2 operator/(int scale, const IVector2 &vector) {
        return IVector2(scale / vector.X, scale / vector.Y);
    }

    using Vec2 = Vector2;
    using IVec2 = IVector2;

    Sal_Stringify_Prebuilt(Vector2);
    Sal_Stringify_Prebuilt(IVector2);

    inline IVector2 Vector2::ToInteger() const { return IVector2(X, Y); }
    inline Vector2 IVector2::ToFloat() const { return Vector2(X, Y); }

    inline const Vector2 Vector2::Zero = Vector2(0.0f, 0.0f);
    inline const Vector2 Vector2::One = Vector2(1.0f, 1.0f);
    inline const Vector2 Vector2::XAxis = Vector2(1.0f, 0.0f);
    inline const Vector2 Vector2::YAxis = Vector2(0.0f, 1.0f);

    inline const IVector2 IVector2::Zero = IVector2(0, 0);
    inline const IVector2 IVector2::One = IVector2(1, 1);
    inline const IVector2 IVector2::XAxis = IVector2(1, 0);
    inline const IVector2 IVector2::YAxis = IVector2(0, 1);
}
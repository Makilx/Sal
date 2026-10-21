#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Math.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    struct Vector3;
    struct IVector3;

    struct Vector3 {
        float X = 0.0f, Y = 0.0f, Z = 0.0f;

        // Constants
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 XAxis;
        static const Vector3 YAxis;
        static const Vector3 ZAxis;

        // Constructors
        constexpr Vector3() = default;
        constexpr Vector3(float value) : X(value), Y(value), Z(value) {}
        constexpr Vector3(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}

        // Operators
        constexpr Vector3 operator-() const { return {-X, -Y, -Z}; }
        constexpr Vector3 operator+(const Vector3 &vector) const {
            return {X + vector.X, Y + vector.Y, Z + vector.Z};
        }
        constexpr Vector3 operator-(const Vector3 &vector) const {
            return {X - vector.X, Y - vector.Y, Z - vector.Z};
        }
        constexpr Vector3 operator*(const Vector3 &vector) const {
            return {X * vector.X, Y * vector.Y, Z * vector.Z};
        }
        constexpr Vector3 operator/(const Vector3 &vector) const {
            return {X / vector.X, Y / vector.Y, Z / vector.Z};
        }
        constexpr Vector3 &operator+=(const Vector3 &vector) {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            return *this;
        }
        constexpr Vector3 &operator-=(const Vector3 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            return *this;
        }
        constexpr Vector3 &operator*=(const Vector3 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            Z *= vector.Z;
            return *this;
        }
        constexpr Vector3 &operator/=(const Vector3 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            Z /= vector.Z;
            return *this;
        }
        constexpr Vector3 operator+(float value) const {
            return {X + value, Y + value, Z + value};
        }
        constexpr Vector3 operator-(float value) const {
            return {X - value, Y - value, Z - value};
        }
        constexpr Vector3 operator*(float value) const {
            return {X * value, Y * value, Z * value};
        }
        constexpr Vector3 operator/(float value) const {
            return {X / value, Y / value, Z / value};
        }
        constexpr Vector3 &operator+=(float value) {
            X += value;
            Y += value;
            Z += value;
            return *this;
        }
        constexpr Vector3 &operator-=(float value) {
            X -= value;
            Y -= value;
            Z -= value;
            return *this;
        }
        constexpr Vector3 &operator*=(float value) {
            X *= value;
            Y *= value;
            Z *= value;
            return *this;
        }
        constexpr Vector3 &operator/=(float value) {
            X /= value;
            Y /= value;
            Z /= value;
            return *this;
        }
        constexpr bool operator==(const Vector3 &vector) const {
            return X == vector.X && Y == vector.Y && Z == vector.Z;
        }
        constexpr bool operator!=(const Vector3 &vector) const {
            return X != vector.X || Y != vector.Y || Z != vector.Z;
        }
        constexpr bool operator>=(const Vector3 &vector) const {
            return X >= vector.X && Y >= vector.Y && Z >= vector.Z;
        }
        constexpr bool operator<=(const Vector3 &vector) const {
            return X <= vector.X && Y <= vector.Y && Z <= vector.Z;
        }
        constexpr bool operator>(const Vector3 &vector) const {
            return X > vector.X && Y > vector.Y && Z > vector.Z;
        }
        constexpr bool operator<(const Vector3 &vector) const {
            return X < vector.X && Y < vector.Y && Z < vector.Z;
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
            return X * X + Y * Y + Z * Z;
        }

        float Magnitude() const { return sal::Sqrt(MagnitudeSquared()); }

        IVector3 ToInteger() const;

        constexpr Vector3 Lerp(const Vector3 &vector, float time) const {
            return {sal::Lerp(X, vector.X, time), sal::Lerp(Y, vector.Y, time),
                    sal::Lerp(Z, vector.Z, time)};
        }

        Vector3 Normalized() const {
            const float magnitude = Magnitude();

            if (magnitude <= EPSILON) {
                return Zero;
            }

            return *this / magnitude;
        }

        constexpr float Dot(const Vector3 &vector) const {
            return X * vector.X + Y * vector.Y + Z * vector.Z;
        }

        constexpr Vector3 Cross(const Vector3 &vector) const {
            return {Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z,
                    X * vector.Y - Y * vector.X};
        }

        constexpr float DistanceSquared(const Vector3 &vector) const {
            const float dx = X - vector.X;
            const float dy = Y - vector.Y;
            const float dz = Z - vector.Z;
            return dx * dx + dy * dy + dz * dz;
        }

        float Distance(const Vector3 &vector) const {
            return Sqrt(DistanceSquared(vector));
        }

        constexpr float Angle(const Vector3 &vector) const {
            const float denominator = Magnitude() * vector.Magnitude();

            if (denominator <= EPSILON) {
                return 0.0f;
            }

            return std::acos(Clamp(Dot(vector) / denominator, -1.0f, 1.0f));
        }

        constexpr Vector3 Reflect(const Vector3 &normal) const {
            return *this - normal * (2.0f * Dot(normal));
        }

        constexpr Vector3 Project(const Vector3 &normal) const {
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
                   << ", " << Z;
            return stream.str();
        }
    };

    constexpr Vector3 operator+(float scale, const Vector3 &vector) {
        return Vector3(scale + vector.X, scale + vector.Y, scale + vector.Z);
    }
    constexpr Vector3 operator-(float scale, const Vector3 &vector) {
        return Vector3(scale - vector.X, scale - vector.Y, scale - vector.Z);
    }
    constexpr Vector3 operator*(float scale, const Vector3 &vector) {
        return Vector3(scale * vector.X, scale * vector.Y, scale * vector.Z);
    }
    constexpr Vector3 operator/(float scale, const Vector3 &vector) {
        return Vector3(scale / vector.X, scale / vector.Y, scale / vector.Z);
    }

    struct IVector3 {
        int X = 0, Y = 0, Z = 0;

        // Constants
        static const IVector3 Zero;
        static const IVector3 One;
        static const IVector3 XAxis;
        static const IVector3 YAxis;
        static const IVector3 ZAxis;

        // Constructors
        constexpr IVector3() = default;
        constexpr IVector3(int value) : X(value), Y(value), Z(value) {}
        constexpr IVector3(int X, int Y, int Z) : X(X), Y(Y), Z(Z) {}

        // Operators
        constexpr IVector3 operator-() const { return {-X, -Y, -Z}; }
        constexpr IVector3 operator+(const IVector3 &vector) const {
            return {X + vector.X, Y + vector.Y, Z + vector.Z};
        }
        constexpr IVector3 operator-(const IVector3 &vector) const {
            return {X - vector.X, Y - vector.Y, Z - vector.Z};
        }
        constexpr IVector3 operator*(const IVector3 &vector) const {
            return {X * vector.X, Y * vector.Y, Z * vector.Z};
        }
        constexpr IVector3 operator/(const IVector3 &vector) const {
            return {X / vector.X, Y / vector.Y, Z / vector.Z};
        }
        constexpr IVector3 &operator+=(const IVector3 &vector) {
            X += vector.X;
            Y += vector.Y;
            Z += vector.Z;
            return *this;
        }
        constexpr IVector3 &operator-=(const IVector3 &vector) {
            X -= vector.X;
            Y -= vector.Y;
            Z -= vector.Z;
            return *this;
        }
        constexpr IVector3 &operator*=(const IVector3 &vector) {
            X *= vector.X;
            Y *= vector.Y;
            Z *= vector.Z;
            return *this;
        }
        constexpr IVector3 &operator/=(const IVector3 &vector) {
            X /= vector.X;
            Y /= vector.Y;
            Z /= vector.Z;
            return *this;
        }
        constexpr IVector3 operator+(int value) const {
            return {X + value, Y + value, Z + value};
        }
        constexpr IVector3 operator-(int value) const {
            return {X - value, Y - value, Z - value};
        }
        constexpr IVector3 operator*(int value) const {
            return {X * value, Y * value, Z * value};
        }
        constexpr IVector3 operator/(int value) const {
            return {X / value, Y / value, Z / value};
        }
        constexpr IVector3 &operator+=(int value) {
            X += value;
            Y += value;
            Z += value;
            return *this;
        }
        constexpr IVector3 &operator-=(int value) {
            X -= value;
            Y -= value;
            Z -= value;
            return *this;
        }
        constexpr IVector3 &operator*=(int value) {
            X *= value;
            Y *= value;
            Z *= value;
            return *this;
        }
        constexpr IVector3 &operator/=(int value) {
            X /= value;
            Y /= value;
            Z /= value;
            return *this;
        }
        constexpr bool operator==(const IVector3 &vector) const {
            return X == vector.X && Y == vector.Y && Z == vector.Z;
        }
        constexpr bool operator!=(const IVector3 &vector) const {
            return X != vector.X || Y != vector.Y || Z != vector.Z;
        }
        constexpr bool operator>=(const IVector3 &vector) const {
            return X >= vector.X && Y >= vector.Y && Z >= vector.Z;
        }
        constexpr bool operator<=(const IVector3 &vector) const {
            return X <= vector.X && Y <= vector.Y && Z <= vector.Z;
        }
        constexpr bool operator>(const IVector3 &vector) const {
            return X > vector.X && Y > vector.Y && Z > vector.Z;
        }
        constexpr bool operator<(const IVector3 &vector) const {
            return X < vector.X && Y < vector.Y && Z < vector.Z;
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
        constexpr int MagnitudeSquared() const { return X * X + Y * Y + Z * Z; }

        int Magnitude() const { return Sqrt(MagnitudeSquared()); }

        Vector3 ToFloat() const;

        constexpr IVector3 Lerp(const IVector3 &vector, float time) const {
            return {static_cast<int>(sal::Lerp(X, vector.X, time)),
                    static_cast<int>(sal::Lerp(Y, vector.Y, time)),
                    static_cast<int>(sal::Lerp(Z, vector.Z, time))};
        }

        constexpr int Dot(const IVector3 &vector) const {
            return X * vector.X + Y * vector.Y + Z * vector.Z;
        }

        constexpr IVector3 Cross(const IVector3 &vector) const {
            return {Y * vector.Z - Z * vector.Y, Z * vector.X - X * vector.Z,
                    X * vector.Y - Y * vector.X};
        }

        constexpr int DistanceSquared(const IVector3 &vector) const {
            const int dx = X - vector.X;
            const int dy = Y - vector.Y;
            const int dz = Z - vector.Z;
            return dx * dx + dy * dy + dz * dz;
        }

        int Distance(const IVector3 &vector) const {
            return Sqrt(DistanceSquared(vector));
        }

        // ToString
        String ToString() const {
            StringStream stream;
            stream << std::fixed << std::setprecision(3) << X << ", " << Y
                   << ", " << Z;
            return stream.str();
        }
    };

    constexpr IVector3 operator+(int scale, const IVector3 &vector) {
        return IVector3(scale + vector.X, scale + vector.Y, scale + vector.Z);
    }
    constexpr IVector3 operator-(int scale, const IVector3 &vector) {
        return IVector3(scale - vector.X, scale - vector.Y, scale - vector.Z);
    }
    constexpr IVector3 operator*(int scale, const IVector3 &vector) {
        return IVector3(scale * vector.X, scale * vector.Y, scale * vector.Z);
    }
    constexpr IVector3 operator/(int scale, const IVector3 &vector) {
        return IVector3(scale / vector.X, scale / vector.Y, scale / vector.Z);
    }

    using Vec3 = Vector3;
    using IVec3 = IVector3;

    Sal_Stringify_Prebuilt(Vector3);
    Sal_Stringify_Prebuilt(IVector3);

    inline IVector3 Vector3::ToInteger() const { return IVector3(X, Y, Z); }
    inline Vector3 IVector3::ToFloat() const { return Vector3(X, Y, Z); }

    inline const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
    inline const Vector3 Vector3::XAxis = Vector3(1.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::YAxis = Vector3(0.0f, 1.0f, 0.0f);
    inline const Vector3 Vector3::ZAxis = Vector3(0.0f, 0.0f, 1.0f);

    inline const IVector3 IVector3::Zero = IVector3(0, 0, 0);
    inline const IVector3 IVector3::One = IVector3(1, 1, 1);
    inline const IVector3 IVector3::XAxis = IVector3(1, 0, 0);
    inline const IVector3 IVector3::YAxis = IVector3(0, 1, 0);
    inline const IVector3 IVector3::ZAxis = IVector3(0, 0, 1);
}
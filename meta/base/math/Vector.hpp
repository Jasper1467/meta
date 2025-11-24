#pragma once

#include <cmath>
#include <iostream>
#include <meta/base/core/Platform.hpp>
#include <meta/base/math/Constants.hpp>

namespace meta::Math
{
    // Simple constexpr sqrt for pre-C++26
    META_INLINE constexpr double constexpr_sqrt(double x, double eps = 1e-10)
    {
        double guess = x / 2.0;
        double last_guess = 0.0;
        while (std::abs(guess - last_guess) > eps)
        {
            last_guess = guess;
            guess = 0.5 * (guess + x / guess);
        }
        return guess;
    }

    template <typename T> struct Vector2D
    {
        T x{};
        T y{};

        META_INLINE constexpr Vector2D() noexcept = default;
        META_INLINE constexpr Vector2D(T x_, T y_) noexcept : x(x_), y(y_)
        {
        }

        // Static helpers
        META_INLINE static constexpr Vector2D zero() noexcept
        {
            return { T(0), T(0) };
        }
        META_INLINE static constexpr Vector2D one() noexcept
        {
            return { T(1), T(1) };
        }

        // Arithmetic
        META_INLINE constexpr Vector2D operator+(const Vector2D& rhs) const noexcept
        {
            return { x + rhs.x, y + rhs.y };
        }
        META_INLINE constexpr Vector2D operator-(const Vector2D& rhs) const noexcept
        {
            return { x - rhs.x, y - rhs.y };
        }
        META_INLINE constexpr Vector2D operator*(T scalar) const noexcept
        {
            return { x * scalar, y * scalar };
        }
        META_INLINE constexpr Vector2D operator/(T scalar) const noexcept
        {
            return { x / scalar, y / scalar };
        }

        META_INLINE constexpr Vector2D& operator+=(const Vector2D& rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }
        META_INLINE constexpr Vector2D& operator-=(const Vector2D& rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }
        META_INLINE constexpr Vector2D& operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }
        META_INLINE constexpr Vector2D& operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        // Magnitude
        META_NODISCARD constexpr T lengthSquared() const noexcept
        {
            return x * x + y * y;
        }
        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        // Distance helpers
        META_NODISCARD T distanceSquared(const Vector2D& rhs) const noexcept
        {
            return (rhs - *this).lengthSquared();
        }
        META_NODISCARD T distance(const Vector2D& rhs) const noexcept
        {
            return std::sqrt(distanceSquared(rhs));
        }

        META_NODISCARD constexpr T dot(const Vector2D& rhs) const noexcept
        {
            return x * rhs.x + y * rhs.y;
        }

        // Normalize
        META_NODISCARD Vector2D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        // Clamp
        META_INLINE Vector2D clamp(T minVal, T maxVal) const noexcept
        {
            return { std::fmax(minVal, std::fmin(maxVal, x)), std::fmax(minVal, std::fmin(maxVal, y)) };
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2D& v)
        {
            return os << "(" << v.x << ", " << v.y << ")";
        }
    };

    template <typename T> struct Vector3D
    {
        T x{};
        T y{};
        T z{};

        META_INLINE constexpr Vector3D() noexcept = default;
        META_INLINE constexpr Vector3D(T x_, T y_, T z_) noexcept : x(x_), y(y_), z(z_)
        {
        }

        META_INLINE static constexpr Vector3D zero() noexcept
        {
            return { T(0), T(0), T(0) };
        }
        META_INLINE static constexpr Vector3D one() noexcept
        {
            return { T(1), T(1), T(1) };
        }

        META_INLINE constexpr Vector3D operator+(const Vector3D& rhs) const noexcept
        {
            return { x + rhs.x, y + rhs.y, z + rhs.z };
        }
        META_INLINE constexpr Vector3D operator-(const Vector3D& rhs) const noexcept
        {
            return { x - rhs.x, y - rhs.y, z - rhs.z };
        }
        META_INLINE constexpr Vector3D operator*(T scalar) const noexcept
        {
            return { x * scalar, y * scalar, z * scalar };
        }
        META_INLINE constexpr Vector3D operator/(T scalar) const noexcept
        {
            return { x / scalar, y / scalar, z / scalar };
        }

        META_INLINE constexpr Vector3D& operator+=(const Vector3D& rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }
        META_INLINE constexpr Vector3D& operator-=(const Vector3D& rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }
        META_INLINE constexpr Vector3D& operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }
        META_INLINE constexpr Vector3D& operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        META_NODISCARD constexpr T lengthSquared() const noexcept
        {
            return x * x + y * y + z * z;
        }
        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        META_NODISCARD constexpr T dot(const Vector3D& rhs) const noexcept
        {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }
        META_NODISCARD Vector3D cross(const Vector3D& rhs) const noexcept
        {
            return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
        }

        META_NODISCARD T distanceSquared(const Vector3D& rhs) const noexcept
        {
            return (*this - rhs).lengthSquared();
        }
        META_NODISCARD T distance(const Vector3D& rhs) const noexcept
        {
            return std::sqrt(distanceSquared(rhs));
        }

        META_NODISCARD Vector3D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        META_INLINE Vector3D clamp(T minVal, T maxVal) const noexcept
        {
            return { std::fmax(minVal, std::fmin(maxVal, x)), std::fmax(minVal, std::fmin(maxVal, y)),
                     std::fmax(minVal, std::fmin(maxVal, z)) };
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector3D& v)
        {
            return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        }
    };

    template <typename T> struct Vector4D
    {
        T x{};
        T y{};
        T z{};
        T w{};

        META_INLINE constexpr Vector4D() noexcept = default;
        META_INLINE constexpr Vector4D(T x_, T y_, T z_, T w_) noexcept : x(x_), y(y_), z(z_), w(w_)
        {
        }

        META_INLINE static constexpr Vector4D zero() noexcept
        {
            return { T(0), T(0), T(0), T(0) };
        }
        META_INLINE static constexpr Vector4D one() noexcept
        {
            return { T(1), T(1), T(1), T(1) };
        }

        META_INLINE constexpr Vector4D operator+(const Vector4D& rhs) const noexcept
        {
            return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
        }
        META_INLINE constexpr Vector4D operator-(const Vector4D& rhs) const noexcept
        {
            return { x - rhs.x, y - rhs.y, z - rhs.z, w - wrhs.w };
        }
        META_INLINE constexpr Vector4D operator*(T scalar) const noexcept
        {
            return { x * scalar, y * scalar, z * scalar, w * scalar };
        }
        META_INLINE constexpr Vector4D operator/(T scalar) const noexcept
        {
            return { x / scalar, y / scalar, z / scalar, w / scalar };
        }

        META_INLINE constexpr Vector4D& operator+=(const Vector4D& rhs) noexcept
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }
        META_INLINE constexpr Vector4D& operator-=(const Vector4D& rhs) noexcept
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }
        META_INLINE constexpr Vector4D& operator*=(T scalar) noexcept
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }
        META_INLINE constexpr Vector4D& operator/=(T scalar) noexcept
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        META_NODISCARD constexpr T lengthSquared() const noexcept
        {
            return x * x + y * y + z * z + w * w;
        }
        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        META_NODISCARD constexpr T dot(const Vector4D& rhs) const noexcept
        {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }

        META_NODISCARD Vector4D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        META_INLINE Vector4D clamp(T minVal, T maxVal) const noexcept
        {
            return { std::fmax(minVal, std::fmin(maxVal, x)), std::fmax(minVal, std::fmin(maxVal, y)),
                     std::fmax(minVal, std::fmin(maxVal, z)), std::fmax(minVal, std::fmin(maxVal, w)) };
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector4D& v)
        {
            return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        }
    };
} // namespace meta::Math

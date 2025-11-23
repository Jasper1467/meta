#pragma once
#include <cmath>
#include <iostream>
#include <meta/core/Platform.hpp>
#include <meta/math/Constants.hpp>

namespace meta::Math
{
    // Simple constexpr sqrt approximation for pre-C++26
    constexpr double constexpr_sqrt(double x, double eps = 1e-10)
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

    template <typename T>
    struct Vector2D
    {
        T x{};
        T y{};

        constexpr Vector2D() noexcept = default;
        constexpr Vector2D(T x_, T y_) noexcept : x(x_), y(y_) {}

        constexpr Vector2D operator+(const Vector2D& rhs) const noexcept { return {x + rhs.x, y + rhs.y}; }
        constexpr Vector2D operator-(const Vector2D& rhs) const noexcept { return {x - rhs.x, y - rhs.y}; }
        constexpr Vector2D operator*(T scalar) const noexcept { return {x * scalar, y * scalar}; }
        constexpr Vector2D operator/(T scalar) const noexcept { return {x / scalar, y / scalar}; }

        constexpr Vector2D& operator+=(const Vector2D& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
        constexpr Vector2D& operator-=(const Vector2D& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
        constexpr Vector2D& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; return *this; }
        constexpr Vector2D& operator/=(T scalar) noexcept { x /= scalar; y /= scalar; return *this; }

        META_NODISCARD constexpr T lengthSquared() const noexcept { return x * x + y * y; }

        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        META_NODISCARD constexpr T dot(const Vector2D& rhs) const noexcept { return x * rhs.x + y * rhs.y; }

        Vector2D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector2D& v)
        {
            return os << "(" << v.x << ", " << v.y << ")";
        }
    };

    template <typename T>
    struct Vector3D
    {
        T x{};
        T y{};
        T z{};

        constexpr Vector3D() noexcept = default;
        constexpr Vector3D(T x_, T y_, T z_) noexcept : x(x_), y(y_), z(z_) {}

        constexpr Vector3D operator+(const Vector3D& rhs) const noexcept { return {x + rhs.x, y + rhs.y, z + rhs.z}; }
        constexpr Vector3D operator-(const Vector3D& rhs) const noexcept { return {x - rhs.x, y - rhs.y, z - rhs.z}; }
        constexpr Vector3D operator*(T scalar) const noexcept { return {x * scalar, y * scalar, z * scalar}; }
        constexpr Vector3D operator/(T scalar) const noexcept { return {x / scalar, y / scalar, z / scalar}; }

        constexpr Vector3D& operator+=(const Vector3D& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
        constexpr Vector3D& operator-=(const Vector3D& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
        constexpr Vector3D& operator*=(T scalar) noexcept { x *= scalar; y *= scalar; z *= scalar; return *this; }
        constexpr Vector3D& operator/=(T scalar) noexcept { x /= scalar; y /= scalar; z /= scalar; return *this; }

        META_NODISCARD constexpr T lengthSquared() const noexcept { return x*x + y*y + z*z; }
        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        META_NODISCARD constexpr T dot(const Vector3D& rhs) const noexcept { return x*rhs.x + y*rhs.y + z*rhs.z; }

        Vector3D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector3D& v)
        {
            return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        }
    };

    template <typename T>
    struct Vector4D
    {
        T x{};
        T y{};
        T z{};
        T w{};

        constexpr Vector4D() noexcept = default;
        constexpr Vector4D(T x_, T y_, T z_, T w_) noexcept : x(x_), y(y_), z(z_), w(w_) {}

        constexpr Vector4D operator+(const Vector4D& rhs) const noexcept { return {x+rhs.x, y+rhs.y, z+rhs.z, w+rhs.w}; }
        constexpr Vector4D operator-(const Vector4D& rhs) const noexcept { return {x-rhs.x, y-rhs.y, z-rhs.z, w-rhs.w}; }
        constexpr Vector4D operator*(T scalar) const noexcept { return {x*scalar, y*scalar, z*scalar, w*scalar}; }
        constexpr Vector4D operator/(T scalar) const noexcept { return {x/scalar, y/scalar, z/scalar, w/scalar}; }

        constexpr Vector4D& operator+=(const Vector4D& rhs) noexcept { x+=rhs.x; y+=rhs.y; z+=rhs.z; w+=rhs.w; return *this; }
        constexpr Vector4D& operator-=(const Vector4D& rhs) noexcept { x-=rhs.x; y-=rhs.y; z-=rhs.z; w-=rhs.w; return *this; }
        constexpr Vector4D& operator*=(T scalar) noexcept { x*=scalar; y*=scalar; z*=scalar; w*=scalar; return *this; }
        constexpr Vector4D& operator/=(T scalar) noexcept { x/=scalar; y/=scalar; z/=scalar; w/=scalar; return *this; }

        META_NODISCARD constexpr T lengthSquared() const noexcept { return x*x + y*y + z*z + w*w; }
        META_NODISCARD T length() const noexcept
        {
#if __cpp_lib_constexpr_math >= 201907L
            return std::sqrt(lengthSquared());
#else
            return constexpr_sqrt(lengthSquared());
#endif
        }

        META_NODISCARD constexpr T dot(const Vector4D& rhs) const noexcept { return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w; }

        Vector4D normalized() const noexcept
        {
            T len = length();
            return len != T(0) ? (*this / len) : *this;
        }

        friend std::ostream& operator<<(std::ostream& os, const Vector4D& v)
        {
            return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        }
    };
} // namespace meta::Math

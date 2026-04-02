#pragma once

#include <cmath>
#include <ostream>

namespace Xangine {

template<typename T = float>
struct Vector2 {
    union {
        struct { T x, y; };
        T data[2];
    };
    
    // Constructors
    constexpr Vector2() : x(0), y(0) {}
    constexpr explicit Vector2(T value) : x(value), y(value) {}
    constexpr Vector2(T x, T y) : x(x), y(y) {}
    
    // Array access
    constexpr T& operator[](size_t index) { return data[index]; }
    constexpr const T& operator[](size_t index) const { return data[index]; }
    
    // Arithmetic assignment
    constexpr Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    constexpr Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    constexpr Vector2& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    constexpr Vector2& operator/=(T scalar) {
        T inv = T(1) / scalar;
        x *= inv;
        y *= inv;
        return *this;
    }
    
    // Unary operators
    constexpr Vector2 operator-() const {
        return Vector2(-x, -y);
    }
    
    // Comparison
    constexpr bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
    
    constexpr bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
    
    // Vector operations
    constexpr T dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
    
    constexpr T cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }
    
    T length() const {
        return std::sqrt(x * x + y * y);
    }
    
    constexpr T lengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2 normalized() const {
        T len = length();
        if (len > T(0)) {
            T invLen = T(1) / len;
            return Vector2(x * invLen, y * invLen);
        }
        return Vector2(T(0));
    }
    
    void normalize() {
        *this = normalized();
    }
    
    // Utility
    constexpr T minComponent() const {
        return x < y ? x : y;
    }
    
    constexpr T maxComponent() const {
        return x > y ? x : y;
    }
    
    // Common vectors
    static constexpr Vector2 zero() { return Vector2(0, 0); }
    static constexpr Vector2 one() { return Vector2(1, 1); }
    static constexpr Vector2 up() { return Vector2(0, 1); }
    static constexpr Vector2 down() { return Vector2(0, -1); }
    static constexpr Vector2 left() { return Vector2(-1, 0); }
    static constexpr Vector2 right() { return Vector2(1, 0); }
};

// Binary arithmetic operators
template<typename T>
constexpr Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) {
    return Vector2<T>(a.x + b.x, a.y + b.y);
}

template<typename T>
constexpr Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) {
    return Vector2<T>(a.x - b.x, a.y - b.y);
}

template<typename T>
constexpr Vector2<T> operator*(const Vector2<T>& vec, T scalar) {
    return Vector2<T>(vec.x * scalar, vec.y * scalar);
}

template<typename T>
constexpr Vector2<T> operator*(T scalar, const Vector2<T>& vec) {
    return Vector2<T>(vec.x * scalar, vec.y * scalar);
}

template<typename T>
constexpr Vector2<T> operator/(const Vector2<T>& vec, T scalar) {
    T inv = T(1) / scalar;
    return Vector2<T>(vec.x * inv, vec.y * inv);
}

// Output stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

// Common type aliases
using Vec2 = Vector2<float>;
using Vec2d = Vector2<double>;
using Vec2i = Vector2<int>;
using Vec2u = Vector2<unsigned int>;

} // namespace Xangine
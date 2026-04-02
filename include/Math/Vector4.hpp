#pragma once

#include <cmath>
#include <ostream>

namespace Xangine {

template<typename T = float>
struct Vector4 {
    union {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };  // For color (RGBA)
        T data[4];
    };
    
    // Constructors
    constexpr Vector4() : x(0), y(0), z(0), w(0) {}
    constexpr explicit Vector4(T value) : x(value), y(value), z(value), w(value) {}
    constexpr Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    constexpr Vector4(const Vector3<T>& xyz, T w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    constexpr Vector4(const Vector2<T>& xy, T z, T w) : x(xy.x), y(xy.y), z(z), w(w) {}
    
    // Array access
    constexpr T& operator[](size_t index) { return data[index]; }
    constexpr const T& operator[](size_t index) const { return data[index]; }
    
    // Arithmetic assignment
    constexpr Vector4& operator+=(const Vector4& other) {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }
    
    constexpr Vector4& operator-=(const Vector4& other) {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }
    
    constexpr Vector4& operator*=(T scalar) {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }
    
    constexpr Vector4& operator/=(T scalar) {
        T inv = T(1) / scalar;
        x *= inv; y *= inv; z *= inv; w *= inv;
        return *this;
    }
    
    // Unary operators
    constexpr Vector4 operator-() const {
        return Vector4(-x, -y, -z, -w);
    }
    
    // Comparison
    constexpr bool operator==(const Vector4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    
    constexpr bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }
    
    // Vector operations
    constexpr T dot(const Vector4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    T length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    constexpr T lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    Vector4 normalized() const {
        T len = length();
        if (len > T(0)) {
            T invLen = T(1) / len;
            return Vector4(x * invLen, y * invLen, z * invLen, w * invLen);
        }
        return Vector4(T(0));
    }
    
    void normalize() {
        *this = normalized();
    }
    
    // For homogeneous coordinates (3D graphics)
    Vector3<T> toVector3() const {
        if (w != T(0)) {
            T invW = T(1) / w;
            return Vector3<T>(x * invW, y * invW, z * invW);
        }
        return Vector3<T>(x, y, z);
    }
    
    // Utility
    constexpr T minComponent() const {
        return std::min({x, y, z, w});
    }
    
    constexpr T maxComponent() const {
        return std::max({x, y, z, w});
    }
    
    // Common vectors
    static constexpr Vector4 zero() { return Vector4(0, 0, 0, 0); }
    static constexpr Vector4 one() { return Vector4(1, 1, 1, 1); }
    static constexpr Vector4 identity() { return Vector4(1, 0, 0, 1); }  // For 2D transformations
};

// Binary arithmetic operators
template<typename T>
constexpr Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b) {
    return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template<typename T>
constexpr Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b) {
    return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template<typename T>
constexpr Vector4<T> operator*(const Vector4<T>& vec, T scalar) {
    return Vector4<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
}

template<typename T>
constexpr Vector4<T> operator*(T scalar, const Vector4<T>& vec) {
    return Vector4<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
}

template<typename T>
constexpr Vector4<T> operator/(const Vector4<T>& vec, T scalar) {
    T inv = T(1) / scalar;
    return Vector4<T>(vec.x * inv, vec.y * inv, vec.z * inv, vec.w * inv);
}

// Output stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector4<T>& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

// Common type aliases
using Vec4 = Vector4<float>;
using Vec4d = Vector4<double>;
using Vec4i = Vector4<int>;
using Vec4u = Vector4<unsigned int>;

} // namespace Xangine
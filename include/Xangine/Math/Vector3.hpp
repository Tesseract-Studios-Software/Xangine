#pragma once

#include <cmath>
#include <ostream>

namespace Xangine {

template<typename T = float>
struct Vector3 {
    union {
        struct { T x, y, z; };
        struct { T r, g, b; };  // For color representation
        T data[3];
    };
    
    // Constructors
    constexpr Vector3() : x(0), y(0), z(0) {}
    constexpr explicit Vector3(T value) : x(value), y(value), z(value) {}
    constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
    constexpr Vector3(const Vector2<T>& xy, T z) : x(xy.x), y(xy.y), z(z) {}
    
    // Array access
    constexpr T& operator[](size_t index) { return data[index]; }
    constexpr const T& operator[](size_t index) const { return data[index]; }
    
    // Arithmetic assignment
    constexpr Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    constexpr Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    constexpr Vector3& operator*=(T scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    constexpr Vector3& operator/=(T scalar) {
        T inv = T(1) / scalar;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }
    
    // Unary operators
    constexpr Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }
    
    // Comparison
    constexpr bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    constexpr bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
    
    // Vector operations
    constexpr T dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    constexpr Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    T length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    constexpr T lengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    Vector3 normalized() const {
        T len = length();
        if (len > T(0)) {
            T invLen = T(1) / len;
            return Vector3(x * invLen, y * invLen, z * invLen);
        }
        return Vector3(T(0));
    }
    
    void normalize() {
        *this = normalized();
    }
    
    // Angle between vectors (in radians)
    T angleBetween(const Vector3& other) const {
        T cosAngle = dot(other) / (length() * other.length());
        cosAngle = std::max(T(-1), std::min(T(1), cosAngle));  // Clamp to [-1, 1]
        return std::acos(cosAngle);
    }
    
    // Project onto another vector
    Vector3 projectOnto(const Vector3& onto) const {
        T ontoLenSq = onto.lengthSquared();
        if (ontoLenSq == T(0)) return Vector3(T(0));
        T scalar = dot(onto) / ontoLenSq;
        return onto * scalar;
    }
    
    // Reflect around normal (for physics)
    Vector3 reflect(const Vector3& normal) const {
        return *this - normal * (T(2) * dot(normal));
    }
    
    // Linear interpolation
    Vector3 lerp(const Vector3& to, T t) const {
        return Vector3(
            x + (to.x - x) * t,
            y + (to.y - y) * t,
            z + (to.z - z) * t
        );
    }
    
    // Utility
    constexpr T minComponent() const {
        return std::min({x, y, z});
    }
    
    constexpr T maxComponent() const {
        return std::max({x, y, z});
    }
    
    // Component-wise absolute value
    Vector3 abs() const {
        return Vector3(
            x < T(0) ? -x : x,
            y < T(0) ? -y : y,
            z < T(0) ? -z : z
        );
    }
    
    // Common vectors
    static constexpr Vector3 zero() { return Vector3(0, 0, 0); }
    static constexpr Vector3 one() { return Vector3(1, 1, 1); }
    static constexpr Vector3 up() { return Vector3(0, 1, 0); }
    static constexpr Vector3 down() { return Vector3(0, -1, 0); }
    static constexpr Vector3 left() { return Vector3(-1, 0, 0); }
    static constexpr Vector3 right() { return Vector3(1, 0, 0); }
    static constexpr Vector3 forward() { return Vector3(0, 0, -1); }
    static constexpr Vector3 back() { return Vector3(0, 0, 1); }
};

// Binary arithmetic operators
template<typename T>
constexpr Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b) {
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T>
constexpr Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b) {
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T>
constexpr Vector3<T> operator*(const Vector3<T>& vec, T scalar) {
    return Vector3<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

template<typename T>
constexpr Vector3<T> operator*(T scalar, const Vector3<T>& vec) {
    return Vector3<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

template<typename T>
constexpr Vector3<T> operator/(const Vector3<T>& vec, T scalar) {
    T inv = T(1) / scalar;
    return Vector3<T>(vec.x * inv, vec.y * inv, vec.z * inv);
}

// Component-wise multiplication (Hadamard product)
template<typename T>
constexpr Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b) {
    return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

// Output stream
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

// Common type aliases
using Vec3 = Vector3<float>;
using Vec3d = Vector3<double>;
using Vec3i = Vector3<int>;
using Vec3u = Vector3<unsigned int>;

} // namespace Xangine
#pragma once

#include "Vector3.hpp"
#include "Matrix4.hpp"
#include <cmath>

namespace Xangine {

template<typename T = float>
struct Quaternion {
    T x, y, z, w;  // (x, y, z) is vector part, w is scalar part
    
    // Constructors
    constexpr Quaternion() : x(0), y(0), z(0), w(1) {}
    constexpr Quaternion(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Quaternion(const Vector3<T>& axis, T angle) {
        T halfAngle = angle / T(2);
        T s = std::sin(halfAngle);
        Vector3<T> normAxis = axis.normalized();
        x = normAxis.x * s;
        y = normAxis.y * s;
        z = normAxis.z * s;
        w = std::cos(halfAngle);
    }
    
    // Identity quaternion
    static constexpr Quaternion identity() {
        return Quaternion(0, 0, 0, 1);
    }
    
    // From Euler angles (in radians, order: pitch, yaw, roll)
    static Quaternion fromEuler(const Vector3<T>& euler) {
        T cx = std::cos(euler.x * T(0.5));
        T sx = std::sin(euler.x * T(0.5));
        T cy = std::cos(euler.y * T(0.5));
        T sy = std::sin(euler.y * T(0.5));
        T cz = std::cos(euler.z * T(0.5));
        T sz = std::sin(euler.z * T(0.5));
        
        return Quaternion(
            sx * cy * cz + cx * sy * sz,
            cx * sy * cz - sx * cy * sz,
            cx * cy * sz + sx * sy * cz,
            cx * cy * cz - sx * sy * sz
        );
    }
    
    // Convert to matrix
    Matrix4<T> toMatrix() const {
        Matrix4<T> m = Matrix4<T>::identity();
        
        T xx = x * x;
        T yy = y * y;
        T zz = z * z;
        T xy = x * y;
        T xz = x * z;
        T yz = y * z;
        T wx = w * x;
        T wy = w * y;
        T wz = w * z;
        
        m.data[0] = T(1) - T(2) * (yy + zz);
        m.data[4] = T(2) * (xy - wz);
        m.data[8] = T(2) * (xz + wy);
        
        m.data[1] = T(2) * (xy + wz);
        m.data[5] = T(1) - T(2) * (xx + zz);
        m.data[9] = T(2) * (yz - wx);
        
        m.data[2] = T(2) * (xz - wy);
        m.data[6] = T(2) * (yz + wx);
        m.data[10] = T(1) - T(2) * (xx + yy);
        
        return m;
    }
    
    // Quaternion multiplication
    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y + y * other.w + z * other.x - x * other.z,
            w * other.z + z * other.w + x * other.y - y * other.x,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }
    
    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }
    
    // Rotate a vector
    Vector3<T> rotate(const Vector3<T>& vec) const {
        Quaternion p(vec.x, vec.y, vec.z, 0);
        Quaternion rotated = (*this) * p * conjugate();
        return Vector3<T>(rotated.x, rotated.y, rotated.z);
    }
    
    // Conjugate (inverse for unit quaternions)
    Quaternion conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }
    
    // Length
    T length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    T lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    // Normalize
    void normalize() {
        T len = length();
        if (len > T(0)) {
            T invLen = T(1) / len;
            x *= invLen;
            y *= invLen;
            z *= invLen;
            w *= invLen;
        }
    }
    
    Quaternion normalized() const {
        Quaternion result = *this;
        result.normalize();
        return result;
    }
    
    // Spherical linear interpolation (smooth rotation)
    static Quaternion slerp(const Quaternion& a, const Quaternion& b, T t) {
        T dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        
        Quaternion b2 = b;
        if (dot < T(0)) {
            b2 = Quaternion(-b.x, -b.y, -b.z, -b.w);
            dot = -dot;
        }
        
        if (dot > T(0.9995)) {
            // Linear interpolation for near-parallel
            Quaternion result = Quaternion(
                a.x + t * (b2.x - a.x),
                a.y + t * (b2.y - a.y),
                a.z + t * (b2.z - a.z),
                a.w + t * (b2.w - a.w)
            );
            result.normalize();
            return result;
        }
        
        T theta = std::acos(dot);
        T sinTheta = std::sin(theta);
        T scaleA = std::sin((T(1) - t) * theta) / sinTheta;
        T scaleB = std::sin(t * theta) / sinTheta;
        
        return Quaternion(
            scaleA * a.x + scaleB * b2.x,
            scaleA * a.y + scaleB * b2.y,
            scaleA * a.z + scaleB * b2.z,
            scaleA * a.w + scaleB * b2.w
        );
    }
    
    // Get Euler angles (in radians, order: pitch, yaw, roll)
    Vector3<T> toEuler() const {
        Vector3<T> euler;
        
        // Pitch (x-axis rotation)
        T sinp = T(2) * (w * x - y * z);
        if (std::abs(sinp) >= T(1)) {
            euler.x = std::copysign(M_PI / T(2), sinp);
        } else {
            euler.x = std::asin(sinp);
        }
        
        // Yaw (y-axis rotation)
        if (std::abs(w) > T(0.0001)) {
            euler.y = std::atan2(T(2) * (w * y + x * z), T(1) - T(2) * (x * x + y * y));
        } else {
            euler.y = T(0);
        }
        
        // Roll (z-axis rotation)
        euler.z = std::atan2(T(2) * (w * z + x * y), T(1) - T(2) * (y * y + z * z));
        
        return euler;
    }
};

// Common type alias
using Quat = Quaternion<float>;

} // namespace Xangine
#pragma once

#include "Vector3.hpp"
#include "Vector4.hpp"

namespace Xangine {

template<typename T = float>
struct Matrix4 {
    T data[16];  // Column-major order (for OpenGL/Vulkan)
    
    // Constructors
    constexpr Matrix4() {
        for (int i = 0; i < 16; ++i) data[i] = T(0);
    }
    
    constexpr Matrix4(T diagonal) {
        for (int i = 0; i < 16; ++i) data[i] = T(0);
        data[0] = data[5] = data[10] = data[15] = diagonal;
    }
    
    constexpr Matrix4(const T values[16]) {
        for (int i = 0; i < 16; ++i) data[i] = values[i];
    }
    
    // Access operators (column-major)
    constexpr T& operator()(int col, int row) { return data[col * 4 + row]; }
    constexpr const T& operator()(int col, int row) const { return data[col * 4 + row]; }
    
    // Column access
    Vector4<T> getColumn(int col) const {
        return Vector4<T>(data[col * 4], data[col * 4 + 1], data[col * 4 + 2], data[col * 4 + 3]);
    }
    
    void setColumn(int col, const Vector4<T>& values) {
        data[col * 4] = values.x;
        data[col * 4 + 1] = values.y;
        data[col * 4 + 2] = values.z;
        data[col * 4 + 3] = values.w;
    }
    
    // Row access (slower, for convenience)
    Vector4<T> getRow(int row) const {
        return Vector4<T>(data[row], data[4 + row], data[8 + row], data[12 + row]);
    }
    
    // Static factory methods
    static constexpr Matrix4 identity() {
        Matrix4 m;
        m.data[0] = m.data[5] = m.data[10] = m.data[15] = T(1);
        return m;
    }
    
    static Matrix4 translation(const Vector3<T>& translation) {
        Matrix4 m = identity();
        m.data[12] = translation.x;
        m.data[13] = translation.y;
        m.data[14] = translation.z;
        return m;
    }
    
    static Matrix4 scale(const Vector3<T>& scale) {
        Matrix4 m = identity();
        m.data[0] = scale.x;
        m.data[5] = scale.y;
        m.data[10] = scale.z;
        return m;
    }
    
    static Matrix4 rotationX(T angle) {
        T c = std::cos(angle);
        T s = std::sin(angle);
        Matrix4 m = identity();
        m.data[5] = c;
        m.data[6] = s;
        m.data[9] = -s;
        m.data[10] = c;
        return m;
    }
    
    static Matrix4 rotationY(T angle) {
        T c = std::cos(angle);
        T s = std::sin(angle);
        Matrix4 m = identity();
        m.data[0] = c;
        m.data[2] = -s;
        m.data[8] = s;
        m.data[10] = c;
        return m;
    }
    
    static Matrix4 rotationZ(T angle) {
        T c = std::cos(angle);
        T s = std::sin(angle);
        Matrix4 m = identity();
        m.data[0] = c;
        m.data[1] = s;
        m.data[4] = -s;
        m.data[5] = c;
        return m;
    }
    
    static Matrix4 rotationAxis(const Vector3<T>& axis, T angle) {
        T c = std::cos(angle);
        T s = std::sin(angle);
        T t = T(1) - c;
        Vector3<T> normAxis = axis.normalized();
        T x = normAxis.x;
        T y = normAxis.y;
        T z = normAxis.z;
        
        Matrix4 m = identity();
        m.data[0] = t * x * x + c;
        m.data[1] = t * x * y + s * z;
        m.data[2] = t * x * z - s * y;
        m.data[4] = t * x * y - s * z;
        m.data[5] = t * y * y + c;
        m.data[6] = t * y * z + s * x;
        m.data[8] = t * x * z + s * y;
        m.data[9] = t * y * z - s * x;
        m.data[10] = t * z * z + c;
        return m;
    }
    
    static Matrix4 lookAt(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up) {
        Vector3<T> forward = (target - eye).normalized();
        Vector3<T> right = forward.cross(up.normalized()).normalized();
        Vector3<T> trueUp = right.cross(forward);
        
        Matrix4 m = identity();
        m.data[0] = right.x;
        m.data[4] = right.y;
        m.data[8] = right.z;
        m.data[1] = trueUp.x;
        m.data[5] = trueUp.y;
        m.data[9] = trueUp.z;
        m.data[2] = -forward.x;
        m.data[6] = -forward.y;
        m.data[10] = -forward.z;
        m.data[12] = -right.dot(eye);
        m.data[13] = -trueUp.dot(eye);
        m.data[14] = forward.dot(eye);
        return m;
    }
    
    static Matrix4 perspective(T fovRadians, T aspect, T nearPlane, T farPlane) {
        T tanHalfFov = std::tan(fovRadians / T(2));
        Matrix4 m = identity();
        m.data[0] = T(1) / (aspect * tanHalfFov);
        m.data[5] = T(1) / tanHalfFov;
        m.data[10] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        m.data[11] = -T(1);
        m.data[14] = -(T(2) * farPlane * nearPlane) / (farPlane - nearPlane);
        m.data[15] = T(0);
        return m;
    }
    
    static Matrix4 orthographic(T left, T right, T bottom, T top, T nearPlane, T farPlane) {
        Matrix4 m = identity();
        m.data[0] = T(2) / (right - left);
        m.data[5] = T(2) / (top - bottom);
        m.data[10] = -T(2) / (farPlane - nearPlane);
        m.data[12] = -(right + left) / (right - left);
        m.data[13] = -(top + bottom) / (top - bottom);
        m.data[14] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        return m;
    }
    
    // Matrix operations
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int col = 0; col < 4; ++col) {
            for (int row = 0; row < 4; ++row) {
                T sum = T(0);
                for (int k = 0; k < 4; ++k) {
                    sum += data[k * 4 + row] * other.data[col * 4 + k];
                }
                result.data[col * 4 + row] = sum;
            }
        }
        return result;
    }
    
    Matrix4& operator*=(const Matrix4& other) {
        *this = *this * other;
        return *this;
    }
    
    Vector3<T> transformPoint(const Vector3<T>& point) const {
        T invW = T(1) / (data[3] * point.x + data[7] * point.y + data[11] * point.z + data[15]);
        return Vector3<T>(
            (data[0] * point.x + data[4] * point.y + data[8] * point.z + data[12]) * invW,
            (data[1] * point.x + data[5] * point.y + data[9] * point.z + data[13]) * invW,
            (data[2] * point.x + data[6] * point.y + data[10] * point.z + data[14]) * invW
        );
    }
    
    Vector3<T> transformVector(const Vector3<T>& vector) const {
        return Vector3<T>(
            data[0] * vector.x + data[4] * vector.y + data[8] * vector.z,
            data[1] * vector.x + data[5] * vector.y + data[9] * vector.z,
            data[2] * vector.x + data[6] * vector.y + data[10] * vector.z
        );
    }
    
    Matrix4 transpose() const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.data[j * 4 + i] = data[i * 4 + j];
            }
        }
        return result;
    }
    
    T determinant() const {
        T a11 = data[0], a12 = data[4], a13 = data[8], a14 = data[12];
        T a21 = data[1], a22 = data[5], a23 = data[9], a24 = data[13];
        T a31 = data[2], a32 = data[6], a33 = data[10], a34 = data[14];
        T a41 = data[3], a42 = data[7], a43 = data[11], a44 = data[15];
        
        return a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42))
             - a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41))
             + a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41))
             - a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));
    }
    
    Matrix4 inverse() const {
        T det = determinant();
        if (det == T(0)) return identity();
        
        T invDet = T(1) / det;
        Matrix4 result;
        
        // Compute cofactors (simplified - full implementation would be longer)
        // For now, return transpose (not correct inverse, just placeholder)
        // A full 4x4 inverse would have 16 cofactor calculations
        return transpose();  // TODO: Implement full inverse
    }
    
    // Utility
    void print() const {
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                std::cout << data[col * 4 + row] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Common type alias
using Mat4 = Matrix4<float>;
using Mat4d = Matrix4<double>;

} // namespace Xangine
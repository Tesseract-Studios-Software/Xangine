// Math/Transform.hpp
#pragma once

#include "Vector3.hpp"
#include "Quaternion.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

namespace Xangine {

template<typename T = float>
class Transform {
public:
    Transform() 
        : m_position(Vector3<T>::zero())
        , m_rotation(Quaternion<T>::identity())
        , m_scale(Vector3<T>::one())
        , m_parent(nullptr)
        , m_dirty(true)
        , m_worldDirty(true)
        , m_gimbalLock(false)
        , m_yaw(0)
        , m_pitch(0)
        , m_roll(0) {}
    
    Transform(const Vector3<T>& position) 
        : m_position(position)
        , m_rotation(Quaternion<T>::identity())
        , m_scale(Vector3<T>::one())
        , m_parent(nullptr)
        , m_dirty(true)
        , m_worldDirty(true)
        , m_gimbalLock(false)
        , m_yaw(0)
        , m_pitch(0)
        , m_roll(0) {}
    
    Transform(const Quaternion<T>& rotation) 
        : m_position(Vector3<T>::zero())
        , m_rotation(rotation)
        , m_scale(Vector3<T>::one())
        , m_parent(nullptr)
        , m_dirty(true)
        , m_worldDirty(true)
        , m_gimbalLock(false)
        , m_yaw(0)
        , m_pitch(0)
        , m_roll(0) {}
    
    Transform(const Vector3<T>& position, const Quaternion<T>& rotation) 
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(Vector3<T>::one())
        , m_parent(nullptr)
        , m_dirty(true)
        , m_worldDirty(true)
        , m_gimbalLock(false)
        , m_yaw(0)
        , m_pitch(0)
        , m_roll(0) {}
    
    Transform(const Vector3<T>& position, const Quaternion<T>& rotation, const Vector3<T>& scale) 
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale)
        , m_parent(nullptr)
        , m_dirty(true)
        , m_worldDirty(true)
        , m_gimbalLock(false)
        , m_yaw(0)
        , m_pitch(0)
        , m_roll(0) {}
    
    const Vector3<T>& getPosition() const { return m_position; }
    void setPosition(const Vector3<T>& position) { 
        m_position = position; 
        markDirty();
    }
    
    const Quaternion<T>& getRotation() const { return m_rotation; }
    
    void setRotation(const Quaternion<T>& rotation) {
        m_rotation = rotation;
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            // toEuler returns (pitch, yaw, roll)
            m_pitch = euler.x;
            m_yaw = euler.y;
            m_roll = euler.z;
        }
        markDirty();
    }
    
    const Vector3<T>& getScale() const { return m_scale; }
    void setScale(const Vector3<T>& scale) {
        m_scale = scale;
        markDirty();
    }
    
    void setUniformScale(T scale) {
        m_scale = Vector3<T>(scale, scale, scale);
        markDirty();
    }
    
    void moveLocal(const Vector3<T>& localOffset) {
        Vector3<T> worldOffset = m_rotation.rotate(localOffset);
        m_position += worldOffset;
        markDirty();
    }
    
    void moveWorld(const Vector3<T>& worldOffset) {
        m_position += worldOffset;
        markDirty();
    }
    
    void moveForward(T distance) {
        moveLocal(Vector3<T>(0, 0, -distance));
    }
    
    void moveBackward(T distance) {
        moveLocal(Vector3<T>(0, 0, distance));
    }
    
    void moveRight(T distance) {
        moveLocal(Vector3<T>(distance, 0, 0));
    }
    
    void moveLeft(T distance) {
        moveLocal(Vector3<T>(-distance, 0, 0));
    }
    
    void moveUp(T distance) {
        moveLocal(Vector3<T>(0, distance, 0));
    }
    
    void moveDown(T distance) {
        moveLocal(Vector3<T>(0, -distance, 0));
    }
    
    void rotateLocal(const Vector3<T>& eulerAngles) {
        Quaternion<T> q = Quaternion<T>::fromEuler(eulerAngles);
        m_rotation = m_rotation * q;
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            m_yaw = euler.y;
            m_pitch = euler.x;
            m_roll = euler.z;
        }
        markDirty();
    }
    
    void rotateWorld(const Vector3<T>& eulerAngles) {
        Quaternion<T> q = Quaternion<T>::fromEuler(eulerAngles);
        m_rotation = q * m_rotation;
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            m_yaw = euler.y;
            m_pitch = euler.x;
            m_roll = euler.z;
        }
        markDirty();
    }
    
    void rotateLocalAxis(const Vector3<T>& axis, T angle) {
        Quaternion<T> q(axis, angle);
        m_rotation = m_rotation * q;
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            m_yaw = euler.y;
            m_pitch = euler.x;
            m_roll = euler.z;
        }
        markDirty();
    }
    
    void rotateWorldAxis(const Vector3<T>& axis, T angle) {
        Quaternion<T> q(axis, angle);
        m_rotation = q * m_rotation;
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            m_yaw = euler.y;
            m_pitch = euler.x;
            m_roll = euler.z;
        }
        markDirty();
    }

    void setRotationEuler(const Vector3<T>& eulerAngles) {
        // eulerAngles = (pitch, yaw, roll)
        m_pitch = eulerAngles.x;
        m_yaw = eulerAngles.y;
        m_roll = eulerAngles.z;
        updateRotationFromEuler();
    }
    
    void setGimbalLock(bool enabled) {
        m_gimbalLock = enabled;
        if (enabled) {
            Vector3<T> euler = m_rotation.toEuler();
            m_yaw = euler.y;
            m_pitch = euler.x;
            m_roll = euler.z;
        }
    }
    
    bool getGimbalLock() const {
        return m_gimbalLock;
    }
    
    void rotateYaw(T amount) {
        if (m_gimbalLock) {
            m_yaw += amount;
            updateRotationFromEuler();
        } else {
            rotateLocal(Vector3<T>(0, amount, 0));
        }
    }
    
    void rotatePitch(T amount) {
        if (m_gimbalLock) {
            m_pitch += amount;
            T halfPi = MathConstants<T>::HALF_PI;
            if (m_pitch > halfPi) m_pitch = halfPi;
            if (m_pitch < -halfPi) m_pitch = -halfPi;
            updateRotationFromEuler();
        } else {
            rotateLocal(Vector3<T>(amount, 0, 0));
        }
    }
    
    void rotateRoll(T amount) {
        if (m_gimbalLock) {
            m_roll += amount;
            updateRotationFromEuler();
        } else {
            rotateLocal(Vector3<T>(0, 0, amount));
        }
    }
    
    void setYawPitchRoll(T yaw, T pitch, T roll) {
        m_yaw = yaw;
        m_pitch = pitch;
        m_roll = roll;
        updateRotationFromEuler();
    }
    
    void getYawPitchRoll(T& yaw, T& pitch, T& roll) const {
        yaw = m_yaw;
        pitch = m_pitch;
        roll = m_roll;
    }
    
    Vector3<T> getForward() const {
        return m_rotation.rotate(Vector3<T>(0, 0, -1));
    }
    
    Vector3<T> getBackward() const {
        return m_rotation.rotate(Vector3<T>(0, 0, 1));
    }
    
    Vector3<T> getRight() const {
        return m_rotation.rotate(Vector3<T>(1, 0, 0));
    }
    
    Vector3<T> getLeft() const {
        return m_rotation.rotate(Vector3<T>(-1, 0, 0));
    }
    
    Vector3<T> getUp() const {
        return m_rotation.rotate(Vector3<T>(0, 1, 0));
    }
    
    Vector3<T> getDown() const {
        return m_rotation.rotate(Vector3<T>(0, -1, 0));
    }
    
    Vector3<T> localToWorld(const Vector3<T>& localPoint) const {
        return getWorldMatrix().transformPoint(localPoint);
    }
    
    Vector3<T> worldToLocal(const Vector3<T>& worldPoint) const {
        Mat4 worldMatrix = getWorldMatrix();
        Mat4 inverseMatrix = worldMatrix.inverse();
        return inverseMatrix.transformPoint(worldPoint);
    }
    
    Vector3<T> localToWorldDirection(const Vector3<T>& localDir) const {
        return m_rotation.rotate(localDir);
    }
    
    Vector3<T> worldToLocalDirection(const Vector3<T>& worldDir) const {
        return m_rotation.conjugate().rotate(worldDir);
    }
    
    Mat4 getLocalMatrix() const {
        return Mat4::translation(m_position) * 
               m_rotation.toMatrix() * 
               Mat4::scale(m_scale);
    }
    
    Mat4 getWorldMatrix() const {
        if (m_worldDirty) {
            if (m_parent) {
                m_cachedWorldMatrix = m_parent->getWorldMatrix() * getLocalMatrix();
            } else {
                m_cachedWorldMatrix = getLocalMatrix();
            }
            m_worldDirty = false;
        }
        return m_cachedWorldMatrix;
    }
    
    Vector3<T> getWorldPosition() const {
        return getWorldMatrix().transformPoint(Vector3<T>::zero());
    }
    
    Quaternion<T> getWorldRotation() const {
        if (m_parent) {
            return m_parent->getWorldRotation() * m_rotation;
        }
        return m_rotation;
    }
    
    Transform* getParent() const { return m_parent; }
    
    void setParent(Transform* parent) {
        if (m_parent) {
            auto& siblings = m_parent->m_children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }
        
        m_parent = parent;
        
        if (m_parent) {
            m_parent->m_children.push_back(this);
        }
        
        markDirty();
    }
    
    const std::vector<Transform*>& getChildren() const { return m_children; }
    
    void addChild(Transform* child) {
        child->setParent(this);
    }
    
    void removeChild(Transform* child) {
        if (child->m_parent == this) {
            child->setParent(nullptr);
        }
    }
    
    void lookAt(const Vector3<T>& target, const Vector3<T>& up = Vector3<T>::up()) {
        Vector3<T> direction = (target - m_position).normalized();
        
        Vector3<T> right = direction.cross(up).normalized();
        if (right.lengthSquared() < T(1e-10)) {
            Vector3<T> altUp = (std::abs(up.y) < T(0.9)) ? Vector3<T>::up() : Vector3<T>::right();
            right = direction.cross(altUp).normalized();
        }
        
        Vector3<T> newUp = right.cross(direction);
        
        Mat4 lookAtMatrix = Mat4::identity();
        lookAtMatrix.data[0] = right.x;
        lookAtMatrix.data[4] = right.y;
        lookAtMatrix.data[8] = right.z;
        lookAtMatrix.data[1] = newUp.x;
        lookAtMatrix.data[5] = newUp.y;
        lookAtMatrix.data[9] = newUp.z;
        lookAtMatrix.data[2] = -direction.x;
        lookAtMatrix.data[6] = -direction.y;
        lookAtMatrix.data[10] = -direction.z;
        
        T trace = lookAtMatrix.data[0] + lookAtMatrix.data[5] + lookAtMatrix.data[10];
        if (trace > T(0)) {
            T s = std::sqrt(trace + T(1)) * T(2);
            m_rotation = Quaternion<T>(
                (lookAtMatrix.data[9] - lookAtMatrix.data[6]) / s,
                (lookAtMatrix.data[2] - lookAtMatrix.data[8]) / s,
                (lookAtMatrix.data[4] - lookAtMatrix.data[1]) / s,
                T(0.25) * s
            );
        } else if (lookAtMatrix.data[0] > lookAtMatrix.data[5] && lookAtMatrix.data[0] > lookAtMatrix.data[10]) {
            T s = std::sqrt(T(1) + lookAtMatrix.data[0] - lookAtMatrix.data[5] - lookAtMatrix.data[10]) * T(2);
            m_rotation = Quaternion<T>(
                T(0.25) * s,
                (lookAtMatrix.data[4] + lookAtMatrix.data[1]) / s,
                (lookAtMatrix.data[8] + lookAtMatrix.data[2]) / s,
                (lookAtMatrix.data[9] - lookAtMatrix.data[6]) / s
            );
        } else if (lookAtMatrix.data[5] > lookAtMatrix.data[10]) {
            T s = std::sqrt(T(1) + lookAtMatrix.data[5] - lookAtMatrix.data[0] - lookAtMatrix.data[10]) * T(2);
            m_rotation = Quaternion<T>(
                (lookAtMatrix.data[4] + lookAtMatrix.data[1]) / s,
                T(0.25) * s,
                (lookAtMatrix.data[9] + lookAtMatrix.data[6]) / s,
                (lookAtMatrix.data[2] - lookAtMatrix.data[8]) / s
            );
        } else {
            T s = std::sqrt(T(1) + lookAtMatrix.data[10] - lookAtMatrix.data[0] - lookAtMatrix.data[5]) * T(2);
            m_rotation = Quaternion<T>(
                (lookAtMatrix.data[8] + lookAtMatrix.data[2]) / s,
                (lookAtMatrix.data[9] + lookAtMatrix.data[6]) / s,
                T(0.25) * s,
                (lookAtMatrix.data[4] - lookAtMatrix.data[1]) / s
            );
        }
        
        if (m_gimbalLock) {
            Vector3<T> euler = m_rotation.toEuler();
            m_pitch = euler.x;
            m_yaw = euler.y;
            m_roll = euler.z;
        }
        markDirty();
    }
    
    Vector3<T> getEulerAngles() const {
        return m_rotation.toEuler();
    }
    
    T distanceTo(const Transform& other) const {
        return (getWorldPosition() - other.getWorldPosition()).length();
    }
    
    Vector3<T> directionTo(const Transform& other) const {
        return (other.getWorldPosition() - getWorldPosition()).normalized();
    }
    
    bool isChildOf(const Transform* potentialParent) const {
        const Transform* current = m_parent;
        while (current) {
            if (current == potentialParent) return true;
            current = current->m_parent;
        }
        return false;
    }
    
    Transform* getRoot() {
        Transform* current = this;
        while (current->m_parent) {
            current = current->m_parent;
        }
        return current;
    }
    
    const Transform* getRoot() const {
        const Transform* current = this;
        while (current->m_parent) {
            current = current->m_parent;
        }
        return current;
    }
    
    static Transform lerp(const Transform& a, const Transform& b, T t) {
        Transform result;
        result.m_position = a.m_position.lerp(b.m_position, t);
        result.m_rotation = Quaternion<T>::slerp(a.m_rotation, b.m_rotation, t);
        result.m_scale = a.m_scale.lerp(b.m_scale, t);
        return result;
    }
    
    bool operator==(const Transform& other) const {
        return m_position == other.m_position &&
               m_rotation == other.m_rotation &&
               m_scale == other.m_scale;
    }
    
    bool operator!=(const Transform& other) const {
        return !(*this == other);
    }
    
    void print() const {
        std::cout << "Transform:\n";
        std::cout << "  Position: " << m_position << "\n";
        std::cout << "  Rotation: " << getEulerAngles() << " rad\n";
        std::cout << "  Scale: " << m_scale << "\n";
        std::cout << "  Forward: " << getForward() << "\n";
        std::cout << "  Up: " << getUp() << "\n";
        std::cout << "  Right: " << getRight() << "\n";
    }
    
private:
    void markDirty() {
        m_dirty = true;
        m_worldDirty = true;
        for (auto* child : m_children) {
            child->markDirty();
        }
    }
    
    void updateRotationFromEuler() {
        Quaternion<T> qYaw(Vector3<T>(0, 1, 0), m_yaw);
        Quaternion<T> qPitch(Vector3<T>(1, 0, 0), m_pitch);
        m_rotation = qYaw * qPitch;  // Yaw then pitch
        markDirty();
    }
    
    Vector3<T> m_position;
    Quaternion<T> m_rotation;
    Vector3<T> m_scale;
    
    Transform* m_parent;
    std::vector<Transform*> m_children;
    
    mutable Mat4 m_cachedWorldMatrix;
    mutable bool m_dirty;
    mutable bool m_worldDirty;
    
    bool m_gimbalLock;
    T m_yaw;
    T m_pitch;
    T m_roll;
};

using TransformF = Transform<float>;
using TransformD = Transform<double>;

} // namespace Xangine
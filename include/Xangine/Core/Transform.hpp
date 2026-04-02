#pragma once

#include <Xangine/Math/Vector3.hpp>
#include <Xangine/Math/Quaternion.hpp>
#include <Xangine/Math/Matrix4.hpp>

namespace Xangine {

struct Transform {
    Vec3 position = Vec3::zero();
    Quat rotation = Quat::identity();
    Vec3 scale = Vec3::one();
    
    // Parent-child relationships
    Transform* parent = nullptr;
    std::vector<Transform*> children;
    
    // Cached matrix (dirty flag for performance)
    mutable Mat4 cachedMatrix;
    mutable bool matrixDirty = true;
    
    // Get the world matrix (combines all parents)
    Mat4 getWorldMatrix() const;
    
    // Get local matrix (just this transform)
    Mat4 getLocalMatrix() const {
        return Mat4::translation(position) * 
               rotation.toMatrix() * 
               Mat4::scale(scale);
    }
    
    // Move relative to current orientation
    void translate(const Vec3& delta) {
        position += delta;
        matrixDirty = true;
    }
    
    // Rotate around local axes
    void rotate(const Vec3& euler) {
        rotation = Quat::fromEuler(euler) * rotation;
        matrixDirty = true;
    }
    
    // Set absolute position
    void setPosition(const Vec3& pos) {
        position = pos;
        matrixDirty = true;
    }
    
    // Mark matrix as dirty (call after manual changes)
    void invalidate() {
        matrixDirty = true;
        for (auto* child : children) {
            child->invalidate();
        }
    }
    
    // Get world position
    Vec3 getWorldPosition() const {
        return getWorldMatrix().transformPoint(Vec3::zero());
    }
    
    // Forward direction (where the object is facing)
    Vec3 forward() const {
        return rotation.rotate(Vec3(0, 0, -1));
    }
    
    // Right direction
    Vec3 right() const {
        return rotation.rotate(Vec3(1, 0, 0));
    }
    
    // Up direction
    Vec3 up() const {
        return rotation.rotate(Vec3(0, 1, 0));
    }
};

} // namespace Xangine
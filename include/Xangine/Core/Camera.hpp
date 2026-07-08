// Core/Camera.hpp
#pragma once

#include <Xangine/Math/Matrix4.hpp>
#include <Xangine/Math/Vector3.hpp>
#include <Xangine/Math/Math.hpp>
#include <Xangine/Math/Transform.hpp>
#include "BoundingBox.hpp"
#include "Ray.hpp"

namespace Xangine {

enum class ProjectionType {
    Perspective,
    Orthographic
};

struct Camera {
    TransformF transform;
    
    // Projection settings
    ProjectionType type = ProjectionType::Perspective;
    float fov = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float orthoSize = 5.0f;
    
    // Viewport
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    
    // Get view matrix using transform's position and rotation
    Mat4 getViewMatrix() const {
        Vec3 pos = transform.getWorldPosition();
        Vec3 forward = transform.getForward();
        Vec3 up = transform.getUp();
        return Mat4::lookAt(pos, pos + forward, up);
    }
    
    Mat4 getProjectionMatrix() const {
        float aspect = (float)viewportWidth / (float)viewportHeight;
        
        if (type == ProjectionType::Perspective) {
            return Mat4::perspective(
                Math::toRadians(fov),
                aspect,
                nearPlane,
                farPlane
            );
        } else {
            float right = orthoSize * aspect;
            float left = -right;
            float top = orthoSize;
            float bottom = -orthoSize;
            return Mat4::orthographic(left, right, bottom, top, nearPlane, farPlane);
        }
    }
    
    Mat4 getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }
    
    // Convenience movement methods using transform
    void moveForward(float distance) {
        transform.moveForward(distance);
    }
    
    void moveRight(float distance) {
        transform.moveRight(distance);
    }
    
    void moveUp(float distance) {
        transform.moveUp(distance);
    }
    
    void moveLocal(const Vec3& offset) {
        transform.moveLocal(offset);
    }
    
    void rotateLocal(const Vec3& eulerAngles) {
        transform.rotateLocal(eulerAngles);
    }
    
    // Frustum culling
    bool isBoxVisible(const BoundingBox& box) const;
    
    // Screen ray for picking
    Ray getScreenRay(float screenX, float screenY) const;
};

} // namespace Xangine
#pragma once

#include <Xangine/Math/Matrix4.hpp>
#include <Xangine/Math/Vector3.hpp>
#include <Xangine/Math/Math.hpp>
#include "Transform.hpp"
#include "BoundingBox.hpp"
#include "Ray.hpp"

namespace Xangine {

enum class ProjectionType {
    Perspective,   // 3D games (things get smaller in distance)
    Orthographic    // 2D UI, strategy games
};

struct Camera {
    // Transform (where camera is)
    Transform transform;
    
    // Projection settings
    ProjectionType type = ProjectionType::Perspective;
    float fov = 60.0f;              // Field of view (degrees)
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float orthoSize = 5.0f;         // For orthographic
    
    // Viewport (which part of screen)
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = 1920;
    int viewportHeight = 1080;
    
    // Get view matrix (where camera is looking)
    Mat4 getViewMatrix() const {
        Vec3 pos = transform.getWorldPosition();
        Vec3 forward = transform.forward();
        Vec3 up = transform.up();
        return Mat4::lookAt(pos, pos + forward, up);
    }
    
    // Get projection matrix (lens properties)
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
    
    // Combined matrix (most common for shaders)
    Mat4 getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }
    
    // Check if a bounding box is visible (frustum culling)
    bool isBoxVisible(const BoundingBox& box) const;
    
    // Convert screen coordinates to world ray (for picking)
    Ray getScreenRay(float screenX, float screenY) const;
};

} // namespace Xangine
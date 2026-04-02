#pragma once

#include <Xangine/Math/Vector3.hpp>
#include <Xangine/Math/Matrix4.hpp>
#include <algorithm>

namespace Xangine {

struct BoundingBox {
    Vec3 min = Vec3(std::numeric_limits<float>::max());
    Vec3 max = Vec3(-std::numeric_limits<float>::max());
    
    // Create from min/max
    BoundingBox() = default;
    BoundingBox(const Vec3& min, const Vec3& max) : min(min), max(max) {}
    
    // Add a point to expand the box
    void expand(const Vec3& point) {
        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);
        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }
    
    // Merge two boxes
    void merge(const BoundingBox& other) {
        expand(other.min);
        expand(other.max);
    }
    
    // Get center
    Vec3 center() const {
        return (min + max) * 0.5f;
    }
    
    // Get size
    Vec3 size() const {
        return max - min;
    }
    
    // Check if box is valid (has points)
    bool isValid() const {
        return min.x <= max.x && min.y <= max.y && min.z <= max.z;
    }
    
    // Transform box by matrix
    BoundingBox transform(const Mat4& matrix) const {
        // Transform all 8 corners and find new min/max
        Vec3 corners[8] = {
            matrix.transformPoint(Vec3(min.x, min.y, min.z)),
            matrix.transformPoint(Vec3(max.x, min.y, min.z)),
            matrix.transformPoint(Vec3(min.x, max.y, min.z)),
            matrix.transformPoint(Vec3(max.x, max.y, min.z)),
            matrix.transformPoint(Vec3(min.x, min.y, max.z)),
            matrix.transformPoint(Vec3(max.x, min.y, max.z)),
            matrix.transformPoint(Vec3(min.x, max.y, max.z)),
            matrix.transformPoint(Vec3(max.x, max.y, max.z))
        };
        
        BoundingBox result;
        for (const auto& corner : corners) {
            result.expand(corner);
        }
        return result;
    }
    
    // Check intersection with another box
    bool intersects(const BoundingBox& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
    
    // Check if point is inside
    bool contains(const Vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
};

} // namespace Xangine
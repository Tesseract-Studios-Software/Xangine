#pragma once

#include <Xangine/Math/Vector3.hpp>

namespace Xangine {

struct Ray {
    Vec3 origin;      // Where the ray starts
    Vec3 direction;   // Which way it points (should be normalized)
    
    Ray() : origin(Vec3::zero()), direction(Vec3::forward()) {}
    Ray(const Vec3& origin, const Vec3& direction) 
        : origin(origin), direction(direction.normalized()) {}
    
    // Get point at distance t along ray
    Vec3 getPoint(float t) const {
        return origin + direction * t;
    }
    
    // Check if ray intersects a sphere
    bool intersectsSphere(const Vec3& center, float radius, float& t) const {
        Vec3 oc = origin - center;
        float a = direction.dot(direction);
        float b = 2.0f * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0) return false;
        
        float sqrtD = std::sqrt(discriminant);
        float t1 = (-b - sqrtD) / (2 * a);
        float t2 = (-b + sqrtD) / (2 * a);
        
        t = (t1 < t2) ? t1 : t2;
        return t > 0;
    }
    
    // Check if ray intersects a plane
    bool intersectsPlane(const Vec3& planePoint, const Vec3& planeNormal, float& t) const {
        float denom = direction.dot(planeNormal);
        if (std::abs(denom) < 1e-6f) return false; // Parallel
        
        t = (planePoint - origin).dot(planeNormal) / denom;
        return t >= 0;
    }
};

} // namespace Xangine
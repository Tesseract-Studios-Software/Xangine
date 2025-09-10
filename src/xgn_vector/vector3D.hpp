// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

#include <cmath>

namespace xgn {

double degrees_to_radians(double degree) {
    return degree * 3.14159265358979323846 / 180.0;
}

struct vec3D {
    double x = 0;
    double y = 0;
    double z = 0;
    double rotation_x = 0; // pitch
    double rotation_y = 0; // yaw
    double rotation_z = 0; // roll
    
    vec3D operator+(const vec3D& other) const {
        vec3D result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;
        result.rotation_x = rotation_x + other.rotation_x;
        result.rotation_y = rotation_y + other.rotation_y;
        result.rotation_z = rotation_z + other.rotation_z;
        return result;
    }
    
    // Calculate directional movement based on current orientation
    vec3D directional_movement(vec3D movement) {
        // Convert angles to radians
        double pitch = degrees_to_radians(this->rotation_x);
        double roll = degrees_to_radians(this->rotation_y);
        double yaw = degrees_to_radians(this->rotation_z);
        
        // Normalize yaw to be between 0 and 360 degrees
        double normalized_yaw = std::fmod(this->rotation_y, 360.0);
        if (normalized_yaw < 0) normalized_yaw += 360.0;
        
        // Calculate forward direction (based on pitch and yaw)
        vec3D forward;
        forward.x = std::cos(yaw) * std::cos(pitch);
        forward.y = std::sin(yaw) * std::cos(pitch);
        forward.z = std::sin(pitch);
        
        // Calculate right direction (perpendicular to forward and world up)
        vec3D right;
        right.x = std::cos(yaw - 1.5708); // 90 degrees in radians
        right.y = std::sin(yaw - 1.5708);
        right.z = 0;
        
        // Calculate up direction (cross product of forward and right)
        vec3D up;
        up.x = forward.y * right.z - forward.z * right.y;
        up.y = forward.z * right.x - forward.x * right.z;
        up.z = forward.x * right.y - forward.y * right.x;
        
        // Normalize all direction vectors
        forward = forward.normalize();
        right = right.normalize();
        up = up.normalize();
        
        // Apply movement in local coordinate system
        vec3D result;
        result.x = this->x + forward.x * movement.x + right.x * movement.y + up.x * movement.z;
        result.y = this->y + forward.y * movement.x + right.y * movement.y + up.y * movement.z;
        result.z = this->z + forward.z * movement.x + right.z * movement.y + up.z * movement.z;
        
        // Keep the original rotations
        result.rotation_x = this->rotation_x;
        result.rotation_y = this->rotation_y;
        result.rotation_z = this->rotation_z;
        
        return result;
    }
    
    // Normalize the vector (make it unit length)
    vec3D normalize() const {
        double length = std::sqrt(x*x + y*y + z*z);
        if (length > 0) {
            return {x/length, y/length, z/length, rotation_x, rotation_y, rotation_z};
        }
        return *this;
    }
    
    // Calculate magnitude of the vector
    double magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    // Dot product
    double dot(const vec3D& other) const {
        return x*other.x + y*other.y + z*other.z;
    }
    
    // Cross product
    vec3D cross(const vec3D& other) const {
        return {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x,
            rotation_x, rotation_y, rotation_z
        };
    }
};

// Global 3D vector, similar to vec3D.
struct global_vec3D {
    vec3D global_space;
    
    // Return the global.
    vec3D calculate() {
        return this->global_space;
    }
};

// Global added with local locations and rotation.
struct local_vec3D {
    vec3D global_space;
    vec3D local_space;
    
    // Return the global added with local locations and rotation.
    vec3D calculate() {
        return this->global_space + this->local_space;
    }
    
    // Return the global added with local locations.
    vec3D calculate_location() {
        vec3D temp = global_space;
        temp.x += local_space.x;
        temp.y += local_space.y;
        temp.z += local_space.z;
        return temp;
    }
    
    // Return the global added with local rotation.
    vec3D calculate_rotation() {
        vec3D temp = global_space;
        temp.rotation_x += local_space.rotation_x;
        temp.rotation_y += local_space.rotation_y;
        temp.rotation_z += local_space.rotation_z;
        return temp;
    }
};

// Transform a point from parent's local space to world space
vec3D calculate_parent(const vec3D& parent, const vec3D& local_offset) {
    // Convert parent rotation to radians
    double pitch = degrees_to_radians(parent.rotation_x);
    double yaw = degrees_to_radians(parent.rotation_z);
    double roll = degrees_to_radians(parent.rotation_y);
    
    // Create rotation matrix
    double cosPitch = cos(pitch);
    double sinPitch = sin(pitch);
    double cosYaw = cos(yaw);
    double sinYaw = sin(yaw);
    double cosRoll = cos(roll);
    double sinRoll = sin(roll);
    
    // Create rotation matrix (simplified)
    double matrix[3][3] = {
        {cosYaw * cosRoll, -cosYaw * sinRoll, sinYaw},
        {sinPitch * sinYaw * cosRoll + cosPitch * sinRoll, -sinPitch * sinYaw * sinRoll + cosPitch * cosRoll, -sinPitch * cosYaw},
        {-cosPitch * sinYaw * cosRoll + sinPitch * sinRoll, cosPitch * sinYaw * sinRoll + sinPitch * cosRoll, cosPitch * cosYaw}
    };
    
    // Transform local offset using rotation matrix
    vec3D world_pos;
    world_pos.x = parent.x + matrix[0][0] * local_offset.x + matrix[0][1] * local_offset.y + matrix[0][2] * local_offset.z;
    world_pos.y = parent.y + matrix[1][0] * local_offset.x + matrix[1][1] * local_offset.y + matrix[1][2] * local_offset.z;
    world_pos.z = parent.z + matrix[2][0] * local_offset.x + matrix[2][1] * local_offset.y + matrix[2][2] * local_offset.z;
    
    // Keep the original rotations
    world_pos.rotation_x = parent.rotation_x;
    world_pos.rotation_y = parent.rotation_y;
    world_pos.rotation_z = parent.rotation_z;
    
    return world_pos;
}

} // namespace xgn

#endif // VECTOR3D_HPP
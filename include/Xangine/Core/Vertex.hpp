#pragma once

#include <Xangine/Math/Vector3.hpp>
#include <Xangine/Math/Vector2.hpp>

namespace Xangine {

// Different vertex layouts for different needs
enum class VertexLayout {
    P,           // Position only (particles)
    P_UV,        // Position + UV (2D sprites)
    P_N,         // Position + Normal (lighting)
    P_N_UV,      // Position + Normal + UV (most 3D models)
    P_N_UV_T     // Position + Normal + UV + Tangent (normal mapping)
};

// Standard vertex for 3D rendering
struct Vertex {
    Vec3 position;   // Where in 3D space
    Vec3 normal;     // Which way the surface points (for lighting)
    Vec2 uv;         // Texture coordinates (0-1 range)
    
    // For normal mapping (optional, add later)
    // Vec3 tangent;  
    // Vec3 bitangent;
    
    bool operator==(const Vertex& other) const {
        return position == other.position && 
               normal == other.normal && 
               uv == other.uv;
    }
};

} // namespace Xangine
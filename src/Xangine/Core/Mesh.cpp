#include <Xangine/Core/Mesh.hpp>
#include <limits>

namespace Xangine {

void Mesh::calculateBounds() {
    if (vertices.empty()) {
        bounds = BoundingBox();
        return;
    }
    
    Vec3 min(std::numeric_limits<float>::max());
    Vec3 max(-std::numeric_limits<float>::max());
    
    for (const auto& vertex : vertices) {
        min.x = std::min(min.x, vertex.position.x);
        min.y = std::min(min.y, vertex.position.y);
        min.z = std::min(min.z, vertex.position.z);
        
        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
        max.z = std::max(max.z, vertex.position.z);
    }
    
    bounds = BoundingBox(min, max);
}

void Mesh::calculateNormals() {
    // Reset normals
    for (auto& vertex : vertices) {
        vertex.normal = Vec3(0.0f, 0.0f, 0.0f);
    }
    
    // Calculate face normals and accumulate
    for (size_t i = 0; i < indices.size(); i += 3) {
        uint32_t i1 = indices[i];
        uint32_t i2 = indices[i + 1];
        uint32_t i3 = indices[i + 2];
        
        Vec3& v1 = vertices[i1].position;
        Vec3& v2 = vertices[i2].position;
        Vec3& v3 = vertices[i3].position;
        
        Vec3 normal = (v2 - v1).cross(v3 - v1).normalized();
        
        vertices[i1].normal += normal;
        vertices[i2].normal += normal;
        vertices[i3].normal += normal;
    }
    
    // Normalize all normals
    for (auto& vertex : vertices) {
        vertex.normal = vertex.normal.normalized();
    }
}

Mesh Mesh::createCube(float size) {
    Mesh mesh;
    float half = size * 0.5f;
    
    // 8 vertices
    mesh.vertices = {
        {{-half, -half,  half}}, // 0
        {{ half, -half,  half}}, // 1
        {{ half, -half, -half}}, // 2
        {{-half, -half, -half}}, // 3
        {{-half,  half,  half}}, // 4
        {{ half,  half,  half}}, // 5
        {{ half,  half, -half}}, // 6
        {{-half,  half, -half}}  // 7
    };
    
    // 12 triangles (2 per face)
    mesh.indices = {
        // Bottom face
        0, 1, 2,
        0, 2, 3,
        // Top face
        4, 6, 5,
        4, 7, 6,
        // Front face
        0, 4, 5,
        0, 5, 1,
        // Back face
        3, 2, 6,
        3, 6, 7,
        // Left face
        0, 3, 7,
        0, 7, 4,
        // Right face
        1, 5, 6,
        1, 6, 2
    };
    
    mesh.calculateNormals();
    mesh.calculateBounds();
    
    return mesh;
}

Mesh Mesh::createSphere(float radius, int segments) {
    Mesh mesh;
    // Simplified sphere - you can expand this later
    // For now, just return a cube as placeholder
    return createCube(radius * 2.0f);
}

Mesh Mesh::createPlane(float width, float depth) {
    Mesh mesh;
    float halfW = width * 0.5f;
    float halfD = depth * 0.5f;
    
    mesh.vertices = {
        {{-halfW, 0.0f, -halfD}},
        {{ halfW, 0.0f, -halfD}},
        {{ halfW, 0.0f,  halfD}},
        {{-halfW, 0.0f,  halfD}}
    };
    
    mesh.indices = {
        0, 1, 2,
        0, 2, 3
    };
    
    mesh.calculateNormals();
    mesh.calculateBounds();
    
    return mesh;
}

void Mesh::optimize() {
    // Placeholder for vertex cache optimization
    // Will implement later
}

} // namespace Xangine
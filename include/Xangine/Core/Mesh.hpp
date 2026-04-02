#pragma once

#include <vector>
#include <cstdint>
#include "Vertex.hpp"
#include "BoundingBox.hpp"

namespace Xangine {

struct Mesh {
    // The actual 3D data
    std::vector<Vertex> vertices;      // All the points
    std::vector<uint32_t> indices;     // How they connect into triangles
    
    // Optional: Sub-meshes for different materials
    struct SubMesh {
        uint32_t startIndex;    // Where in the index buffer
        uint32_t indexCount;    // How many indices
        uint32_t materialId;    // Which material to use
    };
    std::vector<SubMesh> subMeshes;
    
    // Bounding box for culling
    BoundingBox bounds;
    
    // Create a simple cube (helper function)
    static Mesh createCube(float size = 1.0f);
    
    // Create a sphere
    static Mesh createSphere(float radius = 1.0f, int segments = 32);
    
    // Create a plane
    static Mesh createPlane(float width = 1.0f, float depth = 1.0f);
    
    // Calculate normals if missing
    void calculateNormals();
    
    // Calculate bounding box from vertices
    void calculateBounds();
    
    // Optimize vertex cache for better performance
    void optimize();
    
    // Check if mesh is valid
    bool isValid() const {
        return !vertices.empty() && !indices.empty();
    }
};

} // namespace Xangine
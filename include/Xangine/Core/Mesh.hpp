#pragma once

#include <vector>
#include <cstdint>
#include "Vertex.hpp"
#include "BoundingBox.hpp"
#include <Xangine/Graphics/ShadingModel.hpp>
#include <Xangine/Graphics/Material.hpp>

namespace Xangine {

struct Mesh {
    // The actual 3D data
    std::vector<Vertex> vertices;      // All the points
    std::vector<uint32_t> indices;     // How they connect into triangles
    
    // Optional: Sub-meshes for different materials
    struct SubMesh {
        uint32_t startIndex = 0;    // Where in the index buffer
        uint32_t indexCount = 0;    // How many indices
        uint32_t materialId = 0;    // Which material to use
    };
    std::vector<SubMesh> subMeshes;

    // Materials available for the mesh and its sub-meshes
    std::vector<Material> materials;
    
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

    enum class MeshType {
        Primitive,   // Cube, plane, cylinder → Flat default
        Organic,     // Sphere, character, terrain → Smooth default
        Custom       // User decides
    };
    
    MeshType type = MeshType::Primitive;
    
    ShadingModel getRecommendedShading() const {
        switch (type) {
            case MeshType::Primitive: return ShadingModel::Flat;
            case MeshType::Organic:   return ShadingModel::Smooth;
            case MeshType::Custom:    return ShadingModel::Auto;
        }
    }

    const Material& getMaterial(uint32_t materialId) const {
        if (materialId < materials.size()) {
            return materials[materialId];
        }
        static const Material fallbackMaterial;
        return fallbackMaterial;
    }
};

} // namespace Xangine
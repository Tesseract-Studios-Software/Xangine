// Mesh.hpp
#pragma once

#include <vector>
#include <cstdint>
#include "Vertex.hpp"
#include "BoundingBox.hpp"
#include <Xangine/Math/Transform.hpp>
#include <Xangine/Graphics/ShadingModel.hpp>
#include <Xangine/Graphics/Material.hpp>

namespace Xangine {

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    
    struct SubMesh {
        uint32_t startIndex = 0;
        uint32_t indexCount = 0;
        uint32_t materialId = 0;
    };
    std::vector<SubMesh> subMeshes;
    std::vector<Material> materials;
    
    BoundingBox bounds;
    
    static Mesh createCube(float size = 1.0f);
    static Mesh createUVSphere(float radius = 1.0f, int rings = 32, int sectors = 32);
    static Mesh createPlane(float width = 1.0f, float depth = 1.0f);
    
    void calculateNormals();
    void calculateBounds();
    void optimize();
    
    bool isValid() const {
        return !vertices.empty() && !indices.empty();
    }

    enum class MeshType {
        Primitive,
        Organic,
        Custom
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
        return getDefaultMaterial();
    }
    
    void setDefaultMaterial(const Material& material) {
        m_defaultMaterial = material;
    }
    
    const Material& getDefaultMaterial() const {
        return m_defaultMaterial;
    }
    
private:
    Material m_defaultMaterial = Material::gray(0.6f, ShadingModel::Flat);
};

} // namespace Xangine
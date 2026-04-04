#pragma once

#include <Xangine/Graphics/ShadingModel.hpp>
#include <Xangine/Graphics/Texture.hpp>
#include <Xangine/Math/Vector3.hpp>

namespace Xangine {

struct Material {
    // Shading
    ShadingModel shading = ShadingModel::Flat;
    
    // Colors
    Vec3 albedo = Vec3(1.0f, 1.0f, 1.0f);
    Vec3 emissive = Vec3(0.0f, 0.0f, 0.0f);
    
    // Textures
    Texture* albedoMap = nullptr;
    Texture* normalMap = nullptr;
    Texture* metallicMap = nullptr;
    Texture* roughnessMap = nullptr;
    
    // PBR properties
    float metallic = 0.0f;
    float roughness = 0.5f;
};

} // namespace Xangine
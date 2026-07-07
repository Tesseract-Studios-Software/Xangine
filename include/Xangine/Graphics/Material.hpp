#pragma once

#include <Xangine/Graphics/ShadingModel.hpp>
#include <Xangine/Graphics/Texture.hpp>
#include <Xangine/Math/Vector3.hpp>

namespace Xangine {

struct Material {
    Material() = default;
    Material(const Vec3& color, ShadingModel model = ShadingModel::Flat)
        : shading(model), albedo(color) {}

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

    static Material fromAlbedo(const Vec3& color, ShadingModel model = ShadingModel::Flat) {
        return Material(color, model);
    }

    static Material gray(float intensity = 0.6f, ShadingModel model = ShadingModel::Flat) {
        return Material(Vec3(intensity, intensity, intensity), model);
    }
};

} // namespace Xangine
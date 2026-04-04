#pragma once

namespace Xangine {

enum class ShadingModel {
    Flat,    // Sharp faces, geometry shader calculates normals
    Smooth,  // Smooth appearance, uses vertex normals
    Auto     // Let engine decide based on mesh
};

} // namespace Xangine
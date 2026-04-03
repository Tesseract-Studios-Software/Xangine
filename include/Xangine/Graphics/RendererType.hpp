#pragma once

namespace Xangine {

enum class RendererType {
    OpenGL,
    Vulkan,
    Metal,
    Automatic  // Auto-detect best available
};

} // namespace Xangine
#pragma once

#include <Xangine/Core/Mesh.hpp>
#include <Xangine/Core/Camera.hpp>
#include <Xangine/Math/Matrix4.hpp>

namespace Xangine {

class Window;

class Renderer {
public:
    virtual ~Renderer() = default;
    
    // Lifecycle
    virtual bool initialise(Window* window) = 0;
    virtual void shutdown() = 0;
    
    // Frame operations
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void clear(float r, float g, float b, float a) = 0;
    virtual void swapBuffers() = 0;
    
    // Drawing
    virtual void drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) = 0;
    
    // Resources
    virtual void createVertexBuffer(const Mesh& mesh) = 0;
    virtual void destroyVertexBuffer(Mesh& mesh) = 0;
};

} // namespace Xangine
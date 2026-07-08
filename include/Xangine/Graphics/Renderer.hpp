// Renderer.hpp
#pragma once

#include <Xangine/Core/Mesh.hpp>
#include <Xangine/Core/Camera.hpp>
#include <Xangine/Graphics/Material.hpp>
#include <Xangine/Math/Matrix4.hpp>
#include <Xangine/Math/Transform.hpp>

namespace Xangine {

class Window;

class Renderer {
public:
    virtual ~Renderer() = default;
    
    virtual bool initialise(Window* window) = 0;
    virtual void shutdown() = 0;
    
    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;
    virtual void clear(float r, float g, float b, float a) = 0;
    virtual void swapBuffers() = 0;
    
    virtual void drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera) = 0;
    virtual void drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera, const Material& material) = 0;
    
    virtual void createVertexBuffer(const Mesh& mesh) = 0;
    virtual void destroyVertexBuffer(Mesh& mesh) = 0;
    
    void setDefaultMaterial(const Material& material) {
        m_defaultMaterial = material;
    }
    
    const Material& getDefaultMaterial() const {
        return m_defaultMaterial;
    }
    
protected:
    Material m_defaultMaterial = Material::gray(0.6f, ShadingModel::Flat);
};

} // namespace Xangine
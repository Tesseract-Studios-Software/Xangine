// src/Xangine/Graphics/Backends/OpenGL/OpenGLRenderer.hpp
#pragma once

#include <Xangine/Graphics/Renderer.hpp>
#include <Xangine/Graphics/Shader.hpp>
#include <unordered_map>

namespace Xangine {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;

    bool initialise(Window* window) override;
    void shutdown() override;

    void beginFrame() override;
    void endFrame() override;
    void clear(float r, float g, float b, float a) override;
    void swapBuffers() override;

    void drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera) override;
    void drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera, const Material& material) override;

    void createVertexBuffer(const Mesh& mesh) override;
    void destroyVertexBuffer(Mesh& mesh) override;

private:
    struct GLBuffer {
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ebo = 0;
        size_t indexCount = 0;
    };

    void drawMeshInternal(const Mesh& mesh, const TransformF& transform, const Camera& camera, const Material& material);

    std::unordered_map<const Mesh*, GLBuffer> m_buffers;
    Window* m_window = nullptr;
    Shader m_shader;
    bool m_shaderInitialised = false;
};

} // namespace Xangine
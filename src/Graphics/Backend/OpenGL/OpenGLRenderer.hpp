#pragma once

#include <Xangine/Platform/OpenGL.hpp>  // This now includes GLFW
#include <Xangine/Graphics/Renderer.hpp>
#include <Xangine/Core/Window.hpp>
#include <vector>  // For framebuffers

namespace Xangine {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;
    
    // Renderer interface
    bool initialize(Window* window) override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    void clear(float r, float g, float b, float a) override;
    void drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) override;
    void createVertexBuffer(const Mesh& mesh) override;
    void destroyVertexBuffer(Mesh& mesh) override;
    
private:
    Window* m_window = nullptr;
    unsigned int m_shaderProgram = 0;
    unsigned int m_vao = 0;  // Vertex Array Object
    unsigned int m_vbo = 0;  // Vertex Buffer Object
    unsigned int m_ebo = 0;  // Element Buffer Object
    
    void createShaders();
    void setupVertexAttributes();
};

} // namespace Xangine
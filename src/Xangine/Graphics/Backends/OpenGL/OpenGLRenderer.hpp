#pragma once

#include <Xangine/Platform/OpenGL.hpp>
#include <Xangine/Platform/GLFW.hpp>
#include <Xangine/Graphics/Renderer.hpp>
#include <Xangine/Graphics/Material.hpp>
#include <Xangine/Core/Window.hpp>

namespace Xangine {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer() override;
    
    // Renderer interface
    bool initialise(Window* window) override;
    void shutdown() override;
    void beginFrame() override;
    void endFrame() override;
    void swapBuffers() override;
    void clear(float r, float g, float b, float a) override;
    void drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) override;
    void drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera, const Material& material);
    void createVertexBuffer(const Mesh& mesh) override;
    void destroyVertexBuffer(Mesh& mesh) override;
    
private:
    Window* m_window = nullptr;
    
    // Shader programs
    unsigned int m_smoothShadingProgram = 0;
    unsigned int m_flatShadingProgram = 0;
    
    // Buffers
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    
    void createSmoothShadingShader();
    void createFlatShadingShader();
    void compileShader(unsigned int& program, const char* vertexSource, const char* fragmentSource);
    void compileShaderWithGeometry(unsigned int& program, const char* vertexSource, const char* geometrySource, const char* fragmentSource);
    void setupVertexAttributes();
    
    unsigned int OpenGLRenderer::getShaderForMaterial(const Material& material, const Mesh& mesh);
};

} // namespace Xangine
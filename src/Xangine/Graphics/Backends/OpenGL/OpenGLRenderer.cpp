#include "OpenGLRenderer.hpp"
#include <Xangine/Platform/OpenGL.hpp>   // OpenGL only
#include <Xangine/Platform/GLFW.hpp>     // GLFW for swap buffers

namespace Xangine {

OpenGLRenderer::OpenGLRenderer() = default;
OpenGLRenderer::~OpenGLRenderer() = default;

bool OpenGLRenderer::initialise(Window* window) {
    m_window = window;
    
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->getNativeHandle());
    
    glfwMakeContextCurrent(glfwWindow);
    glfwSwapInterval(1);
    
    #ifdef XANGINE_PLATFORM_MACOS
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    createShaders();
    setupVertexAttributes();
    
    return true;
}

void OpenGLRenderer::shutdown() {
    glDeleteProgram(m_shaderProgram);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void OpenGLRenderer::beginFrame() {
    // Nothing needed for OpenGL
}

void OpenGLRenderer::endFrame() {
    // Nothing needed for OpenGL
}

void OpenGLRenderer::swapBuffers() {
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_window->getNativeHandle());
    glfwSwapBuffers(glfwWindow);
}

void OpenGLRenderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) {
    Mat4 mvp = camera.getProjectionMatrix() * 
               camera.getViewMatrix() * 
               transform.getLocalMatrix();
    
    glUseProgram(m_shaderProgram);
    
    int mvpLocation = glGetUniformLocation(m_shaderProgram, "uMVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp.data[0]);
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
}

void OpenGLRenderer::createVertexBuffer(const Mesh& mesh) {
    // Upload mesh data to GPU
    // Implementation here
}

void OpenGLRenderer::destroyVertexBuffer(Mesh& mesh) {
    // Free GPU resources for mesh
    // Implementation here
}

void OpenGLRenderer::createShaders() {
    const char* vertexSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 uMVP;
        void main() {
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )";
    
    const char* fragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRenderer::setupVertexAttributes() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2
    };
    
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

} // namespace Xangine
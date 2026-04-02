#include "OpenGLRenderer.hpp"

#ifdef XANGINE_PLATFORM_MACOS
    #import <GLFW/glfw3.h>  // Ensure GLFW is imported
#endif

namespace Xangine {

OpenGLRenderer::OpenGLRenderer() = default;
OpenGLRenderer::~OpenGLRenderer() = default;

bool OpenGLRenderer::initialize(Window* window) {
    m_window = window;
    
    // Get native GLFW window handle
    GLFWwindow* glfwWindow = (GLFWwindow*)window->getNativeHandle();
    
    // Make OpenGL context current (THIS WAS YOUR ERROR)
    glfwMakeContextCurrent(glfwWindow);
    
    // Set swap interval (VSync)
    glfwSwapInterval(1);
    
    #ifdef XANGINE_PLATFORM_MACOS
        // On macOS, we need to set the OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    
    // Enable OpenGL features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Create shaders and buffers
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
    GLFWwindow* glfwWindow = (GLFWwindow*)m_window->getNativeHandle();
    glfwSwapBuffers(glfwWindow);
}

void OpenGLRenderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) {
    // Calculate MVP matrix
    Mat4 mvp = camera.getProjectionMatrix() * 
               camera.getViewMatrix() * 
               transform.getLocalMatrix();
    
    glUseProgram(m_shaderProgram);
    
    // Get uniform location and set MVP
    int mvpLocation = glGetUniformLocation(m_shaderProgram, "uMVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp.data[0]);
    
    // Draw
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void OpenGLRenderer::createVertexBuffer(const Mesh& mesh) {
    // This would upload mesh data to GPU
    // For now, we're using a hardcoded cube in setupVertexAttributes
}

void OpenGLRenderer::destroyVertexBuffer(Mesh& mesh) {
    // Cleanup
}

void OpenGLRenderer::createShaders() {
    // Vertex shader
    const char* vertexSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 uMVP;
        void main() {
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )";
    
    // Fragment shader
    const char* fragmentSource = R"(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.5, 0.2, 1.0);
        }
    )";
    
    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    
    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Link program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    // Cleanup shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRenderer::setupVertexAttributes() {
    // Create a simple triangle/cube vertex data
    float vertices[] = {
        // Triangle
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2
    };
    
    // Create VAO, VBO, EBO
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

} // namespace Xangine
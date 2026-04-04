#include "OpenGLRenderer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
    
    createSmoothShadingShader();
    createFlatShadingShader();
    
    return true;
}

void OpenGLRenderer::shutdown() {
    if (m_smoothShadingProgram) glDeleteProgram(m_smoothShadingProgram);
    if (m_flatShadingProgram) glDeleteProgram(m_flatShadingProgram);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}

void OpenGLRenderer::beginFrame() {}
void OpenGLRenderer::endFrame() {}

void OpenGLRenderer::swapBuffers() {
    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_window->getNativeHandle());
    glfwSwapBuffers(glfwWindow);
}

void OpenGLRenderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

unsigned int OpenGLRenderer::getShaderForMaterial(const Material& material, const Mesh& mesh) {
    switch (material.shading) {
        case ShadingModel::Flat:
            return m_flatShadingProgram;
        case ShadingModel::Smooth:
            return m_smoothShadingProgram;
        case ShadingModel::Auto:
        default:
            // Check if mesh has custom normals (not the default generated ones)
            // For now, a simple heuristic: if normals are all zero, use Flat
            bool hasNormals = false;
            for (const auto& vertex : mesh.vertices) {
                if (vertex.normal.lengthSquared() > 0.001f) {
                    hasNormals = true;
                    break;
                }
            }
            return hasNormals ? m_smoothShadingProgram : m_flatShadingProgram;
    }
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera) {
    // Default material
    Material defaultMaterial;
    drawMesh(mesh, transform, camera, defaultMaterial);
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const Transform& transform, const Camera& camera, const Material& material) {
    Mat4 model = transform.getLocalMatrix();
    Mat4 view = camera.getViewMatrix();
    Mat4 proj = camera.getProjectionMatrix();
    Mat4 mvp = proj * view * model;
    
    unsigned int shaderProgram = getShaderForMaterial(material, mesh);
    glUseProgram(shaderProgram);
    
    // Matrix uniforms
    int mvpLoc = glGetUniformLocation(shaderProgram, "uMVP");
    int modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    if (mvpLoc != -1) glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp.data[0]);
    if (modelLoc != -1) glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model.data[0]);
    
    // Material uniforms
    int albedoLoc = glGetUniformLocation(shaderProgram, "uAlbedo");
    if (albedoLoc != -1) glUniform3f(albedoLoc, material.albedo.x, material.albedo.y, material.albedo.z);
    
    // Lighting uniforms
    Vec3 lightPos(2.0f, 3.0f, 4.0f);
    Vec3 lightColor(1.0f, 1.0f, 1.0f);
    
    int lightPosLoc = glGetUniformLocation(shaderProgram, "uLightPos");
    int lightColorLoc = glGetUniformLocation(shaderProgram, "uLightColor");
    int cameraPosLoc = glGetUniformLocation(shaderProgram, "uCameraPos");
    
    if (lightPosLoc != -1) glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
    if (lightColorLoc != -1) glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
    if (cameraPosLoc != -1) glUniform3f(cameraPosLoc, camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
}

void OpenGLRenderer::compileShader(unsigned int& program, const char* vertexSource, const char* fragmentSource) {
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    
    // Check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }
    
    // Link program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed:\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRenderer::compileShaderWithGeometry(unsigned int& program, const char* vertexSource, const char* geometrySource, const char* fragmentSource) {
    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    
    // Geometry shader
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometrySource, nullptr);
    glCompileShader(geometryShader);
    
    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    
    // Link program
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Geometry shader linking failed:\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRenderer::createSmoothShadingShader() {
    const char* vertexSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        uniform mat4 uMVP;
        uniform mat4 uModel;
        out vec3 vNormal;
        out vec3 vWorldPos;
        
        void main() {
            gl_Position = uMVP * vec4(aPos, 1.0);
            vNormal = mat3(transpose(inverse(uModel))) * aNormal;
            vWorldPos = (uModel * vec4(aPos, 1.0)).xyz;
        }
    )";
    
    const char* fragmentSource = R"(
        #version 330 core
        in vec3 vNormal;
        in vec3 vWorldPos;
        out vec4 FragColor;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform vec3 uCameraPos;
        uniform vec3 uAlbedo;
        
        void main() {
            vec3 normal = normalize(vNormal);
            vec3 lightDir = normalize(uLightPos - vWorldPos);
            vec3 viewDir = normalize(uCameraPos - vWorldPos);
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * uLightColor;
            
            vec3 ambient = 0.3 * uLightColor;
            vec3 result = (ambient + diffuse + specular) * uAlbedo;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    compileShader(m_smoothShadingProgram, vertexSource, fragmentSource);
    std::cout << "Smooth shading shader compiled" << std::endl;
}

void OpenGLRenderer::createFlatShadingShader() {
    const char* vertexSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        uniform mat4 uMVP;
        uniform mat4 uModel;
        out vec3 vWorldPos;
        
        void main() {
            vWorldPos = (uModel * vec4(aPos, 1.0)).xyz;
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )";
    
    const char* geometrySource = R"(
        #version 330 core
        layout(triangles) in;
        layout(triangle_strip, max_vertices = 3) out;
        uniform mat4 uModel;
        in vec3 vWorldPos[];
        out vec3 gNormal;
        out vec3 gWorldPos;
        
        void main() {
            vec3 a = vWorldPos[1] - vWorldPos[0];
            vec3 b = vWorldPos[2] - vWorldPos[0];
            gNormal = normalize(cross(a, b));
            
            for (int i = 0; i < 3; i++) {
                gWorldPos = vWorldPos[i];
                gl_Position = gl_in[i].gl_Position;
                EmitVertex();
            }
            EndPrimitive();
        }
    )";
    
    const char* fragmentSource = R"(
        #version 330 core
        in vec3 gNormal;
        in vec3 gWorldPos;
        out vec4 FragColor;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform vec3 uCameraPos;
        uniform vec3 uAlbedo;
        
        void main() {
            vec3 normal = normalize(gNormal);
            vec3 lightDir = normalize(uLightPos - gWorldPos);
            vec3 viewDir = normalize(uCameraPos - gWorldPos);
            
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor;
            
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = spec * uLightColor;
            
            vec3 ambient = 0.3 * uLightColor;
            vec3 result = (ambient + diffuse + specular) * uAlbedo;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    compileShaderWithGeometry(m_flatShadingProgram, vertexSource, geometrySource, fragmentSource);
    std::cout << "Flat shading shader (with geometry shader) compiled" << std::endl;
}

void OpenGLRenderer::createVertexBuffer(const Mesh& mesh) {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    
    glBindVertexArray(m_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);
    
    setupVertexAttributes();
}

void OpenGLRenderer::destroyVertexBuffer(Mesh& mesh) {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    m_vao = m_vbo = m_ebo = 0;
}

void OpenGLRenderer::setupVertexAttributes() {
    glBindVertexArray(m_vao);
    
    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal (location 1) - used by smooth shader, ignored by flat shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // UV (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
}

} // namespace Xangine
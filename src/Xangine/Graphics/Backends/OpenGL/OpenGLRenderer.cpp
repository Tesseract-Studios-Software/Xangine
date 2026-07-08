// src/Xangine/Graphics/Backends/OpenGL/OpenGLRenderer.cpp
#include "OpenGLRenderer.hpp"
#include <Xangine/Core/Window.hpp>
#include <Xangine/Platform/OpenGL.hpp>
#include <iostream>

namespace Xangine {

OpenGLRenderer::OpenGLRenderer() = default;

OpenGLRenderer::~OpenGLRenderer() {
    shutdown();
}

bool OpenGLRenderer::initialise(Window* window) {
    m_window = window;

    const char* vertexShader = R"(
        #version 410 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aUV;

        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProjection;

        out vec3 FragNormal;
        out vec3 FragPos;

        void main() {
            vec4 worldPos = uModel * vec4(aPos, 1.0);
            FragPos = vec3(worldPos);
            FragNormal = mat3(transpose(inverse(uModel))) * aNormal;
            gl_Position = uProjection * uView * worldPos;
        }
    )";

    const char* fragmentShader = R"(
        #version 410 core
        out vec4 FragColor;

        in vec3 FragNormal;
        in vec3 FragPos;

        uniform vec3 uColor;
        uniform vec3 uLightPos;
        uniform vec3 uLightColor;
        uniform vec3 uViewPos;
        uniform int uShadingModel;

        void main() {
            vec3 normal;
            if (uShadingModel == 0) {
                normal = normalize(FragNormal);
            } else {
                normal = normalize(cross(dFdx(FragPos), dFdy(FragPos)));
            }
            
            vec3 lightDir = normalize(uLightPos - FragPos);
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * uLightColor;
            
            vec3 viewDir = normalize(uViewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = spec * uLightColor;
            
            float ambientStrength = 0.1;
            vec3 ambient = ambientStrength * uLightColor;
            
            vec3 result = (ambient + diffuse + specular) * uColor;
            FragColor = vec4(result, 1.0);
        }
    )";

    m_shaderInitialised = m_shader.loadFromSource(vertexShader, fragmentShader);
    if (!m_shaderInitialised) {
        std::cerr << "Failed to load shader!" << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    return true;
}

void OpenGLRenderer::shutdown() {
    for (auto& pair : m_buffers) {
        if (pair.second.vao) glDeleteVertexArrays(1, &pair.second.vao);
        if (pair.second.vbo) glDeleteBuffers(1, &pair.second.vbo);
        if (pair.second.ebo) glDeleteBuffers(1, &pair.second.ebo);
    }
    m_buffers.clear();
}

void OpenGLRenderer::beginFrame() {}

void OpenGLRenderer::endFrame() {}

void OpenGLRenderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::swapBuffers() {
    if (m_window) {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_window->getNativeHandle()));
    }
}

void OpenGLRenderer::createVertexBuffer(const Mesh& mesh) {
    if (m_buffers.find(&mesh) != m_buffers.end()) {
        return;
    }

    GLBuffer buffer;
    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(1, &buffer.vbo);
    glGenBuffers(1, &buffer.ebo);

    glBindVertexArray(buffer.vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);

    buffer.indexCount = mesh.indices.size();
    m_buffers[&mesh] = buffer;
}

void OpenGLRenderer::destroyVertexBuffer(Mesh& mesh) {
    auto it = m_buffers.find(&mesh);
    if (it != m_buffers.end()) {
        if (it->second.vao) glDeleteVertexArrays(1, &it->second.vao);
        if (it->second.vbo) glDeleteBuffers(1, &it->second.vbo);
        if (it->second.ebo) glDeleteBuffers(1, &it->second.ebo);
        m_buffers.erase(it);
    }
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera) {
    const Material& material = mesh.getDefaultMaterial();
    drawMeshInternal(mesh, transform, camera, material);
}

void OpenGLRenderer::drawMesh(const Mesh& mesh, const TransformF& transform, const Camera& camera, const Material& material) {
    drawMeshInternal(mesh, transform, camera, material);
}

void OpenGLRenderer::drawMeshInternal(const Mesh& mesh, const TransformF& transform, const Camera& camera, const Material& material) {
    if (!m_shaderInitialised) return;

    auto it = m_buffers.find(&mesh);
    if (it == m_buffers.end()) return;

    const GLBuffer& buffer = it->second;

    Mat4 modelMatrix = transform.getWorldMatrix();
    Mat4 viewMatrix = camera.getViewMatrix();
    Mat4 projectionMatrix = camera.getProjectionMatrix();

    Vec3 lightPos(5.0f, 10.0f, 5.0f);
    Vec3 lightColor(1.0f, 1.0f, 1.0f);
    Vec3 viewPos = camera.transform.getWorldPosition();

    int shadingModel = 0;
    if (material.shading == ShadingModel::Flat) {
        shadingModel = 1;
    } else if (material.shading == ShadingModel::Smooth) {
        shadingModel = 0;
    } else if (material.shading == ShadingModel::Auto) {
        shadingModel = (mesh.type == Mesh::MeshType::Primitive) ? 1 : 0;
    }

    m_shader.bind();
    m_shader.setMat4("uModel", modelMatrix.data);
    m_shader.setMat4("uView", viewMatrix.data);
    m_shader.setMat4("uProjection", projectionMatrix.data);
    m_shader.setVec3("uColor", material.albedo.x, material.albedo.y, material.albedo.z);
    m_shader.setVec3("uLightPos", lightPos.x, lightPos.y, lightPos.z);
    m_shader.setVec3("uLightColor", lightColor.x, lightColor.y, lightColor.z);
    m_shader.setVec3("uViewPos", viewPos.x, viewPos.y, viewPos.z);
    m_shader.setInt("uShadingModel", shadingModel);

    glBindVertexArray(buffer.vao);
    glDrawElements(GL_TRIANGLES, buffer.indexCount, GL_UNSIGNED_INT, nullptr);
}

} // namespace Xangine
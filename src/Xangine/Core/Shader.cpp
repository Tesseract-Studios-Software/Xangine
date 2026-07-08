// src/Xangine/Graphics/Shader.cpp
#include <Xangine/Graphics/Shader.hpp>
#include <Xangine/Platform/OpenGL.hpp>
#include <iostream>

namespace Xangine {

Shader::Shader() = default;

Shader::~Shader() {
    if (m_id) {
        glDeleteProgram(m_id);
    }
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        std::cerr << "Vertex shader compilation failed!" << std::endl;
        return false;
    }

    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        std::cerr << "Fragment shader compilation failed!" << std::endl;
        glDeleteShader(vertexShader);
        return false;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    int success;
    char infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        std::cerr << "Shader linking failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_id);
        m_id = 0;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::bind() const {
    glUseProgram(m_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setMat4(const std::string& name, const float* matrix) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
}

int Shader::getUniformLocation(const std::string& name) const {
    auto it = m_uniformCache.find(name);
    if (it != m_uniformCache.end()) {
        return it->second;
    }

    int location = glGetUniformLocation(m_id, name.c_str());
    m_uniformCache[name] = location;
    return location;
}

} // namespace Xangine
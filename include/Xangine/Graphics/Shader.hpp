// include/Xangine/Graphics/Shader.hpp
#pragma once

#include <string>
#include <unordered_map>

namespace Xangine {

class Shader {
public:
    Shader();
    ~Shader();

    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void bind() const;
    void unbind() const;

    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const float* matrix) const;

    unsigned int getID() const { return m_id; }
    bool isValid() const { return m_id != 0; }

private:
    unsigned int m_id = 0;
    mutable std::unordered_map<std::string, int> m_uniformCache;

    unsigned int compileShader(unsigned int type, const std::string& source);
    int getUniformLocation(const std::string& name) const;
};

} // namespace Xangine
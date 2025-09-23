#pragma once
#include <osg/Program>
#include <osg/Shader>
#include <osgDB/ReadFile>
#include <unordered_map>
#include <xgn_renderer/shader_settings.hpp>
#include <xgn_resources/resource_paths.hpp>
#include <fstream>
#include <sstream>
#include <xgn_log/log.hpp>

namespace xgn {

class ShaderManager {
public:
    static ShaderManager& instance() {
        static ShaderManager instance;
        return instance;
    }

    osg::ref_ptr<osg::Program> load_shader_program(
        const std::string& name,
        const std::string& engine_name,
        const std::string& vertex_file,
        const std::string& fragment_file,
        const std::vector<std::string>& defines = {}
    ) {
        std::string key = name;
        for (const auto& define : defines) {
            key += "_" + define;
        }
        
        if (_shaderCache.find(key) != _shaderCache.end()) {
            return _shaderCache[key];
        }

        osg::ref_ptr<osg::Program> program = new osg::Program;
        
        // Load vertex shader
        std::string vertexPath = ResourcePaths::get_shader_path(engine_name, vertex_file);
        osg::ref_ptr<osg::Shader> vertShader = load_shader_from_file(osg::Shader::VERTEX, vertexPath);
        if (vertShader) {
            program->addShader(vertShader);
        } else {
            log("0x9011", 3, "Failed to load vertex shader: " + vertexPath);
            // Fallback to embedded shader
            vertShader = create_embedded_vertex_shader();
            if (vertShader) program->addShader(vertShader);
        }

        // Load fragment shader with optional defines
        std::string fragmentPath = ResourcePaths::get_shader_path(engine_name, fragment_file);
        osg::ref_ptr<osg::Shader> fragShader = load_shader_from_file(osg::Shader::FRAGMENT, fragmentPath);
        if (fragShader) {
            if (!defines.empty()) {
                std::string source = apply_defines(fragShader->getShaderSource(), defines);
                fragShader->setShaderSource(source);
            }
            program->addShader(fragShader);
        } else {
            log("0x9012", 3, "Failed to load fragment shader: " + fragmentPath);
            // Fallback to embedded shader
            fragShader = create_embedded_fragment_shader();
            if (fragShader) program->addShader(fragShader);
        }

        // Set up attribute bindings
        program->addBindAttribLocation("position", 0);
        program->addBindAttribLocation("texCoord", 1);
        
        _shaderCache[key] = program;
        return program;
    }

    void apply_settings_to_stateset(osg::StateSet* stateset, const EngineSettings& settings, const std::string& prefix = "") {
        // This would iterate through settings and apply them as uniforms
        // Implementation depends on your specific settings structure
    }

private:
    osg::ref_ptr<osg::Shader> load_shader_from_file(osg::Shader::Type type, const std::string& filePath) {
        std::ifstream fileStream(filePath);
        if (!fileStream.is_open()) {
            log("0x9013", 3, "Failed to open shader file: " + filePath);
            return nullptr;
        }
        
        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        std::string source = buffer.str();
        
        if (source.empty()) {
            log("0x9014", 3, "Shader file is empty: " + filePath);
            return nullptr;
        }
        
        // Ensure the source starts with a valid version directive
        if (source.find("#version") == std::string::npos) {
            // Add a version directive if missing
            source = "#version 120\n" + source;
        }
        
        osg::ref_ptr<osg::Shader> shader = new osg::Shader(type);
        shader->setShaderSource(source);
        
        return shader;
    }
    
    osg::ref_ptr<osg::Shader> create_embedded_vertex_shader() {
        const char* source = 
            "#version 120\n"
            "attribute vec3 position;\n"
            "attribute vec2 texCoord;\n"
            "varying vec2 fragTexCoord;\n"
            "void main() {\n"
            "    gl_Position = vec4(position, 1.0);\n"
            "    fragTexCoord = texCoord;\n"
            "}\n";
        
        osg::ref_ptr<osg::Shader> shader = new osg::Shader(osg::Shader::VERTEX, source);
        return shader;
    }
    
    osg::ref_ptr<osg::Shader> create_embedded_fragment_shader() {
        const char* source = 
            "#version 120\n"
            "uniform sampler2D inputTexture;\n"
            "varying vec2 fragTexCoord;\n"
            "void main() {\n"
            "    vec4 color = texture2D(inputTexture, fragTexCoord);\n"
            "    gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);\n"
            "}\n";
        
        osg::ref_ptr<osg::Shader> shader = new osg::Shader(osg::Shader::FRAGMENT, source);
        return shader;
    }

    std::string apply_defines(const std::string& source, const std::vector<std::string>& defines) {
        std::stringstream finalSource;
        // Find the position after the #version directive
        size_t version_end = source.find('\n', source.find("#version"));
        if (version_end != std::string::npos) {
            finalSource << source.substr(0, version_end + 1);
            for (const auto& define : defines) {
                finalSource << "#define " << define << "\n";
            }
            finalSource << source.substr(version_end + 1);
        } else {
            // If no version directive found, add defines at the beginning
            for (const auto& define : defines) {
                finalSource << "#define " << define << "\n";
            }
            finalSource << source;
        }
        return finalSource.str();
    }

    std::unordered_map<std::string, osg::ref_ptr<osg::Program>> _shaderCache;
};

} // namespace xgn
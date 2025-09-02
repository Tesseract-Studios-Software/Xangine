#pragma once
#include <osg/Program>
#include <osg/Shader>
#include <osgDB/ReadFile>
#include <unordered_map>
#include <xgn_renderer/shader_settings.hpp>
#include <xgn_resources/resource_paths.hpp>

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
        osg::ref_ptr<osg::Shader> vertShader = osgDB::readShaderFile(osg::Shader::VERTEX, vertexPath);
        if (vertShader) program->addShader(vertShader);

        // Load fragment shader with optional defines
        std::string fragmentPath = ResourcePaths::get_shader_path(engine_name, fragment_file);
        osg::ref_ptr<osg::Shader> fragShader = osgDB::readShaderFile(osg::Shader::FRAGMENT, fragmentPath);
        if (fragShader) {
            if (!defines.empty()) {
                std::string source = apply_defines(fragShader->getShaderSource(), defines);
                fragShader->setShaderSource(source);
            }
            program->addShader(fragShader);
        }

        _shaderCache[key] = program;
        return program;
    }

    void apply_settings_to_stateset(osg::StateSet* stateset, const EngineSettings& settings, const std::string& prefix = "") {
        // This would iterate through settings and apply them as uniforms
        // Implementation depends on your specific settings structure
    }

private:
    std::string apply_defines(const std::string& source, const std::vector<std::string>& defines) {
        std::stringstream finalSource;
        for (const auto& define : defines) {
            finalSource << "#define " << define << "\n";
        }
        finalSource << source;
        return finalSource.str();
    }

    std::unordered_map<std::string, osg::ref_ptr<osg::Program>> _shaderCache;
};

} // namespace xgn
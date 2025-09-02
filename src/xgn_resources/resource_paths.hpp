#pragma once
#include <string>
#include <osgDB/FileUtils>

namespace xgn {

class ResourcePaths {
public:

static std::string get_shader_path(const std::string& engine_name, const std::string& shader_name) {
    // Try multiple possible locations
    const std::vector<std::string> possiblePaths = {
        "./shaders/" + shader_name,
        "../shaders/" + shader_name,
        "../../shaders/" + shader_name,
        "./engines/" + engine_name + "/shaders/" + shader_name,
        "../engines/" + engine_name + "/shaders/" + shader_name,
        "../../engines/" + engine_name + "/shaders/" + shader_name
    };
    
    for (const auto& path : possiblePaths) {
        if (osgDB::fileExists(path)) {
            return path;
        }
    }
    
    // Fallback: return the original name and let OSG search
    return shader_name;
}

static std::string get_engine_shader_path(const std::string& engine_name, const std::string& shader_name) {
    return get_shader_path(engine_name, shader_name);
}

};

} // namespace xgn
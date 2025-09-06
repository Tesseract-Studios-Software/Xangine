#pragma once
#include <string>
#include <osgDB/FileUtils>

namespace xgn {

class ResourcePaths {
public:

// Pick location
static const int run_type = 0; // 0 = Source; 1 = Compile

static std::string get_shader_path(const std::string& engine_name, const std::string& shader_name) {
    // Source location
    string source_location = "/src/xgn_renderer/";
    string compile_location = "/resources/xgn/";
    string picked_location = source_location;

    if (run_type == 0) {
        picked_location = source_location;
    } else if (run_type == 1) {
        picked_location = compile_location;
    }

    // Try multiple possible locations
    const std::vector<std::string> possible_paths = {
        "." + picked_location + engine_name + "/shaders/" + shader_name,
        ".." + picked_location + engine_name + "/shaders/" + shader_name,
        "../.." + picked_location + engine_name + "/shaders/" + shader_name
    };

    
    for (const auto& path : possible_paths) {
        if (osgDB::fileExists(path)) {
            cout << "Resource found at " << path << endl;
            return path;
        }
    }
    
    // Fallback: return the original name and let OSG search
    return shader_name;
}

static std::string get_engine_shader_path(const std::string& engine_name, const std::string& shader_name) {
    return get_shader_path(engine_name, shader_name);
}

static std::string get_log_path() {
    // Source location
    string source_location = "/src/xgn_log/log.txt";
    string compile_location = "/resources/xgn/log.txt";
    string picked_location = source_location;

    if (run_type == 0) {
        picked_location = source_location;
    } else if (run_type == 1) {
        picked_location = compile_location;
    }
    
    // Try multiple possible locations
    const std::vector<std::string> possible_paths = {
        "./" + picked_location,
        "../" + picked_location,
        "../../" + picked_location
    };

    
    for (const auto& path : possible_paths) {
        if (osgDB::fileExists(path)) {
            return path;
        }
    }
    
    return "log.txt";
}

static std::string get_engines_list_path() {
    // Source location
    string source_location = "/src/xgn_renderer/engines_list.txt";
    string compile_location = "/resources/xgn/engines_list.txt";
    string picked_location = source_location;

    if (run_type == 0) {
        picked_location = source_location;
    } else if (run_type == 1) {
        picked_location = compile_location;
    }
    
    // Try multiple possible locations
    const std::vector<std::string> possible_paths = {
        "./" + picked_location,
        "../" + picked_location,
        "../../" + picked_location
    };

    
    for (const auto& path : possible_paths) {
        if (osgDB::fileExists(path)) {
            return path;
        }
    }
    
    return "engines_list.txt";
}

};

} // namespace xgn
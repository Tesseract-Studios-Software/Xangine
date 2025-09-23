#pragma once
#include <xgn_renderer/render_engine_base.hpp>
#include <xgn_renderer/DirectPass/core.hpp>
#include <xgn_log/log.hpp>
#include <xgn_resources/resource_paths.hpp>
// #include <xgn_renderer/IFMQE/core.hpp>
// #include <xgn_renderer/Hyperplane/core.hpp>
#include <memory>
#include <unordered_map>
#include <fstream>

namespace xgn {

class RenderEngineManager {
public:
    RenderEngineManager(osgViewer::View* view) : _view(view) {
        _engines["DirectPass"] = std::make_unique<DirectPass::DirectPassEngine>();
        // _engines["IFMQE"] = std::make_unique<IFMQE::HyperplaneEngine>();
        // _engines["Hyperplane"] = std::make_unique<Hyperplane::HyperplaneEngine>();

        // // Register available engines
        // ifstream file(ResourcePaths::get_engines_list_path());
        
        // string line;

        // for (getline(file, line)) {
        //     _engines[line] = std::make_unique<DirectPass::DirectPassEngine>();
        // }
        
        // // Set default engine
        // _current_engine = "DirectPass";
    }
    
    // Set the active render engine
    bool set_engine(const std::string& engineName, EngineSettings& settings) {
        if (_engines.find(engineName) == _engines.end()) {
            log("0x9010", 3, "Unknown render engine: " + engineName);
            return false;
        }
        
        // Clean up previous engine passes
        if (!_current_engine.empty() && _engines.find(_current_engine) != _engines.end()) {
            cleanup_engine_passes(_current_engine);
        }
        
        // Setup new engine
        _engines[engineName]->setup_passes(_view, settings);
        _current_engine = engineName;
        
        log("0x3008", 0, "Render engine set to: " + engineName);
        return true;
    }
    
    // Get the current engine
    RenderEngineBase* get_current_engine() {
        if (_current_engine.empty()) return nullptr;
        return _engines[_current_engine].get();
    }
    
    // Get engine by name
    RenderEngineBase* get_engine(const std::string& engineName) {
        auto it = _engines.find(engineName);
        return it != _engines.end() ? it->second.get() : nullptr;
    }
    
    // Update settings for current engine
    void update_settings(const EngineSettings& settings) {
        if (auto* engine = get_current_engine()) {
            engine->update_settings(settings);
        }
    }
    
    // List available engines
    std::vector<std::string> get_available_engines() const {
        std::vector<std::string> names;
        for (const auto& pair : _engines) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    void cleanup_engine_passes(const std::string& engineName) {
        
    }

    osgViewer::View* _view;
    std::string _current_engine;
    std::unordered_map<std::string, std::unique_ptr<RenderEngineBase>> _engines;
};

} // namespace xgn
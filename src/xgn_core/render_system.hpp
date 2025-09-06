#pragma once
#include <osgViewer/View>
#include <xgn_renderer/render_engine_manager.hpp>
#include <xgn_renderer/shader_settings.hpp>

namespace xgn {

class RenderSystem {
public:
    static RenderSystem& instance() {
        static RenderSystem instance;
        return instance;
    }
    
    void initialize(osgViewer::View* view) {
        _view = view;
        _engine_manager = std::make_unique<RenderEngineManager>(view);
    }
    
    bool set_render_engine(const std::string& engineName, EngineSettings& settings) {
        if (_engine_manager) {
            return _engine_manager->set_engine(engineName, settings);
        }
        return false;
    }
    
    void update_engine_settings(EngineSettings& settings) {
        if (_engine_manager) {
            _engine_manager->update_settings(settings);
        }
    }
    
    // NEW: Method to get current engine
    RenderEngineBase* get_current_engine() {
        if (_engine_manager) {
            return _engine_manager->get_current_engine();
        }
        return nullptr;
    }
    
    // NEW: Method to toggle specific pass
    void toggle_pass(const std::string& passName, bool enabled) {
        if (auto* engine = get_current_engine()) {
            engine->toggle_pass(passName, enabled);
        }
    }

private:
    osgViewer::View* _view = nullptr;
    std::unique_ptr<RenderEngineManager> _engine_manager;
};

} // namespace xgn
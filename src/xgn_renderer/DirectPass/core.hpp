#pragma once
#include <xgn_renderer/render_pass.hpp>
#include <xgn_renderer/shader_settings.hpp>
#include <xgn_renderer/DirectPass/passes/invert_pass.hpp>
#include <xgn_core/osg_adapter.hpp>

namespace DirectPass {

class DirectPassEngine {
public:
    void setup_passes(osgViewer::Viewer* viewer, const EngineSettings& settings) {
        _settings = settings;
        
        // Create passes based on settings
        if (settings.get<bool>("directpass.invert.enabled", false)) {
            _invert_pass = new xgn::InvertPass();
            _invert_pass->apply_settings(settings);
            
            osg::ref_ptr<osg::Camera> invert_camera = _invert_pass->create_pass_camera();
            viewer->getSceneData()->asGroup()->addChild(invert_camera);
            
            _passes.push_back(_invert_pass);
        }
    }
    
    void update_settings(const EngineSettings& settings) {
        _settings = settings;
        for (auto& pass : _passes) {
            pass->apply_settings(settings);
        }
    }
    
    void toggle_pass(const std::string& passName, bool enabled) {
        for (auto& pass : _passes) {
            if (pass->get_name() == passName) {
                pass->set_enabled(enabled);
                break;
            }
        }
    }

private:
    EngineSettings _settings;
    std::vector<osg::ref_ptr<xgn::RenderPass>> _passes;
    osg::ref_ptr<xgn::InvertPass> _invert_pass;
};

} // namespace DirectPass
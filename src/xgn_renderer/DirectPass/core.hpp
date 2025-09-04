#pragma once
#include <xgn_renderer/render_engine_base.hpp>
#include <xgn_renderer/DirectPass/passes/invert_pass.hpp>
#include <osgViewer/Viewer>

namespace DirectPass {

class DirectPassEngine : public xgn::RenderEngineBase {
public:
    void setup_passes(osgViewer::View* view, const EngineSettings& settings) override {
        _viewer = view;
        _settings = settings;
        
        // Clear any existing passes
        cleanup_passes();
        
        if (settings.get<bool>("directpass.invert.enabled", false)) {
            add_invert_pass();
        }
    }
    
    void update_settings(const EngineSettings& settings) override {
        _settings = settings;
        for (auto& pass : _passes) {
            pass->apply_settings(settings);
        }
    }
    
    void toggle_pass(const std::string& passName, bool enabled) override {
        for (auto& pass : _passes) {
            if (pass->get_name() == passName) {
                pass->set_enabled(enabled);
                if (pass->get_pass_camera()) {
                    pass->get_pass_camera()->setNodeMask(enabled ? 0xFFFFFFFF : 0x0);
                }
                break;
            }
        }
    }
    
    const std::string& get_name() const override { return "DirectPass"; }
    
    void add_pass(const std::string& passName) override {
        if (passName == "InvertPass" && !has_pass("InvertPass")) {
            add_invert_pass();
        }
    }
    
    void remove_pass(const std::string& passName) override {
        for (auto it = _passes.begin(); it != _passes.end(); ++it) {
            if ((*it)->get_name() == passName) {
                if ((*it)->get_pass_camera()) {
                    _viewer->getSceneData()->asGroup()->removeChild((*it)->get_pass_camera());
                }
                _passes.erase(it);
                break;
            }
        }
    }

private:
    void add_invert_pass() {
        _invert_pass = new xgn::InvertPass();
        _invert_pass->apply_settings(_settings);
        
        osg::ref_ptr<osg::Camera> invert_camera = _invert_pass->create_pass_camera();
        _viewer->getSceneData()->asGroup()->addChild(invert_camera);
        
        _passes.push_back(_invert_pass);
    }
    
    void cleanup_passes() {
        for (auto& pass : _passes) {
            if (pass->get_pass_camera()) {
                _viewer->getSceneData()->asGroup()->removeChild(pass->get_pass_camera());
            }
        }
        _passes.clear();
    }
    
    bool has_pass(const std::string& passName) {
        for (const auto& pass : _passes) {
            if (pass->get_name() == passName) return true;
        }
        return false;
    }

    osgViewer::View* _viewer;
    EngineSettings _settings;
    std::vector<osg::ref_ptr<xgn::RenderPass>> _passes;
    osg::ref_ptr<xgn::InvertPass> _invert_pass;
};

} // namespace DirectPass
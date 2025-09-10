#pragma once
#include <xgn_renderer/render_engine_base.hpp>
#include <xgn_renderer/DirectPass/passes/invert_pass.hpp>
#include <osgViewer/Viewer>

using namespace xgn;

namespace DirectPass {

class DirectPassEngine : public xgn::RenderEngineBase {
public:
    void setup_passes(osgViewer::View* view, const EngineSettings& settings) override {
        _view = view;
        _settings = settings;
        
        // Store original camera settings
        if (_view && _view->getCamera()) {
            _originalClearColor = _view->getCamera()->getClearColor();
            _originalClearMask = _view->getCamera()->getClearMask();
        }
        
        // Clear any existing passes
        cleanup_passes();
        
        if (settings.get<bool>("directpass.invert.enabled", false)) {
            add_invert_pass();
            enable_post_processing();
        } else {
            enable_direct_rendering();
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
            
            if (passName == "InvertPass") {
                if (enabled) {
                    enable_post_processing();
                } else {
                    enable_direct_rendering();
                }
            }
            
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
                    _view->getSceneData()->asGroup()->removeChild((*it)->get_pass_camera());
                }
                _passes.erase(it);
                break;
            }
        }
    }

    void enable_direct_rendering() {
        if (!_view || !_view->getCamera()) return;
        
        osg::Camera* mainCamera = _view->getCamera();
        
        // Restore original rendering to screen
        mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
        mainCamera->setClearColor(_originalClearColor);
        mainCamera->setClearMask(_originalClearMask);
        
        // Disable invert pass camera
        if (_invert_pass && _invert_pass->get_pass_camera()) {
            _invert_pass->get_pass_camera()->setNodeMask(0x0);
        }
    }

    void enable_post_processing() {
        if (!_view || !_view->getCamera()) return;
        
        osg::Camera* mainCamera = _view->getCamera();
        
        // Configure main camera to render to texture
        mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        mainCamera->attach(osg::Camera::COLOR_BUFFER, _mainRenderTexture);
        
        // Enable invert pass camera
        if (_invert_pass && _invert_pass->get_pass_camera()) {
            _invert_pass->get_pass_camera()->setNodeMask(0xFFFFFFFF);
        }
    }

private:
    void add_invert_pass() {
        if (!_view || !_view->getCamera()) {
            log("0x9030", 3, "No view or camera available for invert pass");
            return;
        }
        
        // Get the main camera
        osg::Camera* mainCamera = _view->getCamera();
        
        // Store the original clear color and mask
        _originalClearColor = mainCamera->getClearColor();
        _originalClearMask = mainCamera->getClearMask();
        
        // Create texture for main camera to render to
        _mainRenderTexture = new osg::Texture2D;
        _mainRenderTexture->setTextureSize(mainCamera->getViewport()->width(), 
                                        mainCamera->getViewport()->height());
        _mainRenderTexture->setInternalFormat(GL_RGBA);
        _mainRenderTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        _mainRenderTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        _mainRenderTexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_EDGE);
        _mainRenderTexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);
        
        // Configure main camera to render to texture
        mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        mainCamera->attach(osg::Camera::COLOR_BUFFER, _mainRenderTexture);
        
        // Now create the invert pass
        _invert_pass = new xgn::InvertPass();
        _invert_pass->set_input_texture(0, _mainRenderTexture);
        _invert_pass->apply_settings(_settings);
        
        osg::ref_ptr<osg::Camera> invert_camera = _invert_pass->create_pass_camera();
        
        // Configure invert camera to render to screen
        invert_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
        invert_camera->setClearMask(0); // Don't clear, we're compositing
        
        _view->getSceneData()->asGroup()->addChild(invert_camera);
        
        _passes.push_back(_invert_pass);
        
        log("0x9031", 1, "Invert pass setup complete");
    }
    
    void cleanup_passes() {
        for (auto& pass : _passes) {
            if (pass->get_pass_camera()) {
                _view->getSceneData()->asGroup()->removeChild(pass->get_pass_camera());
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

    osg::Vec4 _originalClearColor;
    GLbitfield _originalClearMask;
    osg::ref_ptr<osg::Texture2D> _mainRenderTexture;
    osgViewer::View* _view = nullptr;
    EngineSettings _settings;
    std::vector<osg::ref_ptr<xgn::RenderPass>> _passes;
    osg::ref_ptr<xgn::InvertPass> _invert_pass;
};

} // namespace DirectPass
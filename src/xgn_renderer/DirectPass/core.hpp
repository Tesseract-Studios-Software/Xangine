#pragma once
#include <xgn_renderer/render_engine_base.hpp>
#include <xgn_renderer/DirectPass/passes/invert_pass.hpp>
#include <osgViewer/Viewer>

using namespace xgn;

namespace DirectPass {

class DirectPassEngine : public xgn::RenderEngineBase {
public:
    void setup_passes(osgViewer::View* view, const EngineSettings& settings) override {
        _view = view;  // Store the view reference
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
                    _view->getSceneData()->asGroup()->removeChild((*it)->get_pass_camera());
                }
                _passes.erase(it);
                break;
            }
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
        
        // Create texture for main camera to render to
        osg::ref_ptr<osg::Texture2D> mainRenderTexture = new osg::Texture2D;
        mainRenderTexture->setTextureSize(mainCamera->getViewport()->width(), 
                                        mainCamera->getViewport()->height());
        mainRenderTexture->setInternalFormat(GL_RGBA);
        mainRenderTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        mainRenderTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        mainRenderTexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_EDGE);
        mainRenderTexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);
        
        // Configure main camera to render to texture
        mainCamera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        mainCamera->attach(osg::Camera::COLOR_BUFFER, mainRenderTexture);
        
        // Ensure main camera still clears the buffers
        mainCamera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mainCamera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
        
        // Now create the invert pass
        _invert_pass = new xgn::InvertPass();
        _invert_pass->set_input_texture(0, mainRenderTexture);
        _invert_pass->apply_settings(_settings);
        
        osg::ref_ptr<osg::Camera> invert_camera = _invert_pass->create_pass_camera();
        
        // Add the invert camera as a child of the main camera's parent
        // This ensures it's in the scene graph but not confused with the main camera
        osg::Group* root = dynamic_cast<osg::Group*>(_view->getSceneData());
        if (root) {
            root->addChild(invert_camera);
        } else {
            // Fallback: add to main camera's parent
            osg::Group* newRoot = new osg::Group;
            newRoot->addChild(_view->getSceneData());
            newRoot->addChild(invert_camera);
            _view->setSceneData(newRoot);
        }
        
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

    osgViewer::View* _view = nullptr;
    EngineSettings _settings;
    std::vector<osg::ref_ptr<xgn::RenderPass>> _passes;
    osg::ref_ptr<xgn::InvertPass> _invert_pass;
};

} // namespace DirectPass
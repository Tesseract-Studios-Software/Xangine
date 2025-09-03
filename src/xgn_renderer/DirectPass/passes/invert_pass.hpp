#pragma once
#include <xgn_renderer/render_pass.hpp>
#include <xgn_renderer/shader_manager.hpp>

namespace xgn {

class InvertPass : public RenderPass {
public:
    InvertPass() { 
        _name = "InvertPass";
        _output_texture = new osg::Texture2D;
        _output_texture->setTextureSize(1024, 768);
        _output_texture->setInternalFormat(GL_RGBA);
    }

    osg::ref_ptr<osg::Camera> create_pass_camera() override {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setRenderOrder(osg::Camera::POST_RENDER);
        camera->setClearMask(0);
        
        // Create full-screen quad
        osg::ref_ptr<osg::Geode> quad = create_fullscreen_quad();
        camera->addChild(quad);
        
        // Configure camera to render to texture
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
        camera->attach(osg::Camera::COLOR_BUFFER, _output_texture);
        
        return camera;
    }

    void apply_settings(const EngineSettings& settings) override {
        _enabled = settings.get<bool>("invert.enabled", true);
        // Add more settings as needed
    }

    void set_input_texture(int unit, osg::ref_ptr<osg::Texture2D> texture) override {
        _input_texture = texture;
        // This would be used in the shader
    }

    osg::ref_ptr<osg::Texture2D> get_output_texture() const override { 
        return _output_texture; 
    }

private:
    osg::ref_ptr<osg::Geode> create_fullscreen_quad() {
        osg::ref_ptr<osg::Geode> quadGeode = new osg::Geode;
        osg::ref_ptr<osg::Geometry> quadGeometry = osg::createTexturedQuadGeometry(
            osg::Vec3(-1, -1, 0), osg::Vec3(2, 0, 0), osg::Vec3(0, 2, 0));
        
        // Load shader
        osg::ref_ptr<osg::Program> program = ShaderManager::instance().load_shader_program(
            "Invert", engine_name, "common.vert", "invert.frag");
        
        osg::StateSet* stateset = quadGeometry->getOrCreateStateSet();
        stateset->setAttributeAndModes(program);
        
        // Set up uniforms
        if (_input_texture) {
            stateset->setTextureAttributeAndModes(0, _input_texture);
            stateset->addUniform(new osg::Uniform("inputTexture", 0));
        }
        
        quadGeode->addDrawable(quadGeometry);
        return quadGeode;
    }

    osg::ref_ptr<osg::Texture2D> _input_texture;
    string engine_name = "DirectPass";
};

} // namespace xgn
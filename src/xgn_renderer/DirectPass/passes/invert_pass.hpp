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
        _pass_camera = new osg::Camera;
        
        // Set render order to POST_RENDER (after main scene)
        _pass_camera->setRenderOrder(osg::Camera::POST_RENDER);
        
        // Don't clear any buffers - we want to composite over the existing render
        _pass_camera->setClearMask(0);
        
        // Set up the camera to cover the entire screen
        _pass_camera->setProjectionMatrix(osg::Matrix::ortho2D(-1, 1, -1, 1));
        _pass_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        _pass_camera->setViewMatrix(osg::Matrix::identity());
        
        // Render to the screen, not to a texture
        _pass_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
        
        // Create full-screen quad
        osg::ref_ptr<osg::Geode> quad = create_fullscreen_quad();
        _pass_camera->addChild(quad);
        
        return _pass_camera;
    }

    void apply_settings(const EngineSettings& settings) override {
        _enabled = settings.get<bool>("invert.enabled", true);
        // Show/hide the camera based on enabled state
        if (_pass_camera) {
            _pass_camera->setNodeMask(_enabled ? 0xFFFFFFFF : 0x0);
        }
    }

    void set_input_texture(int unit, osg::ref_ptr<osg::Texture2D> texture) override {
        _input_texture = texture;
        // Update the shader with the new input texture
        if (_quadGeode) {
            update_shader_texture();
        }
    }

    osg::ref_ptr<osg::Texture2D> get_output_texture() const override { 
        return _output_texture; 
    }

    // Helper method to set up the pass with a specific input texture
    void setup_with_input_texture(osg::ref_ptr<osg::Texture2D> inputTexture) {
        _input_texture = inputTexture;
        if (_pass_camera && _quadGeode) {
            update_shader_texture();
        }
    }

private:
    void update_shader_texture() {
        if (!_input_texture || !_quadGeode) return;
        
        osg::StateSet* stateset = _quadGeode->getOrCreateStateSet();
        
        // Remove any existing texture attributes
        stateset->removeTextureAttribute(0, osg::StateAttribute::TEXTURE);
        
        // Set the new texture
        stateset->setTextureAttributeAndModes(0, _input_texture, osg::StateAttribute::ON);
        stateset->addUniform(new osg::Uniform("inputTexture", 0));
        
        // Ensure texture parameters are set correctly
        _input_texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
        _input_texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
        _input_texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP_TO_EDGE);
        _input_texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP_TO_EDGE);
    }

    osg::ref_ptr<osg::Geode> create_fullscreen_quad() {
        _quadGeode = new osg::Geode;
        osg::ref_ptr<osg::Geometry> quadGeometry = osg::createTexturedQuadGeometry(
            osg::Vec3(-1, -1, 0), osg::Vec3(2, 0, 0), osg::Vec3(0, 2, 0));
        
        // Load shader
        osg::ref_ptr<osg::Program> program = ShaderManager::instance().load_shader_program(
            "Invert", "DirectPass", "common.vert", "invert.frag");
        
        osg::StateSet* stateset = quadGeometry->getOrCreateStateSet();
        stateset->setAttributeAndModes(program);

        // Check if program has any shaders
        if (program->getNumShaders() == 0) {
            log("0x9018", 3, "Shader program has no shaders, using fallback");
            
            // Create simple fallback shaders
            const char* simpleVertSource = 
                "void main() { gl_Position = ftransform(); }";
            
            const char* simpleFragSource = 
                "void main() { gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0); }";
            
            osg::ref_ptr<osg::Shader> simpleVert = new osg::Shader(osg::Shader::VERTEX, simpleVertSource);
            osg::ref_ptr<osg::Shader> simpleFrag = new osg::Shader(osg::Shader::FRAGMENT, simpleFragSource);
            
            program->addShader(simpleVert);
            program->addShader(simpleFrag);
        }
        
        // Set up uniforms (texture will be added later via set_input_texture)
        stateset->addUniform(new osg::Uniform("inputTexture", 0));
        
        _quadGeode->addDrawable(quadGeometry);

        return _quadGeode;
    }

    osg::ref_ptr<osg::Texture2D> _input_texture;
    osg::ref_ptr<osg::Geode> _quadGeode;
};

} // namespace xgn
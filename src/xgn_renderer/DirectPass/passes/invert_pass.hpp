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
        
        // Critical: Set up the camera properly for post-processing
        _pass_camera->setName("InvertPassCamera");
        _pass_camera->setRenderOrder(osg::Camera::POST_RENDER);
        _pass_camera->setClearMask(0); // Don't clear the buffer
        _pass_camera->setAllowEventFocus(false);
        
        // Set up orthographic projection for fullscreen quad
        _pass_camera->setProjectionMatrix(osg::Matrix::ortho2D(-1, 1, -1, 1));
        _pass_camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
        _pass_camera->setViewMatrix(osg::Matrix::identity());
        
        // Render to the screen
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
        
        // Create a proper fullscreen quad
        osg::ref_ptr<osg::Geometry> quadGeometry = new osg::Geometry;
        
        // Vertices for fullscreen quad (NDC coordinates)
        osg::Vec3Array* vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f)); // Bottom-left
        vertices->push_back(osg::Vec3( 1.0f, -1.0f, 0.0f)); // Bottom-right
        vertices->push_back(osg::Vec3( 1.0f,  1.0f, 0.0f)); // Top-right
        vertices->push_back(osg::Vec3(-1.0f,  1.0f, 0.0f)); // Top-left
        quadGeometry->setVertexArray(vertices);
        
        // Texture coordinates
        osg::Vec2Array* texCoords = new osg::Vec2Array;
        texCoords->push_back(osg::Vec2(0.0f, 0.0f)); // Bottom-left
        texCoords->push_back(osg::Vec2(1.0f, 0.0f)); // Bottom-right
        texCoords->push_back(osg::Vec2(1.0f, 1.0f)); // Top-right
        texCoords->push_back(osg::Vec2(0.0f, 1.0f)); // Top-left
        quadGeometry->setTexCoordArray(0, texCoords);
        
        // Normals (required)
        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
        quadGeometry->setNormalArray(normals, osg::Array::BIND_OVERALL);
        
        // Primitive set
        quadGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
        
        // Create simple shader
        const char* vertSource = 
            "varying vec2 texCoord;\n"
            "void main() {\n"
            "    gl_Position = ftransform();\n"
            "    texCoord = gl_MultiTexCoord0.st;\n"
            "}\n";
        
        const char* fragSource = 
            "uniform sampler2D inputTexture;\n"
            "varying vec2 texCoord;\n"
            "void main() {\n"
            "    vec4 color = texture2D(inputTexture, texCoord);\n"
            "    gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);\n"
            "}\n";
        
        osg::ref_ptr<osg::Program> program = new osg::Program;
        program->addShader(new osg::Shader(osg::Shader::VERTEX, vertSource));
        program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragSource));
        
        osg::StateSet* stateset = quadGeometry->getOrCreateStateSet();
        stateset->setAttributeAndModes(program, osg::StateAttribute::ON);
        
        // Set up texture uniform
        if (_input_texture) {
            stateset->setTextureAttributeAndModes(0, _input_texture);
            stateset->addUniform(new osg::Uniform("inputTexture", 0));
        }
        
        // Disable lighting for the quad
        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
        
        _quadGeode->addDrawable(quadGeometry);
        return _quadGeode;
    }

    osg::ref_ptr<osg::Texture2D> _input_texture;
    osg::ref_ptr<osg::Geode> _quadGeode;
};

} // namespace xgn
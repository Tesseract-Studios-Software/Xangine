#pragma once
#include <xgn_renderer/render_pass.hpp>
#include <osgDB/ReadFile>

using namespace xgn;

class InvertPass : public RenderPass {
public:
    InvertPass() { _name = "InvertPass"; }

    osg::ref_ptr<osg::Camera> create_pass_camera() override {
        // 1. Create the camera (as shown in previous examples)
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setRenderOrder(osg::Camera::POST_RENDER);
        camera->setClearMask(0); // Don't clear color buffer

        // 3. **CRITICAL: Attach the specific shader for this pass**
        osg::ref_ptr<osg::Program> program = new osg::Program;
        program->addShader(osgDB::readShaderFile(osg::Shader::FRAGMENT, ""));

        quad->getOrCreateStateSet()->setAttributeAndModes(program);

        // 4. Set up input uniforms (e.g., bind the G-Buffer textures from a previous pass)
        osg::StateSet* ss = quad->getStateSet();
        ss->addUniform(new osg::Uniform("u_gBufferAlbedo", 0)); // Texture unit 0
        ss->addUniform(new osg::Uniform("u_gBufferNormal", 1)); // Texture unit 1
        // ... etc.

        camera->addChild(quad);
        return camera;
    }

    // This pass might render to a texture for the next pass to use
    osg::ref_ptr<osg::Texture2D> getOutputTexture() const override { return _outputTexture; }

private:
    osg::ref_ptr<osg::Texture2D> _outputTexture;
};
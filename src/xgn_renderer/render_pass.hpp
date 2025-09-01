// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef RENDER_PASS_MAIN
#define RENDER_PASS_MAIN

#include <osg/Camera>
#include <osg/Texture2D>

namespace xgn {

// Abstract base class for all render passes
class RenderPass {
public:
    virtual ~RenderPass() = default;

    // Pure virtual function. Each concrete pass (SSGI, Composite) must implement this.
    // It creates the camera for this pass and sets up its shaders and textures.
    virtual osg::ref_ptr<osg::Camera> create_pass_camera() = 0;

    // Optional: Get the output texture so the next pass can use it as input.
    virtual osg::ref_ptr<osg::Texture2D> getOutputTexture() const { return nullptr; }

    const std::string& getName() const { return _name; }

protected:
    std::string _name;
};

};

#endif // RENDER_PASS_MAIN
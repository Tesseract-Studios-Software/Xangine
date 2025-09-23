#pragma once
#include <osg/Camera>
#include <osg/Texture2D>
#include <osg/Program>
#include <xgn_renderer/shader_settings.hpp>

namespace xgn {

class RenderPass : public osg::Referenced {
public:
    virtual ~RenderPass() = default;

    virtual osg::ref_ptr<osg::Camera> create_pass_camera() = 0;
    virtual osg::ref_ptr<osg::Texture2D> get_output_texture() const { return nullptr; }
    virtual osg::ref_ptr<osg::Camera> get_pass_camera() const { return _pass_camera; } // ← ADD THIS METHOD
    
    virtual void apply_settings(const EngineSettings& settings) = 0;
    virtual void set_input_texture(int unit, osg::ref_ptr<osg::Texture2D> texture) = 0;

    const std::string& get_name() const { return _name; }
    bool is_enabled() const { return _enabled; }
    void set_enabled(bool enabled) { _enabled = enabled; }

protected:
    std::string _name;
    bool _enabled = true;
    osg::ref_ptr<osg::Texture2D> _output_texture;
    osg::ref_ptr<osg::Camera> _pass_camera;
};

} // namespace xgn
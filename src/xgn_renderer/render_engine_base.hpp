#pragma once
#include <osgViewer/Viewer>
#include <xgn_renderer/shader_settings.hpp>

namespace xgn {

class RenderEngineBase {
public:
    virtual ~RenderEngineBase() = default;
    
    virtual void setup_passes(osgViewer::Viewer* viewer, const EngineSettings& settings) = 0;
    virtual void update_settings(const EngineSettings& settings) = 0;
    virtual void toggle_pass(const std::string& passName, bool enabled) = 0;
    virtual const std::string& get_name() const = 0;
    
    // Optional: Methods for dynamic pass management
    virtual void add_pass(const std::string& passName) = 0;
    virtual void remove_pass(const std::string& passName) = 0;
};

} // namespace xgn
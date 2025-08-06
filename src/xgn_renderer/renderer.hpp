#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window& loading_window) {
    // Update all objects first
    loading_window = update_objects(loading_window);
    update_camera_position(loading_window.interfaces[0].scenes[loading_window.interfaces[0].scene_in_use].main_camera, loading_window.viewer);
    
    // Then render
    loading_window.done = loading_window.viewer->done();
    if (!loading_window.done) {
        loading_window.viewer->frame();
    }
    return 0;
}

};

#endif // RENDERER_HPP
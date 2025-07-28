#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window& loading_window) {
    // Update all objects first
    update_objects(loading_window);
    
    // Then render
    loading_window.done = loading_window.viewer->done();
    if (!loading_window.done) {
        loading_window.viewer->frame();
    }
    return 0;
}

};

#endif // RENDERER_HPP
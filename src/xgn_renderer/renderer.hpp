#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window*& loading_window) {
    // Null check first
    if (!loading_window || !loading_window->viewer) {
        log("0x9007", 3);
        return -1;
    }

    // Verify graphics context
    if (!loading_window->viewer->getCamera() || 
        !loading_window->viewer->getCamera()->getGraphicsContext()) {
        log("0x9008", 3);
        return -1;
    }

    // Update objects
    update_objects(loading_window);

    // Update cameras
    for (auto& interface : loading_window->interfaces) {
        if (!interface || !interface->viewer) continue;
        update_camera_position(
            interface->scenes[interface->scene_in_use]->main_camera, 
            interface->viewer
        );
    }

    // Render frame
    try {
        loading_window->viewer->frame();
    } catch (const std::exception& e) {
        log("0x9009", 3, e.what());
        return -1;
    }

    loading_window->done = loading_window->viewer->done();
    return 0;
}

};

#endif // RENDERER_HPP
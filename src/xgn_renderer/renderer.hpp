// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <xgn_window/window.hpp>
#include <xgn_core/osg_adapter.hpp>
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window*& loading_window) {
    if (!loading_window || !loading_window->viewer) {
        log("0x9007", 3);
        return -1;
    }

    // Update objects
    update_objects(loading_window);

    // Update cameras for each view
    for (auto* interface : loading_window->interfaces) {
        auto* scene = interface->scenes[interface->scene_in_use];
        if (scene && scene->main_camera) {
            update_camera_position(scene->main_camera, loading_window->viewer->getView(0)->getCamera());
        }
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
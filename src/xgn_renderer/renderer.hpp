#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window loading_window, osg::ref_ptr<osg::Group> root, osgViewer::Viewer viewer) {
    int i, j, k, l;
    xgn3D::object current_object;
    for (i = 0; i < loading_window.interfaces.size(); i++) {
        if (loading_window.interfaces[i].interface_type == "3D") {
            // Load all 3D objects
            for (j = 0; j < loading_window.interfaces[i].scenes[loading_window.interfaces[i].scene_in_use].objects_loaded.size(); j++) {
                current_object = loading_window.interfaces[i].scenes[loading_window.interfaces[i].scene_in_use].objects_loaded[j];
                
            }
        }
        else if (loading_window.interfaces[i].interface_type == "UI") {
            // Write in future updates.
        }
        else {
            log("103", 4, "(" + loading_window.interfaces[i].interface_type + ")");
        }
    }
}

};

#endif // RENDERER_HPP
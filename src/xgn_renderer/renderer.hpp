#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "../xgn_window/window.hpp"
#include <vector>

namespace xgn {

// Render the next frame.
int render_frame(window& loading_window) {
    int i, j, k = 0;

    for (i = 0; i < loading_window.data_count; i++) {
        if (loading_window.interfaces[i].interface_type == "3D") {
            // Load all 3D objects
            for (j = 0; j < loading_window.interfaces[i].objects_loaded.size(); j++) {
                // loading_window.interfaces[i].objects_loaded[j] is the object.
                // 3D object handling.

            }
        }
        else if (loading_window.interfaces[i].interface_type == "UI") {
            // 
        }
    }
}

};

#endif // RENDERER_HPP
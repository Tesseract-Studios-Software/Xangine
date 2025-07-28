#ifndef CORE_HPP
#define CORE_HPP

#include "../xgn3D_object/object.hpp"
#include "./3D_data_loader.hpp"
#include "./3D_obj_file_loader.hpp"
#include "./osg_adapter.hpp"
#include "../xgn_renderer/renderer.hpp"
#include <vector>
#include <libgen.h>
#include <unistd.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
using namespace std;
using namespace xgn3D;
using namespace xgn;

namespace xgn {

// Outputs the corresponding string of the number.
void code(const int& the_number) {
    if (the_number == 3) {
        log("3", 5);
    } else if (the_number == 2) {
        log("2", 5);
    } else if (the_number == 1) {
        log("1", 5);
    } else if (the_number == 0) {
        log("0", 5);
    }
}

// Sets executable path to the current working directory.
void set_executable_dir() {
    string exe_dir;
    string dir;
    char path[1024];
    uint32_t size = sizeof(path);
#if defined(__linux__)
    ssize_t len = readlink("/proc/self/exe", path, size - 1);
    if (len != -1) {
        path[len] = '\0';
        exe_dir = string(dirname(path));
        chdir(exe_dir.c_str());
    }
#elif defined(__APPLE__)
    uint32_t bufsize = sizeof(path);
    if (_NSGetExecutablePath(path, &bufsize) == 0) {
        exe_dir = string(dirname(path));
        chdir(exe_dir.c_str());
    }
#endif
    exe_dir = ".";
    chdir(exe_dir.c_str());
}

// This loads all objects in an interface, it is not suggested to call this repeatively or else it may cause lag.
vector<xgn3D::object> load_all_objects(interface& loading_interface) {
    return xgn::load_all_data(loading_interface);
}

// This loads the object inputted as the argument.
xgn3D::object load_object(object& loading_object) {
    return load_data(loading_object);
}

// // Spawn new window.
// int new_window(window& window_new) {
//     log("102", 1);
// }

// Initialise Xangine instance.
window init(window& default_window) {
    // Return the configured viewer and root
    auto [viewer, root] = setup_osg(default_window); 
    
    // Store in window struct
    default_window.viewer = viewer;
    default_window.root = root;
    root->addChild(create_axes());
    return default_window;
}

int frame(window window) {
    return render_frame(window);
}

};

#endif // CORE_HPP
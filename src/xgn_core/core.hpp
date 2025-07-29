#ifndef CORE_HPP
#define CORE_HPP

#include "../xgn3D_object/object.hpp"
#include "./3D_data_loader.hpp"
#include "./osg_adapter.hpp"
#include "../xgn_renderer/renderer.hpp"
#include <vector>
#include <libgen.h>
#include <unistd.h>
#include <time.h>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
using namespace std;
using namespace xgn3D;
using namespace xgn;

namespace xgn {

// Outputs the corresponding string of the number.
void code(const int& the_number) {
    if (the_number == 0) {
        log("0xffff", 5);
    } else if (the_number == 1) {
        log("0xfffe", 5);
    } else if (the_number == 2) {
        log("0xfffd", 5);
    } else if (the_number == 3) {
        log("0xfffc", 5);
    } else if (the_number == 13) {
        log("0x000d", 5);
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

// Initialise Xangine instance.
window init(window& default_window) {
    auto [viewer, root] = setup_osg(default_window);
    
    // Store references
    default_window.viewer = viewer;
    default_window.root = root;
    
    return default_window;
}

int check_xangine_instance(window& default_window) {
    if (!default_window.root || !default_window.viewer) {
        log("0xd000", 5);
        return -1;
    }
    else {
        return 0;
    }
}

pair<int, float> frame(window window, int fps_limit) {
    // Use time.h to get current time
    static clock_t last_time = clock();
    clock_t current_time = clock();
    double elapsed_time = double(current_time - last_time) / CLOCKS_PER_SEC;
    if (elapsed_time < 1.0 / fps_limit) {
        // Wait until the next frame
        usleep((1.0 / fps_limit - elapsed_time) * 1000000);
        current_time = clock(); // Update current time after waiting
        elapsed_time = double(current_time - last_time) / CLOCKS_PER_SEC;
    }
    return {render_frame(window), 1.0 / elapsed_time};
}

};

#endif // CORE_HPP
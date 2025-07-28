#include "./src/xgn_log/log.hpp"
#include "./src/xgn_core/core.hpp"
using namespace std;

int main() {
    int i;
    set_executable_dir(); // Set the working directory to the executable directory.
    log("99", 1, "The tester is now about to test all error codes.");
    log("95", 1);
    for (i = 100; i < 103; i++) {
        log(to_string(i), 1, "This is a test log message.");
    }
    log("99", 1, "The tester is now about to test all log levels.");
    log("95", 1);
    log("98", 0);
    log("98", 1);
    log("98", 2);
    log("98", 3);
    log("98", 4);
    log("98", 5);
    log("96", 1);
    log("99", 1, "The tester will now create an instance of Xangine to test all features.");
    
    object obj;

    group obj_group;
    obj_group.objects.push_back(obj);

    scene obj_scene;
    obj_scene.groups.push_back(obj_group);

    interface obj_interface;
    obj_interface.scenes.push_back(obj_scene);
    obj_interface.coordinates_on_screen_x = 0;
    obj_interface.coordinates_on_screen_y = 0;
    obj_interface.interface_type = "3D";
    obj_interface.size_x = 800;
    obj_interface.size_y = 450;
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[0] = 5;  // X
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[1] = -5; // Y (negative = toward viewer)
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[2] = 3;  // Z (height)
    // Simplified rotation (just yaw and pitch):
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[0] = -20; // Slight downward pitch
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[1] = 0;  // Facing forward
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[2] = 0;  // No roll

    window default_window;
    default_window.name = "Xangine Test";
    default_window.is_main = true;
    default_window.size_x = 800;
    default_window.size_y = 450;
    default_window.interfaces.push_back(obj_interface);

    default_window = xgn::init(default_window);
    
    while (!default_window.done) {
        xgn::frame(default_window);
    }

    log("96", 1);
    log("3", 2);
    log("2", 3);
    log("1", 4);
    log("0", 5);
}
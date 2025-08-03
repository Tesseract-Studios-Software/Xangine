#include "./src/xgn_log/log.hpp"
#include "./src/xgn_core/core.hpp"
using namespace std;

int main() {
    int i;
    set_executable_dir(); // Set the working directory to the executable directory.
    log("0x1004", 1, "The tester will now create an instance of Xangine to test all features.");
    
    object obj;
    obj.obj_mesh.obj_file = "./src/xgn3D_resources/default_cube.obj";

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
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.aspect_ratio = 16.0 / 9.0;
    // Side view
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[0] = 10;  // X
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[1] = 10; // Y 
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[2] = 10;  // Z 
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[0] = 35;
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[1] = 0;
    obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[2] = 135;
    // Up to down view
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[0] = 5;  // X
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[1] = 0; // Y 
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.coordinates[2] = 10;  // Z 
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[0] = 90;
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[1] = 0;
    // obj_interface.scenes[obj_interface.scene_in_use].main_camera.rotation[2] = 0;

    window default_window;
    default_window.name = "Xangine Test";
    default_window.is_main = true;
    default_window.size_x = 800;
    default_window.size_y = 450;
    default_window.interfaces.push_back(obj_interface);

    default_window = xgn::init(default_window);
    int frames = 0;
    while (!default_window.done) {
        xgn::frame(default_window, 60);
        obj.rotation[2] += 1;
        frames++;
        cout << "Rendered " << frames << " frames." << endl;
    }

    log("96", 1);
    log("3", 2);
    log("2", 3);
    log("1", 4);
    log("0", 5);
}
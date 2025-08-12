#include "./src/xgn_log/log.hpp"
#include "./src/xgn_core/core.hpp"
using namespace std;

int main() {
    int i;

    log_filter = 0;

    set_executable_dir(); // Set the working directory to the executable directory.
    log("0x1004", 1, "The tester will now create an instance of Xangine to test all features.");
    
    log("0x1004", 0);

    xgn3D::object* obj = new xgn3D::object();
    obj->obj_mesh.obj_file = "./src/xgn3D_resources/default_cube.obj";
    obj->obj_material.metal = 128.0;
    obj->obj_material.emission = 1.0;

    xgn3D::group* obj_group = new xgn3D::group();
    obj_group->objects.push_back(obj);

    xgn3D::scene* obj_scene = new xgn3D::scene();
    obj_scene->groups.push_back(obj_group);
    xgn3D::camera* main_camera = new xgn3D::camera();

    xgn::interface* obj_interface = new xgn::interface();
    obj_interface->scenes.push_back(obj_scene);
    obj_interface->coordinates_on_screen_x = 0;
    obj_interface->coordinates_on_screen_y = 0;
    obj_interface->interface_type = "3D";
    obj_interface->size_x = 800;
    obj_interface->size_y = 450;

    main_camera->aspect_ratio = 16.0 / 9.0;
    main_camera->coordinates[0] = 10;  // X
    main_camera->coordinates[1] = 10; // Y 
    main_camera->coordinates[2] = 10;  // Z 
    main_camera->rotation[0] = 45;
    main_camera->rotation[1] = 0;
    main_camera->rotation[2] = 135;

    obj_scene->main_camera = main_camera;

    xgn::window* default_window = new xgn::window();
    default_window->name = "Xangine Test";
    default_window->is_main = true;
    default_window->size_x = 800;
    default_window->size_y = 450;
    default_window->interfaces.push_back(obj_interface);

    xgn::init(default_window);
    int frames = 0;
    while (!default_window->done) {
        auto [return_value, fps] = xgn::frame(default_window, 100);

        // Clear console output for next frame
        cout << "\033[2J\033[1;1H"; // ANSI escape code to clear console
        frames++;
        cout << "Frame:  " << frames << "\n" << "FPS: " << fps << endl;

        obj->coordinates[0] += 0.01;
        cout << obj->coordinates[0] << endl;
        obj->rotation[2] += 1;
        main_camera->rotation[2] += 1;
    }

    log("96", 1);
    log("3", 2);
    log("2", 3);
    log("1", 4);
    log("0", 5);
}
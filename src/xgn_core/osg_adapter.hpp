// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <xgn_log/log.hpp>
#include <xgn_container/container.hpp>
#include <xgn3D_object/object.hpp>
#include <xgn3D_camera/camera.hpp>
#include <xgnUI_key/keyboard.hpp>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/DisplaySettings>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Material>

namespace xgn {

// Setup the OSG root.
osg::ref_ptr<osg::Group> setup_root() {
    log("0x3001", 0);
    // Create root group
    osg::ref_ptr<osg::Group> root = new osg::Group;
    return root;
}

inline osg::ref_ptr<osg::PositionAttitudeTransform> create_object_transform(xgn3D::object*& obj) {
    log("0x3002", 0);
    osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform;
    
    // Set position using vec3D transform
    transform->setPosition(osg::Vec3d(
        obj->transform.x,
        obj->transform.y,
        obj->transform.z
    ));
    
    // Set rotation (convert degrees to radians)
    osg::Quat rotation;
    rotation.makeRotate(
        osg::DegreesToRadians(obj->transform.rotation_y), // yaw (Y)
        osg::Vec3d(0,1,0),                     // yaw axis
        osg::DegreesToRadians(obj->transform.rotation_x), // pitch (X)
        osg::Vec3d(1,0,0),                     // pitch axis
        osg::DegreesToRadians(obj->transform.rotation_z), // roll (Z)
        osg::Vec3d(0,0,1)                       // roll axis
    );
    transform->setAttitude(rotation);
    obj->osg_transform = transform;
    
    return transform;
}

// Load an object into the OSG root.
inline osg::ref_ptr<osg::Group> load_object_osg(xgn3D::object*& load_obj, osg::ref_ptr<osg::Group> root) {
    log("0x3003", 0);
    // Load the model
    osg::ref_ptr<osg::Node> loaded_model = osgDB::readNodeFile(load_obj->obj_mesh.obj_file);
    if (!loaded_model) {
        log("0x9002", 3, "Failed to load: " + load_obj->obj_mesh.obj_file);
        return root;
    }

    // Create transform node for position/rotation
    osg::ref_ptr<osg::PositionAttitudeTransform> transform = create_object_transform(load_obj);
    transform->addChild(loaded_model);

    // Apply material properties
    osg::StateSet* stateset = loaded_model->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    
    // Set material properties from xgn3D::material
    material->setAmbient(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj->obj_material.ambient[0],
                 load_obj->obj_material.ambient[1],
                 load_obj->obj_material.ambient[2],
                 1.0f));
    
    material->setDiffuse(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj->obj_material.diffuse[0],
                 load_obj->obj_material.diffuse[1],
                 load_obj->obj_material.diffuse[2],
                 1.0f));
    
    material->setSpecular(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj->obj_material.specular[0],
                 load_obj->obj_material.specular[1],
                 load_obj->obj_material.specular[2],
                 1.0f));
    
    material->setShininess(osg::Material::FRONT_AND_BACK, 
        load_obj->obj_material.metal);

    material->setEmission(osg::Material::FRONT_AND_BACK,
        osg::Vec4(load_obj->obj_material.emission,
                 load_obj->obj_material.emission,
                 load_obj->obj_material.emission,
                 1.0f));

    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON); 

    stateset->setAttribute(material);

    load_obj->loaded_material = material;
    load_obj->loaded_stateset = stateset;

    root->addChild(transform);

    return root;
}

// Setup the camera.
void setup_camera(xgn3D::camera*& xgn_camera, osg::ref_ptr<osgViewer::View> viewer) {
    log("0x3004", 0);
    osg::Camera* cam = viewer->getCamera();
    cam->setClearColor(osg::Vec4(
        xgn_camera->background_colour[0], 
        xgn_camera->background_colour[1], 
        xgn_camera->background_colour[2], 
        xgn_camera->background_colour[3]));
    
    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera->transform.rotation_x);
    double yaw = osg::DegreesToRadians(xgn_camera->transform.rotation_z);

    xgn::vec3D forward;
    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(yaw) * cos(pitch);
    forward.z = sin(pitch);
    forward = forward.normalize();

    // Calculate right vector (cross product of forward and world up)
    xgn::vec3D right;
    right.x = std::cos(yaw - 1.5708); // 90 degrees in radians
    right.y = std::sin(yaw - 1.5708);
    right.z = 0;
    right = right.normalize();

    // Calculate up vector (cross product of forward and right)
    xgn::vec3D up;
    up.x = forward.y * right.z - forward.z * right.y;
    up.y = forward.z * right.x - forward.x * right.z;
    up.z = forward.x * right.y - forward.y * right.x;
    up = up.normalize();

    osg::Vec3d eye(xgn_camera->transform.x, xgn_camera->transform.y, xgn_camera->transform.z);
    osg::Vec3d center = eye + osg::Vec3d(forward.x, forward.y, forward.z);
    // osg::Vec3d osg_up(up.x, up.y, up.z); // Use calculated up vector

    osg::Vec3d osg_up(0, 0, 1); // Z+ is up
    cam->setViewMatrixAsLookAt(eye, center, osg_up);
    
    // Set projection
    double fov = xgn_camera->fov;
    cam->setProjectionMatrixAsPerspective(
        fov, xgn_camera->aspect_ratio, 
        xgn_camera->clip_start, xgn_camera->clip_end
    );
    xgn_camera->osg_camera = cam;
}

// Setup the window
void setup_view_window(xgn::window*& window) {
    log("0x3005", 0);
    if (!window || !window->viewer) {
        log("0x9005", 3, "Null window or viewer in setup_view_window");
        return;
    }

    // Create graphics context traits
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = window->screen_x; // Window position X
    traits->y = window->screen_y; // Window position Y
    traits->width = window->size_x; // Window width
    traits->height = window->size_y; // Window height
    traits->windowDecoration = true; // Enable window decorations (title bar, borders)
    traits->doubleBuffer = true; // Enable double buffering
    traits->sharedContext = 0;
    traits->supportsResize = true; // Allow window resizing

    // Create graphics context
    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (!gc) {
        log("0x9004", 3);
        return;
    }

    // Configure camera
    osg::Camera* camera = window->viewer->getView(0)->getCamera();
    if (!camera) {
        log("0x9006", 3);
        return;
    }
    window->viewer->getView(0)->getCamera();
    camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // Realize viewer after camera is set up
    window->viewer->realize();
    osgViewer::Viewer::Windows windows;
    window->viewer->getWindows(windows);
    if (!windows.empty() && !window->name.empty()) {
        windows[0]->setWindowName(window->name);
    }
}

// void setup_view_interface(xgn::interface*& interface) {
//     log("0x3005", 0);
//     // Use the viewer already created for the interface (if any), or pass as parameter if needed
//     osg::ref_ptr<osgViewer::Viewer> viewer = interface->view;
//     if (!viewer) return; // Defensive: do nothing if viewer is not set
//     // Set up the embedded view
//     viewer->setUpViewerAsEmbeddedInWindow(interface->coordinates_on_screen_x, interface->coordinates_on_screen_y,
//                                           interface->size_x, interface->size_y);
//     viewer->home();
//     viewer->realize();
// }

void setup_objects(osg::ref_ptr<osg::Group> root, xgn::window*& loading_window) {
    log("0x3006", 0);
    for (auto& interface : loading_window->interfaces) {
        if (interface->interface_type == "3D") {
            auto scene = interface->scenes[interface->scene_in_use];
            for (auto& obj : scene->objects_loaded) {
                log("0x3007", 0);
                load_object_osg(obj, root);
            }
        }
    }
}

void update_camera_position(xgn3D::camera*& xgn_camera, osg::Camera* osg_camera) {
    if (!xgn_camera || !osg_camera) {
        log("0x9006", 3, "Null camera in update_camera_position");
        return;
    }

    // Update background color
    osg_camera->setClearColor(osg::Vec4(
        xgn_camera->background_colour[0],
        xgn_camera->background_colour[1],
        xgn_camera->background_colour[2],
        xgn_camera->background_colour[3]
    ));

    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera->transform.rotation_x);
    double yaw = osg::DegreesToRadians(xgn_camera->transform.rotation_z);
    
    // Calculate forward direction using vec3D for Z-up coordinate system
    xgn::vec3D forward;
    forward.x = std::cos(yaw) * std::cos(pitch);
    forward.y = std::sin(yaw) * std::cos(pitch);
    forward.z = std::sin(pitch);
    forward = forward.normalize();

    // Calculate right vector
    xgn::vec3D right;
    right.x = std::cos(yaw - 1.5708);
    right.y = std::sin(yaw - 1.5708);
    right.z = 0;
    right = right.normalize();

    // Calculate up vector
    xgn::vec3D up;
    up.x = forward.y * right.z - forward.z * right.y;
    up.y = forward.z * right.x - forward.x * right.z;
    up.z = forward.x * right.y - forward.y * right.x;
    up = up.normalize();

    // Set view matrix
    osg::Vec3d eye(
        xgn_camera->transform.x,
        xgn_camera->transform.y,
        xgn_camera->transform.z
    );
    osg::Vec3d center = eye + osg::Vec3d(forward.x, forward.y, forward.z);
    osg::Vec3d osg_up(up.x, up.y, up.z);

    osg_camera->setViewMatrixAsLookAt(eye, center, osg_up);

    // Update projection matrix
    osg_camera->setProjectionMatrixAsPerspective(
        xgn_camera->fov,
        xgn_camera->aspect_ratio,
        xgn_camera->clip_start,
        xgn_camera->clip_end
    );

    // Store the OSG camera reference
    xgn_camera->osg_camera = osg_camera;
}

void update_objects(xgn::window*& window) {
    for (auto& interface : window->interfaces) {
        if (interface->interface_type == "3D") {
            auto scene = interface->scenes[interface->scene_in_use];
            for (auto*& obj : scene->objects_loaded) {
                if (!obj->osg_transform) continue;
                
                // Apply material properties
                osg::StateSet* stateset = obj->loaded_stateset;
                osg::Material* material = obj->loaded_material;
                
                material->setAmbient(osg::Material::FRONT_AND_BACK, 
                    osg::Vec4(obj->obj_material.ambient[0],
                            obj->obj_material.ambient[1],
                            obj->obj_material.ambient[2],
                            1.0f));
                
                material->setDiffuse(osg::Material::FRONT_AND_BACK, 
                    osg::Vec4(obj->obj_material.diffuse[0],
                            obj->obj_material.diffuse[1],
                            obj->obj_material.diffuse[2],
                            1.0f));
                
                material->setSpecular(osg::Material::FRONT_AND_BACK, 
                    osg::Vec4(obj->obj_material.specular[0],
                            obj->obj_material.specular[1],
                            obj->obj_material.specular[2],
                            1.0f));
                
                material->setShininess(osg::Material::FRONT_AND_BACK, 
                    obj->obj_material.metal);

                material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
                stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON); 

                // Update position using vec3D transform
                obj->osg_transform->setPosition(osg::Vec3d(
                    obj->transform.x,
                    obj->transform.y,
                    obj->transform.z
                ));
                
                // Set rotation (convert degrees to radians)
                osg::Quat rotation;
                rotation.makeRotate(
                    osg::DegreesToRadians(obj->transform.rotation_y), // yaw (Y)
                    osg::Vec3d(0,1,0),                     // yaw axis
                    osg::DegreesToRadians(obj->transform.rotation_x), // pitch (X)
                    osg::Vec3d(1,0,0),                     // pitch axis
                    osg::DegreesToRadians(obj->transform.rotation_z), // roll (Z)
                    osg::Vec3d(0,0,1)                       // roll axis
                );
                
                obj->osg_transform->setAttitude(rotation);
            }
        }
    }
}

// Startup OSG.
window* setup_osg(window* win) {
    // 1. Create CompositeViewer and root node
    win->viewer = new osgViewer::CompositeViewer;
    osg::ref_ptr<osg::Group> root = setup_root();
    win->root = root;

    // 2. Configure graphics context (critical for window title and rendering)
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = win->screen_x;
    traits->y = win->screen_y;
    traits->width = win->size_x;
    traits->height = win->size_y;
    traits->windowDecoration = true;  // Must be true for title bar
    traits->doubleBuffer = true;
    traits->supportsResize = true;
    traits->windowName = win->name;   // Set window title here

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (!gc || !gc->valid()) {
        log("0x9004", 3, "Failed to create graphics context");
        return win;
    }
    gc->realize();  // Activate the context

    // 3. Load objects into the root node
    setup_objects(root, win);  // Must be called before creating views!

    // 4. Create views for each interface and assign cameras
    for (auto* interface : win->interfaces) {
        osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
        view->setSceneData(root);  // Link root to the view

        // Configure camera viewport (matches interface dimensions)
        osg::Camera* camera = view->getCamera();
        camera->setGraphicsContext(gc);
        camera->setViewport(
            interface->coordinates_on_screen_x,
            interface->coordinates_on_screen_y,
            interface->size_x,
            interface->size_y
        );

        // Set camera properties from the interface's scene
        if (interface->scenes.size() > 0) {
            auto* scene = interface->scenes[interface->scene_in_use];
            if (scene->main_camera) {
                setup_camera(scene->main_camera, view);  // Pass the view, not the interface's viewer
            }
        }

        win->viewer->addView(view);
        interface->view = view;
    }

    xgnUI::init_keyboard(win->interfaces[0]->view);

    return win;
}

};

#endif //OSG_ADAPT_HPP
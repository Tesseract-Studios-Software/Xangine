// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <xgn_log/log.hpp>
#include <xgn_container/container.hpp>
#include <xgn3D_object/object.hpp>
#include <xgn3D_camera/camera.hpp>
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
    
    // Set position
    transform->setPosition(osg::Vec3d(
        obj->coordinates[0],
        obj->coordinates[1],
        obj->coordinates[2]
    ));
    
    // Set rotation (convert degrees to radians)
    osg::Quat rotation;
    rotation.makeRotate(
        osg::DegreesToRadians(obj->rotation[1]), // yaw (Y)
        osg::Vec3d(0,1,0),                     // yaw axis
        osg::DegreesToRadians(obj->rotation[0]), // pitch (X)
        osg::Vec3d(1,0,0),                     // pitch axis
        osg::DegreesToRadians(obj->rotation[2]), // roll (Z)
        osg::Vec3d(0,0,1)                       // roll axis
    );
    transform->setAttitude(rotation);
    obj->transform = transform;
    
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
void setup_camera(xgn3D::camera*& xgn_camera, osg::ref_ptr<osgViewer::Viewer> viewer) {
    log("0x3004", 0);
    osg::Camera* cam = viewer->getCamera();
    cam->setClearColor(osg::Vec4(
        xgn_camera->background_colour[0], 
        xgn_camera->background_colour[1], 
        xgn_camera->background_colour[2], 
        xgn_camera->background_colour[3]));
    
    // Set initial view
    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera->rotation[0]);
    double roll  = osg::DegreesToRadians(xgn_camera->rotation[1]);
    double yaw   = osg::DegreesToRadians(xgn_camera->rotation[2]);

    int closer = 0;
    bool done = false;
    double rotation_z = xgn_camera->rotation[2];
    while (!done) {
        if (rotation_z < 360 && rotation_z > -360) {
            done = true;
        } else {
            rotation_z -= 360;
        }
    }


    osg::Vec3d forward;
    forward.z() = -sin(pitch);
    if ((rotation_z >= -45 && rotation_z < 45) || (rotation_z >= 135 && rotation_z < 225)) {
        forward.x() = -sin(yaw);
        forward.y() = cos(yaw);
    } else {
        forward.x() = cos(yaw);
        forward.y() = -sin(yaw);
    }

    osg::Vec3d eye(xgn_camera->coordinates[0], xgn_camera->coordinates[1], xgn_camera->coordinates[2]);
    osg::Vec3d center = eye + forward;
    osg::Vec3d up(0, 0, 1); // Z-up

    cam->setViewMatrixAsLookAt(eye, center, up);
    
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
    osg::Camera* camera = window->viewer->getCamera();
    if (!camera) {
        log("0x9006", 3);
        return;
    }
    window->viewer->setUpViewInWindow(window->screen_x, window->screen_y, window->size_x, window->size_y);
    camera->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

    // Realize viewer after camera is set up
    window->viewer->realize();
    osgViewer::Viewer::Windows windows;
    window->viewer->getWindows(windows);
    if (!windows.empty() && !window->name.empty()) {
        windows[0]->setWindowName(window->name);
    }
}

void setup_view_interface(xgn::interface*& interface) {
    log("0x3005", 0);
    // Use the viewer already created for the interface (if any), or pass as parameter if needed
    osg::ref_ptr<osgViewer::Viewer> viewer = interface->viewer;
    if (!viewer) return; // Defensive: do nothing if viewer is not set
    // Set up the embedded view
    viewer->setUpViewerAsEmbeddedInWindow(interface->coordinates_on_screen_x, interface->coordinates_on_screen_y,
                                          interface->size_x, interface->size_y);
    viewer->home();
    viewer->realize();
}

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

void update_camera_position(xgn3D::camera*& xgn_camera, osg::ref_ptr<osgViewer::Viewer> viewer) {
    xgn_camera->osg_camera = viewer->getCamera();
    viewer->getCamera()->setClearColor(osg::Vec4(
        xgn_camera->background_colour[0], 
        xgn_camera->background_colour[1], 
        xgn_camera->background_colour[2],
        xgn_camera->background_colour[3]
    ));
    
    // Set initial view
    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera->rotation[0]);
    double roll  = osg::DegreesToRadians(xgn_camera->rotation[1]);
    double yaw   = osg::DegreesToRadians(xgn_camera->rotation[2]);

    int closer = 0;
    bool done = false;
    double rotation_z = xgn_camera->rotation[2];
    while (!done) {
        if (rotation_z < 360 && rotation_z > -360) {
            done = true;
        } else {
            rotation_z -= 360;
        }
    }


    osg::Vec3d forward;
    forward.z() = -sin(pitch);
    if ((rotation_z >= -45 && rotation_z < 45) || (rotation_z >= 135 && rotation_z < 225)) {
        forward.x() = -sin(yaw);
        forward.y() = cos(yaw);
    } else {
        forward.x() = cos(yaw);
        forward.y() = -sin(yaw);
    }

    osg::Vec3d eye(xgn_camera->coordinates[0], xgn_camera->coordinates[1], xgn_camera->coordinates[2]);
    osg::Vec3d center = eye + forward;
    osg::Vec3d up(0, 0, 1); // Z-up

    viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);
    
    // Set projection
    double fov = xgn_camera->fov;
    viewer->getCamera()->setProjectionMatrixAsPerspective(
        fov, xgn_camera->aspect_ratio, 
        xgn_camera->clip_start, xgn_camera->clip_end
    );
}

void update_objects(xgn::window*& window) {
    for (auto& interface : window->interfaces) {
        if (interface->interface_type == "3D") {
            auto scene = interface->scenes[interface->scene_in_use];
            for (auto*& obj : scene->objects_loaded) {
                if (!obj->transform) continue;
                // Apply material properties
                osg::StateSet* stateset = obj->loaded_stateset;
                osg::Material* material = obj->loaded_material;
                // Set material properties from xgn3D::material
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

                obj->transform->setPosition(osg::Vec3d(
                    obj->coordinates[0],
                    obj->coordinates[1],
                    obj->coordinates[2]
                ));
                
                // Set rotation (convert degrees to radians)
                osg::Quat rotation;
                rotation.makeRotate(
                    osg::DegreesToRadians(obj->rotation[1]), // yaw (Y)
                    osg::Vec3d(0,1,0),                     // yaw axis
                    osg::DegreesToRadians(obj->rotation[0]), // pitch (X)
                    osg::Vec3d(1,0,0),                     // pitch axis
                    osg::DegreesToRadians(obj->rotation[2]), // roll (Z)
                    osg::Vec3d(0,0,1)                       // roll axis
                );
                
                obj->transform->setAttitude(rotation);

                // stateset->setAttribute(material);
            }
        }
    }
}

// Startup OSG.
window* setup_osg(window* win) {
    // Create viewer with proper traits
    win->viewer = new osgViewer::Viewer;
    
    // Create root node
    osg::ref_ptr<osg::Group> root = setup_root();
    
    // Setup window first
    setup_view_window(win);

    int i;
    for (i = 0; i < win->interfaces.size(); i++) {
        setup_view_interface(win->interfaces[i]);
    }
    
    // Setup camera after window is created
    for (auto& interface : win->interfaces) {
        setup_camera(interface->scenes[interface->scene_in_use]->main_camera, win->viewer);
    }
    
    // Setup objects
    setup_objects(root, win);
    
    // Set scene data
    win->viewer->setSceneData(root);
    win->root = root;
    
    return win;
}

};

#endif //OSG_ADAPT_HPP
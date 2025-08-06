#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/DisplaySettings>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Material>
// #include <osg/Geode>
// #include <osg/Node>
#include "../xgn_log/log.hpp"
#include "../xgn_container/container.hpp"
#include "../xgn3D_object/object.hpp"
#include "../xgn3D_camera/camera.hpp"


namespace xgn {

// Setup the OSG root.
osg::ref_ptr<osg::Group> setup_root() {
    log("0x3001", 0);
    // Create root group
    osg::ref_ptr<osg::Group> root = new osg::Group;
    return root;
}

inline osg::ref_ptr<osg::PositionAttitudeTransform> create_object_transform(xgn3D::object& obj) {
    log("0x3002", 0);
    osg::ref_ptr<osg::PositionAttitudeTransform> transform = new osg::PositionAttitudeTransform;
    
    // Set position
    transform->setPosition(osg::Vec3d(
        obj.coordinates[0],
        obj.coordinates[1], 
        obj.coordinates[2]
    ));
    
    // Set rotation (convert degrees to radians)
    osg::Quat rotation;
    rotation.makeRotate(
        osg::DegreesToRadians(obj.rotation[1]), // yaw (Y)
        osg::Vec3d(0,1,0),                     // yaw axis
        osg::DegreesToRadians(obj.rotation[0]), // pitch (X)
        osg::Vec3d(1,0,0),                     // pitch axis
        osg::DegreesToRadians(obj.rotation[2]), // roll (Z)
        osg::Vec3d(0,0,1)                       // roll axis
    );
    transform->setAttitude(rotation);
    obj.transform = transform;
    
    return transform;
}

// Load an object into the OSG root.
inline pair<osg::ref_ptr<osg::Group>, xgn3D::object> load_object_osg(xgn3D::object& load_obj, osg::ref_ptr<osg::Group> root) {
    log("0x3003", 0);
    // Load the model
    osg::ref_ptr<osg::Node> loaded_model = osgDB::readNodeFile(load_obj.obj_mesh.obj_file);
    if (!loaded_model) {
        log("0x9002", 3, "Failed to load: " + load_obj.obj_mesh.obj_file);
        return {root, load_obj};
    }

    // Create transform node for position/rotation
    osg::ref_ptr<osg::PositionAttitudeTransform> transform = create_object_transform(load_obj);
    transform->addChild(loaded_model);

    // Apply material properties
    osg::StateSet* stateset = loaded_model->getOrCreateStateSet();
    osg::Material* material = new osg::Material;
    
    // Set material properties from xgn3D::material
    material->setAmbient(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj.obj_material.ambient[0],
                 load_obj.obj_material.ambient[1],
                 load_obj.obj_material.ambient[2],
                 1.0f));
    
    material->setDiffuse(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj.obj_material.diffuse[0],
                 load_obj.obj_material.diffuse[1],
                 load_obj.obj_material.diffuse[2],
                 1.0f));
    
    material->setSpecular(osg::Material::FRONT_AND_BACK, 
        osg::Vec4(load_obj.obj_material.specular[0],
                 load_obj.obj_material.specular[1],
                 load_obj.obj_material.specular[2],
                 1.0f));
    
    material->setShininess(osg::Material::FRONT_AND_BACK, 
        load_obj.obj_material.metal);

    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateset->setMode(GL_NORMALIZE, osg::StateAttribute::ON); 

    stateset->setAttribute(material);

    root->addChild(transform);

    return {root, load_obj};
}

// Setup the camera.
xgn3D::camera setup_camera(xgn3D::camera xgn_camera, osg::ref_ptr<osgViewer::Viewer> viewer) {
    log("0x3004", 0);
    osg::Camera* cam = viewer->getCamera();
    
    // Set initial view
    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera.rotation[0]);
    double roll  = osg::DegreesToRadians(xgn_camera.rotation[1]);
    double yaw   = osg::DegreesToRadians(xgn_camera.rotation[2]);

    int closer = 0;
    bool done = false;
    double rotation_z = xgn_camera.rotation[2];
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

    osg::Vec3d eye(xgn_camera.coordinates[0], xgn_camera.coordinates[1], xgn_camera.coordinates[2]);
    osg::Vec3d center = eye + forward;
    osg::Vec3d up(0, 0, 1); // Z-up

    cam->setViewMatrixAsLookAt(eye, center, up);
    
    // Set projection
    double fov = xgn_camera.fov;
    cam->setProjectionMatrixAsPerspective(
        fov, xgn_camera.aspect_ratio, 
        xgn_camera.clip_start, xgn_camera.clip_end
    );
    xgn_camera.osg_camera = cam;
    return xgn_camera;
}

// Support for multiple interfaces in one window will come in future updates.
inline osg::ref_ptr<osgViewer::Viewer> setup_view(osg::ref_ptr<osgViewer::Viewer> viewer, xgn::interface interface, xgn::window window) {
    log("0x3005", 0);
    // Set up view with proper window dimensions
    viewer->setUpViewInWindow(100, 100, window.size_x, window.size_y);
    viewer->home();
    // Set window title
    viewer->realize();
    typedef osgViewer::Viewer::Windows Windows;
    Windows windows;
    viewer->getWindows(windows);
    for(osgViewer::Viewer::Windows::iterator itr = windows.begin(); itr != windows.end(); ++itr) {
        (*itr)->setWindowName(window.name);
    }
    return viewer;
}

void setup_objects(osg::ref_ptr<osg::Group> root, xgn::window& loading_window) {
    log("0x3006", 0);
    for (auto& interface : loading_window.interfaces) {
        if (interface.interface_type == "3D") {
            auto& scene = interface.scenes[interface.scene_in_use];
            for (auto& obj : scene.objects_loaded) {
                log("0x3007", 0);
                load_object_osg(obj, root);
            }
        }
    }
}

inline void update_camera_position(const xgn3D::camera& xgn_camera, osg::ref_ptr<osgViewer::Viewer> viewer) {
    osg::Camera* cam = viewer->getCamera();

    // Change view
    // Convert degrees to radians
    double pitch = osg::DegreesToRadians(xgn_camera.rotation[0]);
    double roll  = osg::DegreesToRadians(xgn_camera.rotation[1]);
    double yaw   = osg::DegreesToRadians(xgn_camera.rotation[2]);

    int closer = 0;
    bool done = false;
    double rotation_z = xgn_camera.rotation[2];
    osg::Vec3d forward;
    forward.z() = -sin(pitch);
    if ((rotation_z >= -45 && rotation_z < 45) || (rotation_z >= 135 && rotation_z < 225)) {
        forward.x() = -sin(yaw);
        forward.y() = cos(yaw);
    } else {
        forward.x() = cos(yaw);
        forward.y() = -sin(yaw);
    }

    osg::Vec3d eye(xgn_camera.coordinates[0], xgn_camera.coordinates[1], xgn_camera.coordinates[2]);
    osg::Vec3d center = eye + forward;
    osg::Vec3d up(0, 0, 1); // Z-up

    cam->setViewMatrixAsLookAt(eye, center, up);
    
    // Set projection
    double fov = xgn_camera.fov;
    cam->setProjectionMatrixAsPerspective(
        fov, xgn_camera.aspect_ratio, 
        xgn_camera.clip_start, xgn_camera.clip_end
    );
}

xgn::window update_objects(xgn::window& window) {
    for (auto& interface : window.interfaces) {
        if (interface.interface_type != "3D") continue;
        
        auto& scene = interface.scenes[interface.scene_in_use];
        for (auto& obj : scene.objects_loaded) {
            if (!obj.transform) continue;
            // window.root = load_object_osg(obj, window.root);
        }
    }
    return window;
}

// Startup OSG.
window setup_osg(window& win) {
    // Create viewer first
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    osg::ref_ptr<osg::Group> root = new osg::Group;
    
    // Setup view before camera
    setup_view(viewer, win.interfaces[0], win);
    
    // Then setup camera
    win.interfaces[0].scenes[win.interfaces[0].scene_in_use].main_camera = setup_camera(win.interfaces[0].scenes[win.interfaces[0].scene_in_use].main_camera, viewer);
    
    // Setup objects
    setup_objects(root, win);
    
    // Critical: Set scene data
    viewer->setSceneData(root);

    win.viewer = viewer;
    win.root = root;
    
    return win;
}

};

#endif //OSG_ADAPT_HPP
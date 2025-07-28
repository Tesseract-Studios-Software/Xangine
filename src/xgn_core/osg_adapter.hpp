#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osgDB/ReadFile>
#include <osg/PositionAttitudeTransform>
#include <osg/DisplaySettings>
#include <osgViewer/ViewerEventHandlers>
// #include <osg/Geode>
// #include <osg/Node>
#include "../xgn_log/log.hpp"
#include "../xgn_container/container.hpp"
#include "../xgn3D_object/object.hpp"
#include "../xgn3D_camera/camera.hpp"


namespace xgn {

// Setup the OSG root.
osg::ref_ptr<osg::Group> setup_root() {
    // Create root group
    osg::ref_ptr<osg::Group> root = new osg::Group;
    return root;
}

// Load an object into the OSG root.
inline osg::ref_ptr<osg::Group> load_object_osg(xgn3D::object load_obj, osg::ref_ptr<osg::Group> root) {
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(load_obj.obj_mesh.obj_file);
    root->addChild(loadedModel);
    return root;
}

// Setup the camera.
inline osg::Camera* setup_camera(xgn3D::camera xgn_camera, osgViewer::Viewer viewer) {
    osg::Camera* osg_camera = viewer.getCamera();
    osg_camera->setProjectionMatrixAsPerspective(0, 0, 0, 1000);
    int focal_length = xgn_camera.focal_length;
    int clip_start = xgn_camera.clip_start;
    int clip_end = xgn_camera.clip_end;
    double aspect_ratio = xgn_camera.aspect_ratio;
    double halfFov = 2.0f * atanf((1.0f / (2.0f * focal_length)) / aspect_ratio);
    double fov = 2.0f * halfFov;

    // Set the projection matrix as perspective.
    osg_camera->setProjectionMatrixAsPerspective(fov, aspect_ratio, clip_start, clip_end);
    return osg_camera;
}

// Support for multiple interfaces in one window will come in future updates.
osgViewer::Viewer setup_view(osgViewer::Viewer viewer, xgn::interface interface, xgn::window window) {
    // Set up view with proper window dimensions
    viewer.setUpViewInWindow(100, 100, window.size_x, window.size_y);
    
    // Set window title
    viewer.realize();
    typedef osgViewer::Viewer::Windows Windows;
    Windows windows;
    viewer.getWindows(windows);
    if (!windows.empty()) {
        windows[0]->setWindowName(window.name);
        
        // For macOS specifically, may need to force refresh
        #ifdef __APPLE__
        windows[0]->requestWarpPointer(window.size_x/2, window.size_y/2);
        #endif
    }
    return viewer;
}

inline osg::ref_ptr<osg::PositionAttitudeTransform> create_object_transform(const xgn3D::object& obj) {
    auto transform = new osg::PositionAttitudeTransform;
    
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
    
    return transform;
}

void setup_objects(osg::ref_ptr<osg::Group> root, xgn::window& loading_window) {
    for (auto& interface : loading_window.interfaces) {
        if (interface.interface_type != "3D") continue;
        
        auto& scene = interface.scenes[interface.scene_in_use];
        for (auto& obj : scene.objects_loaded) {
            osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(obj.obj_mesh.obj_file);
            if (!node) {
                log("101", 3, "Failed to load: " + obj.obj_mesh.obj_file);
                continue;
            }
            
            auto transform = create_object_transform(obj);
            transform->addChild(node);
            root->addChild(transform);
            
            // Store the transform reference in the object for later updates
            obj.transform = transform; // You'll need to add this to your object struct
        }
    }
}

inline void update_camera_position(osg::Camera* osg_camera, const xgn3D::camera& xgn_camera) {
    osg::Matrixd view_matrix;
    
    // Create rotation from Euler angles (pitch, yaw, roll)
    osg::Quat rot;
    rot.makeRotate(
        osg::DegreesToRadians(xgn_camera.rotation[1]), // yaw (Y)
        osg::Vec3d(0,1,0),                             // yaw axis
        osg::DegreesToRadians(xgn_camera.rotation[0]), // pitch (X)
        osg::Vec3d(1,0,0),                             // pitch axis
        osg::DegreesToRadians(xgn_camera.rotation[2]), // roll (Z)
        osg::Vec3d(0,0,1)                              // roll axis
    );
    
    // Set view matrix
    view_matrix.makeLookAt(
        osg::Vec3d(xgn_camera.coordinates[0], xgn_camera.coordinates[1], xgn_camera.coordinates[2]), // eye
        osg::Vec3d(xgn_camera.coordinates[0], xgn_camera.coordinates[1], xgn_camera.coordinates[2] - 1), // center
        osg::Vec3d(0,1,0)  // up
    );
    
    view_matrix.preMultRotate(rot);
    osg_camera->setViewMatrix(view_matrix);
}

void update_objects(xgn::window& window) {
    for (auto& interface : window.interfaces) {
        if (interface.interface_type != "3D") continue;
        
        auto& scene = interface.scenes[interface.scene_in_use];
        for (auto& obj : scene.objects_loaded) {
            if (!obj.transform) continue;
            
            // Update position
            obj.transform->setPosition(osg::Vec3d(
                obj.coordinates[0],
                obj.coordinates[1],
                obj.coordinates[2]
            ));
            
            // Update rotation
            osg::Quat rotation;
            rotation.makeRotate(
                osg::DegreesToRadians(obj.rotation[1]), // yaw
                osg::Vec3d(0,1,0),
                osg::DegreesToRadians(obj.rotation[0]), // pitch
                osg::Vec3d(1,0,0),
                osg::DegreesToRadians(obj.rotation[2]), // roll
                osg::Vec3d(0,0,1)
            );
            obj.transform->setAttitude(rotation);
        }
    }
}

osg::ref_ptr<osg::Geode> create_axes() {
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(1,0,0), 0.1f, 2.0f))); // X (Red)
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,1,0), 0.1f, 2.0f))); // Y (Green)
    geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0,0,1), 0.1f, 2.0f))); // Z (Blue)
    return geode;
}

// Startup OSG.
std::pair<osg::ref_ptr<osgViewer::Viewer>, osg::ref_ptr<osg::Group>> setup_osg(window& win) {
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
    osg::ref_ptr<osg::Group> root = new osg::Group;
    
    // Configure camera
    setup_camera(win.interfaces[0].scenes[win.interfaces[0].scene_in_use].main_camera, *viewer);
    
    // Setup view
    viewer->setUpViewInWindow(100, 100, win.size_x, win.size_y);
    viewer->realize();
    
    // Load objects
    setup_objects(root, win);
    
    viewer->setSceneData(root);
    return {viewer, root};
}

};

#endif //OSG_ADAPT_HPP
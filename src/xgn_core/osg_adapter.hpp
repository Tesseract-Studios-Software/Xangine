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

osgViewer::Viewer setup_osg() {
    // OSG setup
    osgViewer::Viewer viewer;
    return viewer;
}

osg::ref_ptr<osg::Group> setup_root() {
    // Create root group
    osg::ref_ptr<osg::Group> root = new osg::Group;
    return root;
}

inline osg::ref_ptr<osg::Group> load_object_osg(xgn3D::object load_obj, osg::ref_ptr<osg::Group> root) {
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(load_obj.obj_mesh.obj_file);
    root->addChild(loadedModel);
    return root;
}
inline osg::Camera* setup_camera(xgn3D::camera xgn_camera, osgViewer::Viewer viewer) {
    osg::Camera* osg_camera = viewer.getCamera();
    osg_camera->setProjectionMatrixAsPerspective(0, 0, 0, 1000);
    // Calculate the field of view (FOV) in radians.
    int focal_length = xgn_camera.focal_length;
    int clip_start = xgn_camera.clip_start;
    int clip_end = xgn_camera.clip_end;
    double aspect_ratio = xgn_camera.aspect_ratio;
    double halfFov = 2.0f * atanf((1.0f / (2.0f * focal_length)) / aspect_ratio);
    double fov = 2.0f * halfFov;

    // Set the projection matrix as perspective.
    osg_camera->setProjectionMatrixAsPerspective(fov, aspect_ratio, clip_start, clip_end);
}

osgViewer::Viewer setup_view(osgViewer::Viewer viewer, xgn::interface Interface) {
    viewer.setUpViewInWindow(Interface.size_x, Interface.size_y, Interface.coordinates_on_screen_x, Interface.coordinates_on_screen_y);
    return viewer;
}

void setup_objects(osg::ref_ptr<osg::Group> root, xgn3D::object current_object, xgn::window loading_window) {
    int i, j = 0;
    for (i = 0; i < loading_window.interfaces.size(); i++) {
        if (loading_window.interfaces[i].interface_type == "3D") {
            for (j = 0; j < loading_window.interfaces[i].scenes[loading_window.interfaces[i].scene_in_use].objects_loaded.size(); j++) {
                current_object = loading_window.interfaces[i].scenes[loading_window.interfaces[i].scene_in_use].objects_loaded[j];
                // 3D object handling.
                osg::ref_ptr<osg::PositionAttitudeTransform> transforms[loading_window.interfaces[i].scenes[loading_window.interfaces[i].scene_in_use].objects_loaded.size()];

                root = load_object_osg(current_object, root);

                // Create transforms
                transforms[j] = new osg::PositionAttitudeTransform;
                root->addChild(transforms[j]);
                transforms[j]->addChild(osgDB::readNodeFile(current_object.obj_mesh.obj_file));
            }
        }
    }
}

};

#endif //OSG_ADAPT_HPP
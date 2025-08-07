#ifndef SCENE_HPP
#define SCENE_HPP

#include "../xgn3D_group/group.hpp"
#include "../xgn_log/log.hpp"
#include "../xgn3D_camera/camera.hpp"

namespace xgn3D {

// Scenes contain groups.
struct scene {
    vector<xgn3D::group*> groups;
    xgn3D::camera* main_camera;
    vector<xgn3D::object*> objects_loaded; // Manual loading of objects is required. Only objects in this vector will be rendered.
};

};

#endif // SCENE_HPP
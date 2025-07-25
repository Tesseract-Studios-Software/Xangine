#ifndef SCENE_HPP
#define SCENE_HPP

#include "../xgn3D_group/group.hpp"
#include "../xgn_log/log.hpp"

namespace xgn3D {

// Scenes contain groups.
struct scene {
    vector<xgn3D::group> groups;
};

};

#endif // SCENE_HPP
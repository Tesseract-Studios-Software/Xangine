#ifndef SCENE_HPP
#define SCENE_HPP

#include "../xgn3D_group/group.hpp"
#include "../xgn_log/log.hpp"

namespace xgn3D {

// Scenes contain groups, each scene can hold up to 1024 groups.
struct scene {
    xgn3D::group groups[1024];
    bool is_new = true;
    int data_count = 0;
};

};

#endif // SCENE_HPP
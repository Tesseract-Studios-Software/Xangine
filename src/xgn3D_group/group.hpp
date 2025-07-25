#ifndef GROUP_HPP
#define GROUP_HPP

#include "../xgn3D_object/object.hpp"
#include "../xgn_log/log.hpp"

namespace xgn3D {

// Groups contain 3D objects, each group can hold up to 1048576 objects.
struct group {
    xgn3D::object objects[1048576];
    bool is_new = true;
    int data_count = 0;
};

};

#endif // GROUP_HPP
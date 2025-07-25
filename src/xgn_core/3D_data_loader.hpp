#ifndef DATA_LOADER_HPP
#define DATA_LOADER_HPP

#include "../xgn_container/container.hpp"
#include "../xgn3D_object/object.hpp"
#include "../xgn3D_datablock/datablock.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <vector>


using namespace std;

namespace xgn {
// This loads the object inputted as the argument.
inline xgn3D::object load_data(xgn3D::object& load) {
    return load;
}

// This loads all objects in an interface, it is not suggested to call this repeatively or else it may cause lag.
vector<xgn3D::object> load_all_data(interface& load_interface) {
    int k, l, m, o;
    vector<xgn3D::object> obj_vector;
    for (k = 0; k < load_interface.scenes.size(); k++) {
        for (l = 0; l < load_interface.scenes[k].groups.size(); l++) {
            for (m = 0; m < load_interface.scenes[k].groups[l].objects.size(); m++) {
                obj_vector.push_back(load_data(load_interface.scenes[k].groups[l].objects[m]));
            }
        }
    }
    return obj_vector;
}
};
#endif // DATA_LOADER_HPP
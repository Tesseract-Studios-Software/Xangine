#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "../xgn3D_datablock/datablock.hpp"
#include "../xgn3D_datablock/material.hpp"
#include "../xgn3D_datablock/mesh.hpp"
#include "../xgn_log/log.hpp"
#include "../xgn_core/osg_adapter.hpp"
#include <osg/ShapeDrawable>

namespace xgn3D {

// Each object has 3D coordinates, a mesh, and a material.
struct object {
    double coordinates[3] = {0, 0, 0};
    xgn3D::mesh obj_mesh;
    xgn3D::material obj_material;
    bool is_new = true;
    osg::ref_ptr<osg::Geode> geode;
};

int create_osg_object(xgn3D::object loading_obj) {
    xgn::load_object_osg(loading_obj);
}

};

#endif // OBJECT_HPP

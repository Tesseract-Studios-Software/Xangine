// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <xgn3D_datablock/datablock.hpp>
#include <xgn3D_datablock/material.hpp>
#include <xgn3D_datablock/mesh.hpp>
#include <xgn_log/log.hpp>
#include <osg/Geode>
#include <osg/Material>

namespace xgn3D {

// Each object has 3D coordinates, a mesh, and a material.
struct object {
    double coordinates[3] = {0, 0, 0};
    double rotation[3] = {0, 0, 0};
    xgn3D::mesh obj_mesh;
    xgn3D::material obj_material;
    osg::ref_ptr<osg::Node> loaded_model;
    osg::Material* loaded_material;
    osg::StateSet* loaded_stateset;
    osg::ref_ptr<osg::PositionAttitudeTransform> transform = nullptr;
};

};

#endif // OBJECT_HPP

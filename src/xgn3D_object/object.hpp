// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <xgn3D_datablock/datablock.hpp>
#include <xgn3D_datablock/material.hpp>
#include <xgn3D_datablock/mesh.hpp>
#include <xgn_log/log.hpp>
#include <xgn_vector/vector3D.hpp>
#include <osg/Geode>
#include <osg/Material>

namespace xgn3D {

// Each object has 3D coordinates, a mesh, and a material.
struct object {
    xgn3D::mesh obj_mesh;
    xgn3D::material obj_material;
    osg::ref_ptr<osg::Node> loaded_model;
    osg::Material* loaded_material;
    osg::StateSet* loaded_stateset;
    xgn::vec3D transform;
    xgn::vec3D movement;
    osg::ref_ptr<osg::PositionAttitudeTransform> osg_transform = nullptr;
    
    // Convenience accessors for coordinates and rotation
    double& x() { return transform.x; }
    double& y() { return transform.y; }
    double& z() { return transform.z; }
    double& rotation_x() { return transform.rotation_x; }
    double& rotation_y() { return transform.rotation_y; }
    double& rotation_z() { return transform.rotation_z; }
    
    // Const versions
    const double& x() const { return transform.x; }
    const double& y() const { return transform.y; }
    const double& z() const { return transform.z; }
    const double& rotation_x() const { return transform.rotation_x; }
    const double& rotation_y() const { return transform.rotation_y; }
    const double& rotation_z() const { return transform.rotation_z; }
};

};

#endif // OBJECT_HPP
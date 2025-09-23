// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef OBJECT_HPP
#define OBJECT_HPP

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
    xgn::vec3D local;
    osg::ref_ptr<osg::PositionAttitudeTransform> osg_transform = nullptr;

    // Parenting system
    object* parent = nullptr;
    xgn::vec3D local_position;    // Position relative to parent
    xgn::vec3D local_rotation;    // Rotation relative to parent
    xgn::vec3D world_position;    // Calculated world position
    xgn::vec3D world_rotation;    // Calculated world rotation
    
    // Method to update world transform based on parent
    void update_transform() {
        if (parent) {
            // Calculate world position based on parent's transform
            world_position = calculate_parent(parent->world_position, local_position);
            
            // Calculate world rotation (add local rotation to parent's rotation)
            world_rotation.rotation_x = parent->world_rotation.rotation_x + local_rotation.rotation_x;
            world_rotation.rotation_y = parent->world_rotation.rotation_y + local_rotation.rotation_y;
            world_rotation.rotation_z = parent->world_rotation.rotation_z + local_rotation.rotation_z;
            
            // Apply to the main transform
            transform.x = world_position.x;
            transform.y = world_position.y;
            transform.z = world_position.z;
            transform.rotation_x = world_rotation.rotation_x;
            transform.rotation_y = world_rotation.rotation_y;
            transform.rotation_z = world_rotation.rotation_z;
        } else {
            // No parent, use local values directly
            transform.x = local_position.x;
            transform.y = local_position.y;
            transform.z = local_position.z;
            transform.rotation_x = local_rotation.rotation_x;
            transform.rotation_y = local_rotation.rotation_y;
            transform.rotation_z = local_rotation.rotation_z;
        }
    }
    
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
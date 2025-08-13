// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "../xgn3D_scene/scene.hpp"
#include "../xgn_log/log.hpp"
#include <osgViewer/Viewer>
#include <vector>

namespace xgn {
// Interfaces contain 3D scenes.
struct interface {
    vector<xgn3D::scene*> scenes;
    int scene_in_use = 0; // Specify the scene to be rendered in the interface here.
    int coordinates_on_screen_x = 0; // X-axis location of the left bottom corner of the interface.
    int coordinates_on_screen_y = 0; // Y-axis location of the left bottom corner of the interface.
    int size_x = 100; // X-axis length of the interface.
    int size_y = 100; // Y-axis length of the interface.
    string interface_type = "3D"; // 3D / UI
    osg::ref_ptr<osgViewer::Viewer> viewer;
};

};

#endif // INTERFACE_HPP
// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../xgn_interface/interface.hpp"
#include "../xgn_log/log.hpp"
#include <osgViewer/Viewer>
#include <vector>

namespace xgn {

// Windows contain interfaces.
struct window {
    vector<xgn::interface*> interfaces;
    int size_x = 800;
    int size_y = 450;
    int screen_x = 100;
    int screen_y = 100;
    bool is_main = false;
    string name = "Xangine Window";
    bool done = false;
    
    osg::ref_ptr<osgViewer::Viewer> viewer;
    osg::ref_ptr<osg::Group> root;
};

};

#endif // WINDOW_HPP
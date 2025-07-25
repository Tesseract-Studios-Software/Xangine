#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Drawable>
#include <osgDB/ReadFile>
// #include <osg/Geode>
// #include <osg/Node>
#include "../xgn3D_object/object.hpp"
#include "./core.hpp"

namespace xgn {

osgViewer::Viewer setup_osg() {
    // OSG setup
    osgViewer::Viewer viewer;
    return viewer;
}

osg::ref_ptr<osg::Group> setup_root() {
    // Create root group
    osg::ref_ptr<osg::Group> root = new osg::Group;
    return root;
}

inline xgn3D::object load_object_osg(xgn3D::object load_obj, osg::ref_ptr<osg::Group> root) {
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(load_obj.obj_mesh.obj_file);
    root->addChild(loadedModel);
}

};

#endif //OSG_ADAPT_HPP
#ifndef OSG_ADAPT_HPP
#define OSG_ADAPT_HPP

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>
#include <osg/Geode>
#include <osg/Node>
#include "../xgn3D_object/object.hpp"
#include "../xgn_core/core.hpp"

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

int load_object_osg(xgn3D::object load_obj) {
    load_obj.geode = new osg::Geode; 
    osg::ref_ptr<osg::ShapeDrawable> sd1 = new osg::ShapeDrawable(osgDB::readNodeFile(load_obj.obj_mesh.obj_file));
    sd1->setColor(osg::Vec4(1.0, 1.0, 0.0, 1.0));
    load_obj.geode->addDrawable(sd1);
    sd1->setUseDisplayList(false);
    sd1->setUseVertexBufferObjects(false);
}

};

#endif //OSG_ADAPT_HPP
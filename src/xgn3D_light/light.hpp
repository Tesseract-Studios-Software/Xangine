// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#include <osg/Light>
#include <osg/ref_ptr>


namespace xgn3D {

// Coming in future updates.
struct light {
    osg::ref_ptr<osg::Light> osg_light;
    float colour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    double brightness = 1.0;
};

}
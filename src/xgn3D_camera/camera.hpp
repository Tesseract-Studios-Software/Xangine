// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace xgn3D {

struct camera {
    int type = 0; // 0 = Perspective; 1 = Othographic
    double fov = 30; // Perspecive only
    double clip_start = 0.01; // Unit = m
    double clip_end   = 100; // Unit = m
    double size = 8; // Orthographic only; Width of camera (Horizontal); Unit = m
    double aspect_ratio = 16.0 / 9.0;
    double coordinates[3] = {0, 0, 0};
    double rotation[3] = {0, 0, 0};
    float background_colour[4] = {0.0f, 0.0f, 0.0f, 1.0f}; // The background colour of the camera
    osg::Camera* osg_camera;
};

}

#endif // CAMERA_HPP
#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace xgn3D {

struct camera {
    int type = 0; // 0 = Perspective; 1 = Othographic
    double focal_length = 50; // Perspecive only; Unit = mm
    double clip_start = 0.01; // Unit = m
    double clip_end   = 100; // Unit = m
    double size = 8; // Orthographic only; Width of camera (Horizontal); Unit = m
    double aspect_ratio = 16.0 / 9.0;
    double coordinates[3] = {0, 0, 0};
    double rotation[3] = {0, 0, 0};
};

}

#endif // CAMERA_HPP
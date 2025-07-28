#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace xgn3D {

struct camera {
    int type = 0; // 0 = Perspective; 1 = Othographic
    int focal_length = 50; // Perspecive only; Unit = mm
    int clip_start = 0.1; // Unit = m
    int clip_end   = 1000; // Unit = m
    int size = 8; // OOrthographic only; Width of camera (Horizontal); Unit = m
    double aspect_ratio = 16 / 9;
};

}

#endif // CAMERA_HPP
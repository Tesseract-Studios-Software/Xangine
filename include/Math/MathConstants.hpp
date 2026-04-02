#pragma once

#include <cmath>

namespace Xangine {

// Mathematical constants
template<typename T = float>
struct MathConstants {
    static constexpr T PI = T(3.14159265358979323846);
    static constexpr T TWO_PI = T(2.0 * 3.14159265358979323846);
    static constexpr T HALF_PI = T(1.57079632679489661923);
    static constexpr T EPSILON = T(1e-6);
    static constexpr T INFINITY = std::numeric_limits<T>::infinity();
    
    // Conversion helpers
    static constexpr T toRadians(T degrees) {
        return degrees * PI / T(180.0);
    }
    
    static constexpr T toDegrees(T radians) {
        return radians * T(180.0) / PI;
    }
    
    // Clamping
    static constexpr T clamp(T value, T min, T max) {
        return value < min ? min : (value > max ? max : value);
    }
    
    // Smooth interpolation
    static T smoothStep(T edge0, T edge1, T t) {
        t = clamp((t - edge0) / (edge1 - edge0), T(0), T(1));
        return t * t * (T(3) - T(2) * t);
    }
};

// Common float constants
using Math = MathConstants<float>;
using Mathd = MathConstants<double>;

} // namespace Xangine
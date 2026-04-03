#pragma once

namespace Xangine {

struct Colour {
    float r, g, b, a;
    
    Colour() : r(0), g(0), b(0), a(1) {}
    Colour(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
    
    static Colour Red()   { return Colour(1.0f, 0.0f, 0.0f); }
    static Colour Green() { return Colour(0.0f, 1.0f, 0.0f); }
    static Colour Blue()  { return Colour(0.0f, 0.0f, 1.0f); }
    static Colour Black() { return Colour(0.0f, 0.0f, 0.0f); }
    static Colour White() { return Colour(1.0f, 1.0f, 1.0f); }
    static Colour Grey()  { return Colour(0.5f, 0.5f, 0.5f); }
};

} // namespace Xangine
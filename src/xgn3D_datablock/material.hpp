#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
using namespace std;

namespace xgn3D {

struct material {
    // Default material properties
    string name = "default";
    double ambient[3] = {0.2f, 0.2f, 0.2f}; // Ambient colour
    double diffuse[3] = {0.8f, 0.8f, 0.8f}; // Diffuse colour
    double specular[3] = {1.0f, 1.0f, 1.0f}; // Specular colour
    double metal = 32.0f; // Metal (shininess) factor
    double emission = 0.0f; // Emission factor
    double emissive[3] = {0.0f, 0.0f, 0.0f}; // Emissive colour
    double transparency = 1.0f; // Transparency (1.0 = opaque, 0.0 = fully transparent)
    string texture_file; // Texture file path
    string normal_map_file; // Normal map file path
    string specular_map_file; // Specular map file path
    string displacement_map_file; // Displacement map file path
    string bump_map_file; // Bump map file path
    string emissive_map_file; // Emissive map file path
};

};

#endif // MATERIAL_HPP
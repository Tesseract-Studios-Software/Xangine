// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <fstream>
#include "../xgn_log/log.hpp"
using namespace std;

namespace xgn3D {

// Meshes are 3D datablocks with vertices, edges, and faces.
struct mesh {
    // Default .obj cube mesh
    string obj_file = "./src/xgn3D_resources/default_cube.obj"; // Default mesh file
};

};

#endif // MESH_HPP
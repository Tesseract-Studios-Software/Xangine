// Xangine is licensed under MIT License
// Copyright (c) 2025 Tesseract Studios

#ifndef GROUP_HPP
#define GROUP_HPP

#include "../xgn3D_object/object.hpp"
#include "../xgn_log/log.hpp"

namespace xgn3D {

// Groups contain 3D objects.
struct group {
    vector<xgn3D::object*> objects;
};

};

#endif // GROUP_HPP
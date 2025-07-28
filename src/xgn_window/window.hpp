#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../xgn_interface/interface.hpp"
#include "../xgn_log/log.hpp"
#include <vector>

namespace xgn {

// Windows contain interfaces.
struct window {
    vector<xgn::interface> interfaces;
    int size_x = 800;
    int size_y = 450;
    bool is_main = false;
    string name = "Tesseract Xangine Default Window";
};

};

#endif // WINDOW_HPP
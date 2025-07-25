#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_log/log.hpp"
#include <vector>

// Containers contain windows.
struct container {
    vector<xgn::window> windows;
    bool is_new = true;
    int data_count = 0;
};

#endif // CONTAINER_HPP
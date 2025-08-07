#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "../xgn_window/window.hpp"
#include "../xgn_log/log.hpp"
#include <vector>

// Containers contain windows.
struct container {
    vector<xgn::window*> windows;
};

#endif // CONTAINER_HPP
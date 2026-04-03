#pragma once

#include "Platform.hpp"

#ifdef XANGINE_PLATFORM_MACOS
    #include <GLFW/glfw3.h>
    
#elif defined(XANGINE_PLATFORM_WINDOWS)
    #include <GLFW/glfw3.h>
    
#elif defined(XANGINE_PLATFORM_LINUX)
    #include <GLFW/glfw3.h>
#endif
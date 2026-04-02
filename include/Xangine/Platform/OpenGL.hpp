#pragma once

#include "Platform.hpp"

// macOS requires frameworks to be imported differently
#ifdef XANGINE_PLATFORM_MACOS
    // macOS uses frameworks, not traditional includes
    #include <OpenGL/gl.h>
    #include <OpenGL/OpenGL.h>
    #include <GLFW/glfw3.h>
    
    // GLEW is problematic on macOS, use glad instead or skip
    #define GL_GLEXT_PROTOTYPES
    
#elif defined(XANGINE_PLATFORM_WINDOWS)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    
#elif defined(XANGINE_PLATFORM_LINUX)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif
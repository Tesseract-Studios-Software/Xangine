#pragma once

#include "Platform.hpp"

// macOS requires frameworks to be imported differently
#ifdef XANGINE_PLATFORM_MACOS
    // macOS uses frameworks, not traditional includes
    #import <OpenGL/gl.h>
    #import <OpenGL/OpenGL.h>
    #import <GLFW/glfw3.h>
    
    // GLEW is problematic on macOS, use glad instead or skip
    #define GL_GLEXT_PROTOTYPES
    
#elif defined(XANGINE_PLATFORM_WINDOWS)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    
#elif defined(XANGINE_PLATFORM_LINUX)
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
#endif
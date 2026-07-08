// include/Xangine/Platform/OpenGL.hpp
#pragma once

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#include "Platform.hpp"

#ifdef XANGINE_PLATFORM_MACOS
    #include <OpenGL/gl3.h>
#elif defined(XANGINE_PLATFORM_WINDOWS)
    #include <GL/glew.h>
#elif defined(XANGINE_PLATFORM_LINUX)
    #include <GL/glew.h>
#endif
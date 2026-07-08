// Platform.hpp
#pragma once

#ifndef XANGINE_PLATFORM_DETECTED

#ifdef _WIN32
    #define XANGINE_PLATFORM_WINDOWS
    #define XANGINE_PLATFORM_NAME "Windows"
    #define XANGINE_PLATFORM_DETECTED
    
#elif __APPLE__
    #include <TargetConditionals.h>
    #define XANGINE_PLATFORM_APPLE
    
    #if TARGET_OS_MAC
        #define XANGINE_PLATFORM_MACOS
        #define XANGINE_PLATFORM_NAME "macOS"
    #elif TARGET_OS_IPHONE
        #define XANGINE_PLATFORM_IOS
        #define XANGINE_PLATFORM_NAME "iOS"
    #endif
    #define XANGINE_PLATFORM_DETECTED
    
#elif __linux__
    #define XANGINE_PLATFORM_LINUX
    #define XANGINE_PLATFORM_NAME "Linux"
    #define XANGINE_PLATFORM_DETECTED
    
#else
    #error "Unknown platform!"
#endif

#endif // XANGINE_PLATFORM_DETECTED
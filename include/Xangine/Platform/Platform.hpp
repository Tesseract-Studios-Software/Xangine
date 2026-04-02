#pragma once

// ============================================
// OPERATING SYSTEM DETECTION
// ============================================

#ifdef _WIN32
    #define XANGINE_PLATFORM_WINDOWS
    #define XANGINE_PLATFORM_NAME "Windows"
    
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
    
#elif __linux__
    #define XANGINE_PLATFORM_LINUX
    #define XANGINE_PLATFORM_NAME "Linux"
    
#else
    #error "Unknown platform!"
#endif

// ============================================
// COMPILER DETECTION
// ============================================

#ifdef _MSC_VER
    #define XANGINE_COMPILER_MSVC
    #define XANGINE_COMPILER_NAME "MSVC"
    
#elif __clang__
    #define XANGINE_COMPILER_CLANG
    #define XANGINE_COMPILER_NAME "Clang"
    
#elif __GNUC__
    #define XANGINE_COMPILER_GCC
    #define XANGINE_COMPILER_NAME "GCC"
#endif

// ============================================
// ARCHITECTURE DETECTION
// ============================================

#ifdef _WIN64
    #define XANGINE_ARCH_64_BIT
#elif __x86_64__
    #define XANGINE_ARCH_64_BIT
#elif __arm64__
    #define XANGINE_ARCH_ARM64
#endif

// ============================================
// HELPER MACROS
// ============================================

// Export symbols for DLL (Windows only)
#ifdef XANGINE_PLATFORM_WINDOWS
    #ifdef XANGINE_BUILD_DLL
        #define XANGINE_API __declspec(dllexport)
    #else
        #define XANGINE_API __declspec(dllimport)
    #endif
#else
    #define XANGINE_API  // Empty on other platforms
#endif

// Debug/Release detection
#ifdef _DEBUG
    #define XANGINE_DEBUG
#elif defined(NDEBUG)
    #define XANGINE_RELEASE
#endif
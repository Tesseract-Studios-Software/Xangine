cmake_minimum_required(VERSION 3.20)
project(Xangine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform detection
if(APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    add_definitions(-DXANGINE_PLATFORM_MACOS)
endif()

# Find GLFW
find_package(glfw3 REQUIRED)

# Xangine library
add_library(Xangine STATIC)

target_sources(Xangine PRIVATE
    src/Xangine/Core/Window.cpp
    src/Xangine/Core/Transform.cpp  # Add this
    src/Xangine/Graphics/Backends/OpenGL/OpenGLRenderer.cpp
)

target_include_directories(Xangine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Xangine
    glfw
    ${OPENGL_LIB}
)
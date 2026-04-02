cmake_minimum_required(VERSION 3.20)
project(Xangine)

set(CMAKE_CXX_STANDARD 20)

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Xangine library
add_library(Xangine STATIC)
target_sources(Xangine PRIVATE
    src/Graphics/Backends/OpenGL/OpenGLRenderer.cpp
    # Add other .cpp files as you create them
)
target_include_directories(Xangine PUBLIC include)
target_link_libraries(Xangine OpenGL::GL glfw)

# # Example
# add_executable(CubeExample Examples/BasicCube/main.cpp)
# target_link_libraries(CubeExample Xangine)

# Find OpenGL (macOS uses framework)
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# GLEW might need manual path on macOS
set(GLEW_INCLUDE_DIR "/usr/local/include")
set(GLEW_LIBRARY "/usr/local/lib/libGLEW.dylib")

# Add to your target
target_include_directories(Xangine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GLEW_INCLUDE_DIR}
)

target_link_libraries(Xangine 
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${GLEW_LIBRARY}
)
cmake_minimum_required(VERSION 3.20)
project(Xangine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GLFW
find_package(glfw3 REQUIRED)

# Platform-specific setup
if(APPLE)
    # macOS uses frameworks
    find_library(OPENGL_LIB OpenGL REQUIRED)
    find_library(COCOA_LIB Cocoa REQUIRED)
    find_library(IOKIT_LIB IOKit REQUIRED)
    find_library(COREVIDEO_LIB CoreVideo REQUIRED)
    
    set(PLATFORM_LIBS ${OPENGL_LIB} ${COCOA_LIB} ${IOKIT_LIB} ${COREVIDEO_LIB})
    add_definitions(-DXANGINE_PLATFORM_MACOS)
elseif(WIN32)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    set(PLATFORM_LIBS ${OPENGL_LIB} ${GLEW_LIBRARIES})
    add_definitions(-DXANGINE_PLATFORM_WINDOWS)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Xangine library
add_library(Xangine STATIC)

target_sources(Xangine PRIVATE
    src/Graphics/Backends/OpenGL/OpenGLRenderer.cpp
)

target_link_libraries(Xangine 
    glfw
    ${PLATFORM_LIBS}
)

# Example executable
add_executable(CubeExample Examples/BasicCube/main.cpp)
target_link_libraries(CubeExample Xangine)
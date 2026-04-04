cmake_minimum_required(VERSION 3.20)
project(TheCube)

set(CMAKE_CXX_STANDARD 20)

add_executable(TheCube TheCube.cpp)

# Add Xangine include directory (relative path)
target_include_directories(TheCube PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Add Xangine library (assuming it's built)
target_link_libraries(TheCube
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/libXangine.a
)

# GLFW setup
find_package(glfw3 REQUIRED)
target_link_libraries(TheCube glfw)

if(APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(TheCube ${OPENGL_LIB})
endif()
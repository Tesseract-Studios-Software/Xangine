cmake_minimum_required(VERSION 3.20)
project(WindowTest)

set(CMAKE_CXX_STANDARD 20)

add_executable(WindowTest WindowTest.cpp)

# Add Xangine include directory (relative path)
target_include_directories(WindowTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Add Xangine library (assuming it's built)
target_link_libraries(WindowTest
    ${CMAKE_CURRENT_SOURCE_DIR}/../../build/libXangine.a
)

# GLFW setup
find_package(glfw3 REQUIRED)
target_link_libraries(WindowTest glfw)

if(APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(WindowTest ${OPENGL_LIB})
endif()
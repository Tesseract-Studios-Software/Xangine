cmake_minimum_required(VERSION 3.20)
project(TheCube)

set(CMAKE_CXX_STANDARD 20)

add_executable(TheCube TheCube.cpp)

# Add Xangine include directory
target_include_directories(TheCube PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)

# Add Xangine as a subdirectory
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../.. ${CMAKE_CURRENT_SOURCE_DIR}/../../build)

# Link against Xangine (it should bring GLFW dependencies)
target_link_libraries(TheCube
    Xangine
)

if(APPLE)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(TheCube ${OPENGL_LIB})
endif()
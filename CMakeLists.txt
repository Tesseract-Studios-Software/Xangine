cmake_minimum_required(VERSION 3.20)
project(Xangine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tell CMake where to find your headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Or use target_include_directories (more modern)
add_library(Xangine STATIC)
target_include_directories(Xangine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add your source files
target_sources(Xangine PRIVATE
    # Add .cpp files here when you have them
)
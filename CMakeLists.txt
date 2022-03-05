cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(src)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/hub-install)

option(BUILD_EXAMPLES "Build examples" OFF)

if (BUILD_EXAMPLES)
        option(BUILD_LIDAR_EXAMPLE "Build Lidar examples (depends of realsense2 lib)" OFF)
        option(BUILD_OPENCV_EXAMPLE "Build OpenCV examples (depends of opencv lib)" OFF)
        add_subdirectory(applications)
endif()

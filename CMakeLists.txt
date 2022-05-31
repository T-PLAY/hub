cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/hub-install)

option(BUILD_EXAMPLES "Build examples" OFF)

if (BUILD_EXAMPLES)
    option(BUILD_VIEWERS "Build viewers" ON)
    option(BUILD_LIDAR_EXAMPLE "Build Lidar examples (depends of realsense2 lib)" OFF)
    option(BUILD_OPENCV_EXAMPLE "Build OpenCV examples (depends of opencv lib)" OFF)
    option(BUILD_PDI_EXAMPLE "Build PDI examples (depends of polhemus lib)" OFF)
    option(BUILD_PATRIOT_EXAMPLE "Build Patriot examples (depends of polhemus lib)" OFF)
    option(BUILD_RADIUM_EXAMPLE "Build Radium examples (depends of radium lib)" OFF)
    add_subdirectory(applications)
endif()

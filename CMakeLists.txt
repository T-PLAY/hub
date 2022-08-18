cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

option(HUB_BUILD_STATIC_LIBRARY OFF)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
set(SRC_TARGET hub)
add_subdirectory(src)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/hub-install)

#set(CMAKE_SYSTEM_NAME WindowsStore)
#set(CMAKE_SYSTEM_VERSION 10.0)

option(HUB_ENABLE_EXAMPLES "Build examples" OFF)
if (HUB_ENABLE_EXAMPLES)
#    option(HAS_QT "Build qt examples" OFF)
    option(HUB_ENABLE_LIDAR_EXAMPLE "Build Lidar examples (depends of realsense2 lib)" OFF)
    option(HUB_ENABLE_OPENCV_EXAMPLE "Build OpenCV examples (depends of opencv lib)" OFF)
    option(HUB_ENABLE_PDI_EXAMPLE "Build PDI examples (depends of polhemus lib)" OFF)
    option(HUB_ENABLE_KEYBOARD_STREAMER "Build keyboard streamer (depends of glm lib)" OFF)
    option(HUB_ENABLE_PATRIOT_EXAMPLE "Build Patriot examples (depends of polhemus lib)" OFF)
    option(HUB_ENABLE_RADIUM_EXAMPLE "Build Radium examples (depends of radium lib)" OFF)
    option(HUB_ENABLE_DCMTK "Build Dcmtk examples and lib" OFF)
    option(HUB_ENABLE_SCUS "Build Scus" OFF)
    add_subdirectory(applications)
endif()


option(HUB_ENABLE_TESTS "Enable tests" ON)
if (HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

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
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

option(HUB_ENABLE_APPLICATIONS "Build applications" OFF)
#if (HUB_ENABLE_APPLICATIONS)
#    option(HAS_QT "Build qt examples" OFF)
option(HUB_APPLICATION_ENABLE_LIDAR "Build Lidar examples (depends of realsense2 lib)" OFF)
option(HUB_APPLICATION_ENABLE_OPENCV "Build OpenCV examples (depends of opencv lib)" OFF)
option(HUB_APPLICATION_ENABLE_PDI "Build PDI examples (depends of polhemus lib)" OFF)
option(HUB_APPLICATION_ENABLE_KEYBOARD "Build keyboard streamer (depends of glm lib)" OFF)
#option(HUB_APPLICATION_ENABLE_PATRIOT "Build Patriot examples (depends of polhemus lib)" OFF)
option(HUB_APPLICATION_ENABLE_RADIUM "Build Radium examples (depends of radium lib)" OFF)
option(HUB_USE_DCMTK "Build Dcmtk examples and libs" OFF)
option(HUB_ENABLE_SCUS "Build Scus" OFF)
add_subdirectory(applications)
#endif()

option(HUB_ENABLE_TESTS "Enable tests" ON)
if (HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

option(HUB_ENABLE_EXAMPLES "Enable examples" ON)
if (HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

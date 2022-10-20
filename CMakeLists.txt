cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

project (Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

#option(HUB_BUILD_STATIC_LIBRARY OFF)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
set(SRC_TARGET hub)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/hub-install)

#set(CMAKE_SYSTEM_NAME WindowsStore)
#set(CMAKE_SYSTEM_VERSION 10.0)

option(HUB_ENABLE_APPLICATIONS "Enable applications" OFF)
option(HUB_ENABLE_QT "Enable Qt" OFF)
option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)
option(HUB_APPLICATION_ENABLE_RADIUM "Build Radium examples (depends of radium lib)" OFF)
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
#endif()
#if (HUB_BUILD_STATIC_LIBRARY) {
#    add_definitions(-DSTATIC)
#}
#else {
#    add_definitions(-DEXPORT)
#}

add_subdirectory(src)
add_subdirectory(applications)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#    add_definitions(-DDEBUG)
    target_compile_definitions(hub PUBLIC -DDEBUG)
else()
#    add_definitions(-DNDEBUG)
    target_compile_definitions(hub PUBLIC -DNDEBUG)
endif()
#add_definitions(-DDEBUG)

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if (HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if (HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

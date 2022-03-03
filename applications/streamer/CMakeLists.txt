cmake_minimum_required (VERSION 3.8)

add_subdirectory(simpleStreamer)
if (BUILD_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()
add_subdirectory(webcam)


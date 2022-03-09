cmake_minimum_required (VERSION 3.8)

add_subdirectory(simpleStreamer)

if (BUILD_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()

if (BUILD_OPENCV_EXAMPLE)
        add_subdirectory(webcam)
endif()

if (BUILD_PDI_EXAMPLE)
    add_subdirectory(pdi)
endif()

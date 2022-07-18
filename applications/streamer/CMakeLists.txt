cmake_minimum_required (VERSION 3.8)

add_subdirectory(simpleStreamer)

#if (UNIX)
add_subdirectory(posStreamer)
#endif()


if (BUILD_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()

if (BUILD_OPENCV_EXAMPLE)
    add_subdirectory(webcam)
endif()

if (BUILD_PDI_EXAMPLE)
    add_subdirectory(pdi)
endif()

if (BUILD_PATRIOT_EXAMPLE)
    add_subdirectory(patriot)
endif()

if (BUILD_SCUS AND BUILD_DCMTK)
    add_subdirectory(simulator)
endif()

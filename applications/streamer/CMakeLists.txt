cmake_minimum_required (VERSION 3.8)

add_subdirectory(simpleStreamer)

#if (UNIX)
if (HUB_ENABLE_KEYBOARD_STREAMER)
	add_subdirectory(posStreamer)
endif()


if (HUB_ENABLE_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()

if (HUB_ENABLE_OPENCV_EXAMPLE)
    add_subdirectory(webcam)
endif()

if (HUB_ENABLE_PDI_EXAMPLE)
    add_subdirectory(pdi)
endif()

if (HUB_ENABLE_PATRIOT_EXAMPLE)
    add_subdirectory(patriot)
endif()

if (HUB_ENABLE_SCUS AND HUB_ENABLE_DCMTK)
    add_subdirectory(simulator)
endif()

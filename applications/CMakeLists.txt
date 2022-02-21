

add_subdirectory(ihm)
add_subdirectory(realTimeViewer)
add_subdirectory(server)
add_subdirectory(streamer)

if (BUILD_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()


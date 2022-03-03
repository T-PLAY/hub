

#add_subdirectory(ihm)
add_subdirectory(viewer)
add_subdirectory(server)
add_subdirectory(streamer)

if (BUILD_LIDAR_EXAMPLE)
    add_subdirectory(lidar)
endif()


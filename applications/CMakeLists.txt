

#add_subdirectory(ihm)
if (BUILD_VIEWERS)
	add_subdirectory(viewer)
endif()
add_subdirectory(server)
add_subdirectory(streamer)
add_subdirectory(recorder)
add_subdirectory(player)


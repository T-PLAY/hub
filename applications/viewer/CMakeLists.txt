cmake_minimum_required (VERSION 3.8)

add_subdirectory(libViewer)
add_subdirectory(clientViewer)
add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)

if (BUILD_RADIUM_EXAMPLE)
    add_subdirectory(radiumViewer)
endif()

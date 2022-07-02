cmake_minimum_required (VERSION 3.8)

add_subdirectory(libViewer)

#add_subdirectory(clientViewer)
add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)

if (BUILD_RADIUM_EXAMPLE)
    cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
    if (${hostname} STREQUAL "brueghel")
        add_definitions(-DMRI_PATH=\"/export/home/brueghel/gbouyjou/Downloads/MRI/\")
    elseif (${hostname} STREQUAL "gigabyte")
        add_definitions(-DMRI_PATH=\"/home/gauthier/tmpDicomViewer/MRI/\")
    else()
        message(FATAL_ERROR "unknown system for finding MRI path")
    endif ()

#    add_definitions(-DONLY_POSE)
    add_subdirectory(radiumViewer)
#    add_subdirectory(radiumViewer2)
#    add_subdirectory(radiumViewer3)
#    add_subdirectory(radiumViewer4)
#    add_subdirectory(radiumViewer5)
    add_subdirectory(radiumViewer6)

    if (BUILD_DCMTK)

        add_subdirectory(libDicomLoader)
        add_subdirectory(VolumeDemoApp)
        add_subdirectory(TexturedQuad)

    endif()
endif()


message(STATUS "QT_VERSION = '" ${QT_VERSION} "'")


find_package(QT NAMES Qt6 Qt5)
set(QT_VERSION ${QT_VERSION_MAJOR})
#message(STATUS ${QT_VERSION})
if (NOT QT_VERSION)
        set(QT_VERSION 5)
endif()
#set(QT_VERSION 5)
#set(QT_VERSION 6)

cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
if (${hostname} STREQUAL "brueghel") # work linux
    add_definitions(-DUSE_GOT_PR)
    set(MRI_PATH "/export/home/brueghel/gbouyjou/Downloads/MRI/")
elseif (${hostname} STREQUAL "Brueghel") # work window
    set(MRI_PATH "C:/Users/gbouyjou/Downloads/MRI/")

elseif (${hostname} STREQUAL "gigabyte")
    set(MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/")
elseif (${hostname} STREQUAL "msi")
    set(MRI_PATH "/home/gauthier/Downloads/MRI/")
else()
    message(FATAL_ERROR "unknown system for finding MRI path")
endif ()
add_definitions(-DMRI_PATH=\"${MRI_PATH}\")


add_subdirectory(libApp)

#add_subdirectory(ihm)
if (BUILD_VIEWERS)
    add_subdirectory(viewer)
endif()
add_subdirectory(server)
add_subdirectory(streamer)
add_subdirectory(recorder)
add_subdirectory(player)
add_subdirectory(loader)

add_subdirectory(imageManipulator)

if (BUILD_SCUS AND BUILD_DCMTK)
	add_subdirectory(testScus)
endif()

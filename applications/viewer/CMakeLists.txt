cmake_minimum_required (VERSION 3.8)

find_package(QT NAMES Qt6 Qt5)
#set(QT_VERSION 5)
set(QT_VERSION ${QT_VERSION_MAJOR})
#set(QT_VERSION 6)

add_subdirectory(libViewer)
add_subdirectory(clientViewer)
add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)

if (BUILD_RADIUM_EXAMPLE)
#    add_definitions(-DONLY_POSE)
    add_subdirectory(radiumViewer)
#    add_subdirectory(radiumViewer2)
#    add_subdirectory(radiumViewer3)
#    add_subdirectory(radiumViewer4)
#    add_subdirectory(radiumViewer5)
    add_subdirectory(radiumViewer6)
endif()


message(STATUS "QT_VERSION = " ${QT_VERSION})


find_package(QT NAMES Qt6 Qt5)
set(QT_VERSION ${QT_VERSION_MAJOR})
#message(STATUS ${QT_VERSION})
if (NOT QT_VERSION)
        set(QT_VERSION 5)
endif()
#set(QT_VERSION 5)
#set(QT_VERSION 6)


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



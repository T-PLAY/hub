
add_subdirectory(server)

if (HUB_ENABLE_APPLICATIONS)

    cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
    if (${hostname} STREQUAL "brueghel") # work linux
        add_definitions(-DUSE_GOT_PR)
        add_definitions(-DNO_PR_976)
        set(MRI_PATH "/export/home/brueghel/gbouyjou/Downloads/MRI/")

    elseif (${hostname} STREQUAL "Brueghel") # work window
        add_definitions(-DUSE_GOT_PR)
        set(MRI_PATH "E:/Downloads/MRI/")

        #set(Qt5_DIR PATH "E:/Programs/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
        #set(Radium_DIR PATH "E:/Documents/radiumReleaseCandidate/Radium-Engine/out/install/x64-Debug/lib/cmake/Radium")

    elseif (${hostname} STREQUAL "gigabyte")
        add_definitions(-DUSE_GOT_PR)
        #    add_definitions(-DUSE_GOT_CONTRIB)
        set(MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/")

    elseif (${hostname} STREQUAL "msi")
        set(MRI_PATH "/home/gauthier/Downloads/MRI/")

    else()
        message(WARNING "unknown system for finding MRI path")
    endif ()
    add_definitions(-DMRI_PATH=\"${MRI_PATH}\")

    find_package(QT NAMES Qt6 Qt5)
    set(QT_VERSION ${QT_VERSION_MAJOR})
    if (NOT QT_VERSION)
        if (NOT Qt5_DIR)
			set(Qt5_DIR CACHE PATH)
        endif()
    endif()

    add_subdirectory(libApp)

    if (QT_VERSION)
        add_subdirectory(viewer)
#        add_subdirectory(loader)
        add_subdirectory(imageManipulator)
    endif()
    add_subdirectory(streamer)
#    add_subdirectory(recorder)
#    add_subdirectory(player)

    if (HUB_ENABLE_SCUS AND HUB_USE_DCMTK)
        add_subdirectory(testScus)
    endif()

endif()



# if(HUB_ENABLE_APPLICATIONS OR HUB_ENABLE_TESTS)
#    add_subdirectory(server)
# endif()

option(HUB_ENABLE_APPLICATIONS "Enable applications" OFF)
if(HUB_ENABLE_TESTS OR HUB_ENABLE_EXAMPLES OR HUB_ENABLE_APPLICATIONS)
    add_subdirectory(server)
endif()

if(HUB_ENABLE_APPLICATIONS)

    set(Radium_DIR)
    option(HUB_APPLICATION_ENABLE_RADIUM "Build Radium examples (depends of radium lib)" OFF)
    option(HUB_APPLICATION_ENABLE_LIDAR "Build Lidar examples (depends of realsense2 lib)" OFF)
    option(HUB_APPLICATION_ENABLE_OPENCV "Build OpenCV examples (depends of opencv lib)" OFF)
    option(HUB_APPLICATION_ENABLE_PDI "Build PDI examples (depends of polhemus lib)" OFF)
    option(HUB_APPLICATION_ENABLE_KEYBOARD "Build keyboard streamer (depends of glm lib)" OFF)
    option(HUB_APPLICATION_ENABLE_DCMTK "Build Dcmtk examples and libs" OFF)
    option(HUB_ENABLE_SCUS "Build Scus" OFF)


    cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
    if(${hostname} STREQUAL "brueghel") # work linux
        add_definitions(-DUSE_GOT_PR)
        add_definitions(-DNO_PR_976)
        set(MRI_PATH "/export/home/brueghel/gbouyjou/Downloads/MRI/")

    elseif(${hostname} STREQUAL "Brueghel") # work window
        add_definitions(-DUSE_GOT_PR)
        set(MRI_PATH "E:/Downloads/MRI/")

        set(QT_DIR "E:/Programs/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
        set(Qt5_DIR "E:/Programs/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")

        set(Radium_DIR
            "E:/Documents/radiumReleaseCandidate4/Radium-Engine/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/Radium"
        )
        set(Scus_DIR "E:/Documents/medical/scus/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/Scus")
        set(TBB_DIR "E:/Documents/medical/oneTBB/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/TBB")
        set(DCMTK_DIR "E:/Documents/medical/dcmtk/out/install/x64-${CMAKE_BUILD_TYPE}/cmake")
        set(OpenCV_DIR "E:/Documents/medical/opencv/out/install/x64-${CMAKE_BUILD_TYPE}/")
        set(realsense2_DIR "E:/Documents/medical/lidar/librealsense/lidar-install/lib/cmake/realsense2")

        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(glm_DIR
                "E:/Documents/radiumReleaseCandidate4/radium-external/install-d/lib/cmake/glm")
        else()
            set(glm_DIR
                "E:/Documents/radiumReleaseCandidate4/radium-external/install-r/lib/cmake/glm")
        endif()

    elseif(${hostname} STREQUAL "gigabyte")
        add_definitions(-DUSE_GOT_PR)
        set(MRI_PATH "/home/gauthier/tmpDicomViewer/MRI/")

        set(Scus_DIR "/home/gauthier/document/boulot/irit/medical/scus/build/scus-install/lib/cmake/Scus")
        set(Radium_DIR "/home/gauthier/tmpRadiumReleaseCandidate/Radium-Engine/Bundle-Clang-Debug/lib/cmake/Radium")
        set(realsense2_DIR "/home/gauthier/document/boulot/irit/medical/librealsense/build/realsense-install/lib/cmake/realsense2")

    elseif(${hostname} STREQUAL "msi")
        set(MRI_PATH "/home/gauthier/Downloads/MRI/")

    else()
        message(WARNING "unknown system for finding MRI path")
    endif()
    add_definitions(-DMRI_PATH=\"${MRI_PATH}\")

#    if(HUB_ENABLE_QT)
        find_package(QT NAMES Qt6 Qt5 REQUIRED)
        set(QT_VERSION ${QT_VERSION_MAJOR})
        message(STATUS "QT_VERSION = '" ${QT_VERSION_MAJOR} "' '" ${QT_VERSION_MINOR} "'")
#    endif()

#    add_subdirectory(libApp)

    add_subdirectory(importer)

#    if(HUB_ENABLE_QT)
        if(QT_VERSION)
            add_subdirectory(viewer)
            add_subdirectory(loader)
#            add_subdirectory(imageManipulator)
        endif()
#    endif()
    add_subdirectory(streamer)
    add_subdirectory(recorder)
    add_subdirectory(player)
    add_subdirectory(exporter)
    add_subdirectory(calibrator)


endif()

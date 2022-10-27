
if (HUB_ENABLE_APPLICATIONS OR HUB_ENABLE_TESTS)
        add_subdirectory(server)
endif()

set(Radium_DIR)

if (HUB_ENABLE_APPLICATIONS)

        cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
        if (${hostname} STREQUAL "brueghel") # work linux
                add_definitions(-DUSE_GOT_PR)
                add_definitions(-DNO_PR_976)
                set(MRI_PATH "/export/home/brueghel/gbouyjou/Downloads/MRI/")

        elseif (${hostname} STREQUAL "Brueghel") # work window
                add_definitions(-DUSE_GOT_PR)
                set(MRI_PATH "E:/Downloads/MRI/")

                #	set(PDI_SDK "<UNSET>" CACHE PATH "Path of PDI SDK dir on your system")
                #        set(Qt5_DIR CACHE PATH "E:/Programs/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
                set(QT_DIR "E:/Programs/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")

                set(Radium_DIR "E:/Documents/radiumReleaseCandidate4/Radium-Engine/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/Radium")
                set(Scus_DIR "E:/Documents/medical/scus/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/Scus")
                set(TBB_DIR "E:/Documents/medical/oneTBB/out/install/x64-${CMAKE_BUILD_TYPE}/lib/cmake/TBB")
                set(DCMTK_DIR "E:/Documents/medical/dcmtk/out/install/x64-${CMAKE_BUILD_TYPE}/cmake")

                if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
                        set(glm_DIR "E:/Documents/radiumReleaseCandidate4/radium-external/install-d/lib/cmake/glm")
                else()
                        set(glm_DIR "E:/Documents/radiumReleaseCandidate4/radium-external/install-r/lib/cmake/glm")
                endif()
                #                set(Eigen3_DIR "E:/Documents/radiumReleaseCandidate3/radium-external/install-d/share/eigen3/cmake")


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

        if (HUB_ENABLE_QT)
			find_package(QT NAMES Qt6 Qt5)
			set(QT_VERSION ${QT_VERSION_MAJOR})
			#    if (NOT QT_VERSION)
			#        if (NOT Qt5_DIR)
			#                set(Qt5_DIR CACHE PATH "")
			#        endif()
			#    endif()
			message(STATUS "QT_VERSION = '" ${QT_VERSION} "'")
		endif()

        add_subdirectory(libApp)

        if (HUB_ENABLE_QT)
			if (QT_VERSION)
					add_subdirectory(viewer)
                                                add_subdirectory(loader)
#                                    if (HUB_USE_DCMTK)
					add_subdirectory(imageManipulator)
#                                    endif()
			endif()
        endif()
        add_subdirectory(streamer)
        add_subdirectory(recorder)
            add_subdirectory(player)
            add_subdirectory(exporter)
            add_subdirectory(calibrator)

        if (HUB_ENABLE_SCUS AND HUB_USE_DCMTK)
                add_subdirectory(testScus)
        endif()

endif()

cmake_minimum_required (VERSION 3.8)

add_subdirectory(libViewer)

#add_subdirectory(clientViewer)
add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)
add_subdirectory(nativeViewer)

if (BUILD_RADIUM_EXAMPLE)

    #    add_definitions(-DONLY_POSE)
    add_subdirectory(radiumViewer)
    #    add_subdirectory(radiumViewer2)
    #    add_subdirectory(radiumViewer3)
    #    add_subdirectory(radiumViewer4)


    if (BUILD_DCMTK)
        add_subdirectory(libDicomLoader)
        add_subdirectory(VolumeDemoApp)
        add_subdirectory(TexturedQuad)
    endif()

    add_subdirectory(libGuiManager)

#            add_subdirectory(radiumViewer5) #minimal BaseApplication
    add_subdirectory(radiumViewer6)
#            add_subdirectory(radiumViewer7) #radium-apps sandbox

endif()


message(STATUS "QT_VERSION = '" ${QT_VERSION} "'")

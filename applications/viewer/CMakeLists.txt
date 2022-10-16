cmake_minimum_required (VERSION 3.8)

if (QT_VERSION)
	add_subdirectory(libViewer)
endif()

#add_subdirectory(clientViewer)
add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)
# add_subdirectory(smartViewer)
#add_subdirectory(nativeViewer)

if (HUB_USE_DCMTK)
        add_subdirectory(libDicomUtils)
endif()

if (HUB_APPLICATION_ENABLE_RADIUM)

    #    add_definitions(-DONLY_POSE)
#    add_subdirectory(radiumViewer)
    #    add_subdirectory(radiumViewer2)
    #    add_subdirectory(radiumViewer3)
    #    add_subdirectory(radiumViewer4)


    if (HUB_USE_DCMTK)
        add_subdirectory(libDicomLoader)
        add_subdirectory(VolumeDemoApp)
        add_subdirectory(TexturedQuad)
    endif()

    add_subdirectory(libGuiManager)

#            add_subdirectory(radiumViewer5) #minimal BaseApplication
    add_subdirectory(radiumViewer6)
#            add_subdirectory(radiumViewer7) #radium-apps sandbox
            add_subdirectory(radiumViewer8)

endif()


#message(STATUS "QT_VERSION = '" ${QT_VERSION} "'")

cmake_minimum_required(VERSION 3.8)

if(QT_VERSION)
    add_subdirectory(libViewer)
endif()

add_subdirectory(streamViewer)
add_subdirectory(simpleViewer)

if(HUB_USE_DCMTK)
    add_subdirectory(libDicomUtils)
endif()

if(HUB_APPLICATION_ENABLE_RADIUM)


    if(HUB_USE_DCMTK)
        add_subdirectory(libDicomLoader)
        add_subdirectory(VolumeDemoApp)
        add_subdirectory(TexturedQuad)
    endif()

    add_subdirectory(libGuiManager)

    # add_subdirectory(radiumViewer5) #minimal BaseApplication
    add_subdirectory(radiumViewer6)
    # add_subdirectory(radiumViewer7) #radium-apps sandbox add_subdirectory(radiumViewer8)

endif()


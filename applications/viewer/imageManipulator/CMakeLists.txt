cmake_minimum_required(VERSION 3.8)

add_subdirectory(libImageManipulator)

if(HUB_APPLICATION_ENABLE_DCMTK)
    add_subdirectory(simpleImageManipulator)
endif()

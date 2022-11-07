cmake_minimum_required(VERSION 3.8)

add_subdirectory(libImageManipulator)

if(HUB_USE_DCMTK)
    add_subdirectory(simpleImageManipulator)
endif()

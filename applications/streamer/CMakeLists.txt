cmake_minimum_required(VERSION 3.8)

add_subdirectory(simpleStreamer)

if(HUB_APPLICATION_ENABLE_KEYBOARD)
    add_subdirectory(posStreamer)
endif()

if(HUB_APPLICATION_ENABLE_LIDAR)
    add_subdirectory(lidar)
endif()

if(HUB_APPLICATION_ENABLE_OPENCV)
    add_subdirectory(webcam)
endif()

if(HUB_APPLICATION_ENABLE_PDI)
#    add_subdirectory(pdi)
endif()

if(HUB_APPLICATION_ENABLE_PATRIOT)
    add_subdirectory(patriot)
endif()

if(HUB_APPLICATION_ENABLE_SCUS AND HUB_APPLICATION_ENABLE_DCMTK)
    add_subdirectory(testScus)
    add_subdirectory(simulator)
    add_subdirectory(simulatorOld)
endif()

if(HUB_APPLICATION_ENABLE_DCMTK)
    add_subdirectory(dicomStreamer)
endif()

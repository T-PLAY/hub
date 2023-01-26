cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

file(GLOB EXAMPLE_SRC *.cpp)

string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}")
set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

foreach(name ${EXAMPLE_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME ${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
    # target_link_libraries(${TARGET_NAME} hub libServer)
    target_link_libraries(${TARGET_NAME} hub)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
endforeach()


cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
if(${hostname} STREQUAL "Brueghel") # work windows
    set(OpenCV_DIR "E:/Documents/medical/opencv/out/install/x64-${CMAKE_BUILD_TYPE}/")
    set(realsense2_DIR "E:/Documents/medical/lidar/librealsense/lidar-install/lib/cmake/realsense2")

elseif(${hostname} STREQUAL "gigabyte2")
    set(realsense2_DIR "/home/gauthier/tmpMedical/librealsense/build/realsense-install/lib/cmake/realsense2")
endif()


add_subdirectory(example-intel-realsense)
add_subdirectory(example-opencv)


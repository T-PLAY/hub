cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project(patriot)





file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(
    ${PROJECT_NAME}
    ${sources}
    ${headers})

target_include_directories(
    ${PROJECT_NAME} PUBLIC # ${SRC_DIR} # ${LIBUSB1_INCLUDE_DIRS} ${LibUSB_INCLUDE_DIRS}
                           include/ ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)
    # #    set(THREADS_PREFER_PTHREAD_FLAG ON) find_package(Threads REQUIRED) #
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC # ${libusb-1.0_LIBRARIES} ${LIBUSB_LIBRARY} Libusb1
           usb-1.0
           # usb ${LIBUSB1_LIBRARIES} #    # ${REQUIRED_LIBS_QUALIFIED} #    # ${UNIX_REQUIRED_LIBS}
           # #    # ${WIN32_REQUIRED_LIBS}
)

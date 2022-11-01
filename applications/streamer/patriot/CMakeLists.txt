cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
# set(CMAKE_MODULE_PATH cmake)

project(patriot)

# add_compile_options("-fno-stack-protector")

# find_package(libusb-1.0 REQUIRED) find_package(libusb1 REQUIRED) find_package(PkgConfig REQUIRED)
# pkg_check_modules(libusb REQUIRED libusb-1.0) find_package(libusb REQUIRED) find_package(Libusb1
# REQUIRED)

# find_package(LibUsb REQUIRED) find_package(usb-1.0 REQUIRED)

# find_package(libusb REQUIRED) find_package(libusb-1.0 REQUIRED) find_package(usb REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(
    ${PROJECT_NAME}
    ${sources}
    # ${forms}
    ${headers})

target_include_directories(
    ${PROJECT_NAME} PUBLIC # ${SRC_DIR} # ${LIBUSB1_INCLUDE_DIRS} ${LibUSB_INCLUDE_DIRS}
                           include/ ${PROJECT_SOURCE_DIR})

if(WIN32)
    # set(WIN32_REQUIRED_LIBS wsock32 ws2_32)

elseif(UNIX)
    # #    set(THREADS_PREFER_PTHREAD_FLAG ON) find_package(Threads REQUIRED) #
    # target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT}) set(UNIX_REQUIRED_LIBS
    # Threads::Threads)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC # ${libusb-1.0_LIBRARIES} ${LIBUSB_LIBRARY} Libusb1
           usb-1.0
           # LibUsb ${LibUSB_LIBRARIES} ${LibUSB_LIBRARY} ${usb_LIBRARY} usb-1.0 libusb usb libusb
           # usb ${LIBUSB1_LIBRARIES} #    # ${REQUIRED_LIBS_QUALIFIED} #    # ${UNIX_REQUIRED_LIBS}
           # #    # ${WIN32_REQUIRED_LIBS}
)
#

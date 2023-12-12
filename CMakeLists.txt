cmake_minimum_required(VERSION 3.8)

project(hub)


include(cmake/Functions.cmake)
set_property(GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED "")


#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD 14) # std::decay
#set(CMAKE_CXX_STANDARD 17) # std::any, AppleClang std::void_t<>
set(CMAKE_CXX_STANDARD 20) # std::any, zpp_bits
#set(CMAKE_CXX_STANDARD 23) # std::any
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)
#if(WIN32)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /W4")
#else()
##    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra")
##    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++2a")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(WIN32)
    add_definitions(-DCPLUSPLUS_VERSION=${CMAKE_CXX_STANDARD})
endif()

message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake system version: ${CMAKE_SYSTEM_VERSION}")

# Looking for current system archictecture
set(ARCHITECTURE "")
if(CMAKE_GENERATOR_PLATFORM)
    message(STATUS "CMake generator platform: ${CMAKE_GENERATOR_PLATFORM}")
    if(${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
        set(ARCHITECTURE "x64")
    elseif(${CMAKE_GENERATOR_PLATFORM} STREQUAL "Win32")
        set(ARCHITECTURE "x86")
    elseif(${CMAKE_GENERATOR_PLATFORM} STREQUAL "ARM64")
        set(ARCHITECTURE "ARM64")
    elseif(${CMAKE_GENERATOR_PLATFORM} STREQUAL "ARM")
        set(ARCHITECTURE "ARM")
    else()
        message(FATAL_ERROR "unknown platform")
    endif()
else()
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(ARCHITECTURE "x86")
    else()
        set(ARCHITECTURE "x64")
    endif()
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "WindowsStore")
    set(ARCHITECTURE "UWP-${ARCHITECTURE}")
endif()
message(STATUS "Architecture: ${ARCHITECTURE}")

foreach(
    SYSTEM_INFO IN
    ITEMS "HOSTNAME"
          "TOTAL_PHYSICAL_MEMORY"
          "PROCESSOR_DESCRIPTION"
          "OS_NAME"
          "OS_RELEASE"
          "OS_VERSION"
          "OS_PLATFORM")
    cmake_host_system_information(RESULT tmp QUERY ${SYSTEM_INFO})
    set(${SYSTEM_INFO} ${tmp})
endforeach()

if (WIN32)
        set(USERNAME $ENV{USERNAME})
else()
        set(USERNAME $ENV{USER})
endif()

# Looking for supported compiler
set(SUPPORTED_COMPILERS "Clang" "GNU" "LLVM" "MSVC" "AppleClang")
set(Clang_MINIMAL_VERSION 11.0.0)
#set(Clang_MINIMAL_VERSION 7.0.1)
set(GNU_MINIMAL_VERSION 8.1.0)
#set(GNU_MINIMAL_VERSION 4.9.4)
set(LLVM_MINIMAL_VERSION 0)
set(MSVC_MINIMAL_VERSION 19)
set(AppleClang_MINIMAL_VERSION 12)

set(COMPILER_FOUND)
foreach(SUPPORTED_COMPILER ${SUPPORTED_COMPILERS})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "${SUPPORTED_COMPILER}")
        if(COMPILER_FOUND)
            message(FATAL_ERROR "too many compilers founded ${COMPILER_FOUND} and ${SUPPORTED_COMPILER}")
        endif()
        if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS ${${SUPPORTED_COMPILER}_MINIMAL_VERSION})
            message(
                FATAL_ERROR
                    "${CMAKE_CXX_COMPILER_ID} version must be ${${SUPPORTED_COMPILER}_MINIMAL_VERSION} or greater! Aborting..."
            )
        endif()
        set(COMPILER_FOUND ${SUPPORTED_COMPILER})
        set("${SUPPORTED_COMPILER}" 1)
    else()
        set("${SUPPORTED_COMPILER}" 0)
    endif()
endforeach()

if (LLVM)
    message(FATAL_ERROR "not supported")
endif()


if(NOT COMPILER_FOUND)
    message(FATAL_ERROR "no supported compiler founded")
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX OR UNIX)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Install path prefix." FORCE)
endif()

# Set default value of CMake variables
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "CMake build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RELWITHDEBINFO_POSTFIX rd)
set(CMAKE_MINSIZEREL_POSTFIX rms)

set(CMAKE_POSTFIX "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    add_definitions(-DDEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(CMAKE_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX})
endif()

add_definitions(-DHUB_PROJECT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")

find_package(Boost QUIET) # tcp socket communication
if (Boost_FOUND)
    add_definitions(-DHUB_USE_BOOST)
#    target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_USE_BOOST)
endif()

find_package(OpenCV QUIET)
if (OpenCV_FOUND)
    add_definitions(-DHUB_USE_OPENCV)
    link_libraries(${OpenCV_LIBS})
endif()

find_package(Eigen3 QUIET)
if (Eigen3_FOUND)
#    message(FATAL_ERROR "ouou")
    add_definitions(-DHUB_USE_EIGEN3)
    link_libraries(Eigen3::Eigen)
    include_directories(${EIGEN3_INCLUDE_DIR})
endif()

find_package(TBB QUIET)
if (TBB_FOUND)
    add_definitions(-DHUB_USE_TBB)
    link_libraries(tbb)
endif()

find_package(PahoMqttCpp QUIET)
if (PahoMqttCpp_FOUND)
    add_definitions(-DHUB_USE_MQTT)
    link_libraries(PahoMqttCpp::paho-mqttpp3)
endif()

if (HUB_SERVER_PORT)
    add_definitions(-DHUB_SERVER_PORT=${HUB_SERVER_PORT})
endif()

# option(HUB_BUILD_SERVER "Build implement server" OFF)
# if (HUB_BUILD_SERVER)
#     add_definitions(-DHUB_BUILD_SERVER)
#     # target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_BUILD_SERVER)
#     # if (HUB_SERVER_PORT)
#         # target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
#     # endif()
# endif()


#find_package(OpenEXR QUIET) # tcp socket communication
#if (OpenEXR_FOUND)
##    message(FATAL_ERROR "error")
##    add_definitions(-DHUB_USE_OIMAGEIO)
#    link_libraries(OpenEXR)
##    link_libraries(${OpenImageIO_LIBRARIES})
#endif()

#find_package(OpenImageIO QUIET) # tcp socket communication
#if (OpenImageIO_FOUND)
###    message(FATAL_ERROR "error")
##    add_definitions(-DHUB_USE_OIMAGEIO)
#    link_libraries(OpenImageIO)
#    link_libraries(${OpenImageIO_LIBRARIES})
#    include_directories(${OpenImageIO_INCLUDE_DIRS})
##    link_libraries(Threads::Threads)
#endif()

set(HUB_USE_ZPP_BITS 0) # serializer
if (CMAKE_CXX_STANDARD GREATER_EQUAL 20)
#    option(HUB_USE_ZPP_BITS "Use zpp_bits.h for input/output serialization" ON)
    set(HUB_USE_ZPP_BITS 1)
    add_definitions(-DHUB_USE_ZPP_BITS)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# set(HUB_SOURCE_DIRS "core")
# set(HUB_SOURCE_DIRS "core" "net")
# set(HUB_SOURCE_DIRS "core" "data")
# set(HUB_SOURCE_DIRS "core" "net" "data")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "sensor")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "server")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "sensor" "server")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "sensor" "server" "client")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "sensor" "server" "client" "native")

# set(HUB_SOURCE_DIRS "core" "net" "io" "server")
set(HUB_SOURCE_DIRS "core" "net" "io" "server" "client")
# set(HUB_SOURCE_DIRS "core" "net" "data" "io" "server" "client" "native")


foreach(MODULE "core" "net" "data" "io" "sensor" "server" "client" "native")
    string(TOUPPER ${MODULE} UPPER_MODULE)
    set(HUB_BUILD_${UPPER_MODULE} 0)
endforeach()

if (HUB_BUILD_NATIVE)
    message(FATAL_ERROR "ou")
endif()

#set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
#set(HUB_SOURCE_DIR ".")
#set(HUB_SOURCE_DIRS "client") # io sensor
#set(HUB_SOURCE_DIRS "data") # sensor
#set(HUB_SOURCE_DIRS "io") # core
#set(HUB_SOURCE_DIRS "native")
#set(HUB_SOURCE_DIRS "sensor")
#set(HUB_SOURCE_DIRS "server")
#set(HUB_SOURCE_DIRS "utils") # core
#set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
#set(HUB_INCLUDE_DIR "net")
#set(HUB_INCLUDE_DIR ".")
#set(HUB_INCLUDE_DIR ${HUB_SOURCE_DIR})
foreach(HUB_SOURCE_DIR ${HUB_SOURCE_DIRS})
    string(REPLACE "/" "_" HUB_SOURCE_DIR "${HUB_SOURCE_DIR}")
    string(TOUPPER ${HUB_SOURCE_DIR} UPPER_SOURCE_DIR)
    set(HUB_BUILD_${UPPER_SOURCE_DIR} 1)
    add_definitions(-DHUB_BUILD_${UPPER_SOURCE_DIR})
endforeach()

set(HUB_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

add_subdirectory(src)

option(HUB_BUILD_DOC "Build documentation" OFF)
if(HUB_BUILD_DOC)
    add_subdirectory(doc)
endif()

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if(HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

#option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if(HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

add_subdirectory(bin)

if(NOT WIN32)
    add_custom_target(
        cppCheck
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND cppcheck --enable=all --error-exitcode=1 src tests -I src -I tests --inline-suppr --suppress=toomanyconfigs
                --suppress=missingIncludeSystem
        COMMENT "Running cppCheck")
    custom_target_added(cppCheck)
endif()

add_custom_target(
    format
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND scripts/format-all.sh
    COMMENT "Format sources")
custom_target_added(format)

add_custom_target(all-targets)
custom_target_added(all-targets)
foreach(target "tests" "coverage" "cppCheck" "doc-doxygen" "doc-sphinx")
    if(TARGET ${target})
        add_dependencies(all-targets ${target})
    endif()
endforeach()


option(HUB_ENABLE_WRAPPER "Enable wrapper" OFF)
if(HUB_ENABLE_WRAPPER)
    add_subdirectory(wrapper)
endif()

message(STATUS "Modules: ${HUB_SOURCE_DIRS}")

get_property(TMP GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED)
#message(STATUS "Custom targets: cppCheck format doc-doxygen doc-sphinx coverage coverage-lcov coverage-all tests")
message(STATUS "Custom targets:${TMP}")

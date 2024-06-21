cmake_minimum_required(VERSION 3.8)

project(hub)

include(cmake/Functions.cmake)
set_property(GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED "")

set(CMAKE_CXX_STANDARD 17) # std::any, AppleClang std::void_t<>, if constexpr
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

set(HEADER_MSG "${Yellow}[hub]${ColourReset}")
message(STATUS "${HEADER_MSG} CXX compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "${HEADER_MSG} Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "${HEADER_MSG} CMake generator: ${CMAKE_GENERATOR}")
message(STATUS "${HEADER_MSG} CMake system name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "${HEADER_MSG} CMake system version: ${CMAKE_SYSTEM_VERSION}")

# Looking for current system archictecture
set(ARCHITECTURE "")
if(CMAKE_GENERATOR_PLATFORM)
    message(STATUS "${HEADER_MSG} CMake generator platform: ${CMAKE_GENERATOR_PLATFORM}")
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
elseif(CMAKE_OSX_ARCHITECTURES)
    message(STATUS "${HEADER_MSG} CMake osx architectures: ${CMAKE_OSX_ARCHITECTURES}")
    set(ARCHITECTURE ${CMAKE_OSX_ARCHITECTURES})
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
message(STATUS "${HEADER_MSG} Architecture: ${ARCHITECTURE}")

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

if(WIN32)
    set(USERNAME $ENV{USERNAME})
else()
    set(USERNAME $ENV{USER})
endif()

# Looking for supported compiler
set(SUPPORTED_COMPILERS "Clang" "GNU" "LLVM" "MSVC" "AppleClang")
set(Clang_MINIMAL_VERSION 11.0.0)
set(GNU_MINIMAL_VERSION 8.1.0)
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
        add_definitions(-DHUB_COMPILER_${SUPPORTED_COMPILER})
    else()
        set("${SUPPORTED_COMPILER}" 0)
    endif()
endforeach()

if(LLVM)
    message(FATAL_ERROR "not supported")
endif()

if(NOT COMPILER_FOUND)
    message(FATAL_ERROR "no supported compiler founded")
endif()

# Set default value of CMake variables
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
message(STATUS "${HEADER_MSG} CMake build type: ${CMAKE_BUILD_TYPE}")

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

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_SOURCE_DIR}/install/${ARCHITECTURE}-${CMAKE_BUILD_TYPE}"
        CACHE PATH "Install path prefix." FORCE)
endif()

set(HUB_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DHUB_PROJECT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")

find_package(Boost QUIET) # tcp socket communication
if(Boost_FOUND)
    add_definitions(-DHUB_USE_BOOST)
    message(STATUS "${HEADER_MSG} Boost ${Boost_VERSION} found")
endif()

find_package(TBB QUIET)
set(HUB_TBB_TARGET)
if(TBB_FOUND)
    if(TARGET tbb)
        set(HUB_TBB_TARGET tbb)
    elseif(TARGET TBB::tbb)
        set(HUB_TBB_TARGET TBB::tbb)
    else()
        message(FATAL_ERROR "tbb target not found")
    endif()

    if (NOT GNU) # warning with gcc : [Parallel STL message]: "Vectorized algorithm unimplemented, redirected to serial"
        add_definitions(-DHUB_USE_TBB)
    endif()
endif()

find_package(PahoMqttCpp QUIET)
if(PahoMqttCpp_FOUND)
    add_definitions(-DHUB_USE_MQTT)
    link_libraries(PahoMqttCpp::paho-mqttpp3)
endif()

set(HUB_USE_ZPP_BITS 0) # serializer
if(CMAKE_CXX_STANDARD GREATER_EQUAL 20)
    set(HUB_USE_ZPP_BITS 1)
    add_definitions(-DHUB_USE_ZPP_BITS)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# cmake-format: off
# # set(HUB_MODULES "core")
# # set(HUB_MODULES "core" "net")
# # set(HUB_MODULES "core" "net" "io")
# # set(HUB_MODULES "core" "net" "io" "server")
# # set(HUB_MODULES "core" "net" "io" "server" "client")
# # set(HUB_MODULES "core" "net" "io" "server" "client" "native")
# # set(HUB_MODULES "core" "net" "io" "server" "client" "native" "sensor")
set(HUB_MODULES "core" "net" "io" "server" "client" "native" "sensor" "data")

set(ALL_HUB_MODULES "core" "net" "io" "server" "client" "native" "sensor" "data")
# cmake-format: on

foreach(MODULE ${ALL_HUB_MODULES})
    string(TOUPPER ${MODULE} UPPER_MODULE)
    if(${MODULE} IN_LIST HUB_MODULES)
        set(HUB_BUILD_${UPPER_MODULE} 1)
    else()
        message(STATUS "${HEADER_MSG} ${Red}module ${MODULE}${ColourReset} disabled")
        add_definitions(-DHUB_NON_BUILD_${UPPER_MODULE})
        set(HUB_BUILD_${UPPER_MODULE} 0)
    endif()
endforeach()

set(HUB_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")
set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
    set(HUB_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
else()
    set(HUB_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

add_subdirectory(src)

option(HUB_BUILD_DOC "Build documentation" OFF)
if(HUB_BUILD_DOC)
    add_subdirectory(doc)
endif()

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if(HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if(HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

option(HUB_ENABLE_BINS "Enable binaries" ON)
if(HUB_ENABLE_BINS)
    add_subdirectory(bin)
endif()

find_package(cppcheck QUIET)
if(cppcheck_FOUND)
    execute_process(
        COMMAND ${CPPCHECK_EXECUTABLE} --version
        RESULT_VARIABLE CPP_CHECK_FOUND
        OUTPUT_VARIABLE CPP_CHECK_OUTPUT
        ERROR_VARIABLE CPP_CHECK_ERROR
    )

    string(REGEX MATCHALL "-.*$|[0-9]+" CPP_CHECK_VERSION_LIST ${CPP_CHECK_OUTPUT})
    list(GET CPP_CHECK_VERSION_LIST 0 CPP_CHECK_VERSION_MAJOR)
    list(GET CPP_CHECK_VERSION_LIST 1 CPP_CHECK_VERSION_MINOR)
    # list(GET CPP_CHECK_VERSION_LIST 2 CPP_CHECK_VERSION_PATCH)

    message(STATUS "${HEADER_MSG} Cppcheck ${CPP_CHECK_VERSION_MAJOR}.${CPP_CHECK_VERSION_MINOR} found")

    if(NOT ${CPP_CHECK_VERSION_MAJOR} EQUAL 2)
        message(FATAL_ERROR "${CPP_CHECK_OUTPUT} unsupported")
    endif()

    if (${CPP_CHECK_VERSION_MINOR} GREATER 10)
        add_custom_target(
            hub-cppCheck
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND
                cppcheck -DCPP_CHECK src -I src --error-exitcode=1 --enable=all --suppress=missingIncludeSystem
                --suppress=missingInclude --suppress=unusedFunction --suppress=virtualCallInConstructor --inline-suppr
            COMMENT "Running cppCheck")
    else()
        add_custom_target(
            hub-cppCheck
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND
                cppcheck -DCPP_CHECK src -I src --error-exitcode=1 --enable=all
                --suppress=missingInclude --suppress=unusedFunction --suppress=virtualCallInConstructor --inline-suppr
            COMMENT "Running cppCheck")
    endif()
    custom_target_added(cppCheck)

endif()

add_custom_target(
    hub-format
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND scripts/format-all.sh
    COMMENT "Format sources")
custom_target_added(format)

option(HUB_ENABLE_WRAPPER "Enable wrapper" OFF)
if(HUB_ENABLE_WRAPPER)
    add_subdirectory(wrapper)
endif()

message(STATUS "${HEADER_MSG} Modules: (${HUB_MODULES})")

get_property(TMP GLOBAL PROPERTY CUSTOM_TARGETS_FOUNDED)
message(STATUS "${HEADER_MSG} Custom targets:${TMP}")

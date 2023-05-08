cmake_minimum_required(VERSION 3.8)

project(hub)

set(HUB_IS_ROOT 0)
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(HUB_IS_ROOT 1)
endif()

#set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

#get_property(LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
#message(STATUS "CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}" )
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")

# Finding archictecture
set (ARCHITECTURE "")
if (CMAKE_GENERATOR_PLATFORM)
	message(STATUS "CMake generator platform: ${CMAKE_GENERATOR_PLATFORM}")
	if (${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
		set(ARCHITECTURE "x64")
	elseif (${CMAKE_GENERATOR_PLATFORM} STREQUAL "Win32")
		set(ARCHITECTURE "x86")
	elseif (${CMAKE_GENERATOR_PLATFORM} STREQUAL "ARM64")
                set(ARCHITECTURE "arm64")
	elseif (${CMAKE_GENERATOR_PLATFORM} STREQUAL "ARM")
                set(ARCHITECTURE "arm")
	else()
            message(FATAL_ERROR "unknown platform")
	endif()
else()
	if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		set(ARCHITECTURE "x86")
	else()
		set(ARCHITECTURE "x64")
	endif()
endif()
#if (HUB_IS_ROOT)
    message(STATUS "Architecture: ${ARCHITECTURE}")
#endif()

foreach(SYSTEM_INFO IN ITEMS "HOSTNAME" "TOTAL_PHYSICAL_MEMORY" "PROCESSOR_DESCRIPTION" "OS_NAME" "OS_RELEASE" "OS_VERSION" "OS_PLATFORM")
    cmake_host_system_information(RESULT tmp QUERY ${SYSTEM_INFO})
#    message(STATUS "${SYSTEM_INFO} : ${tmp}")
    set(${SYSTEM_INFO} ${tmp})
endforeach()

# Looking for compatible compiler
set(SUPPORTED_COMPILERS "Clang" "GNU" "LLVM" "MSVC")
set(Clang_MINIMAL_VERSION 11.0.0)
set(GNU_MINIMAL_VERSION 8.1.0)
set(LLVM_MINIMAL_VERSION 0)
set(MSVC_MINIMAL_VERSION 19)

set(COMPILER_FOUND)
foreach (SUPPORTED_COMPILER ${SUPPORTED_COMPILERS})
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "${SUPPORTED_COMPILER}")
      if (COMPILER_FOUND)
        message(FATAL_ERROR "too many compilers founded ${COMPILER_FOUND} and ${SUPPORTED_COMPILER}")
      endif()
      if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS ${${SUPPORTED_COMPILER}_MINIMAL_VERSION})
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be ${${SUPPORTED_COMPILER}_MINIMAL_VERSION} or greater! Aborting...")
      endif()
      set(COMPILER_FOUND ${SUPPORTED_COMPILER})
      set("${SUPPORTED_COMPILER}" 1)
  else()
      set("${SUPPORTED_COMPILER}" 0)
  endif()
endforeach()

if (NOT COMPILER_FOUND)
    message(FATAL_ERROR "no compiler founded")
endif()

# set default value of CMake variables
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

#if(NOT CMAKE_INSTALL_PREFIX OR CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
#if(NOT CMAKE_INSTALL_PREFIX)
if (HUB_IS_ROOT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "Install path prefix." FORCE)
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_POSTFIX "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

add_definitions(-DHUB_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HUB_EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

add_subdirectory(external)
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

if (NOT WIN32)
        add_custom_target(
            cppCheck
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND cppcheck --enable=all --error-exitcode=1 src tests -I src --inline-suppr
                    --suppress=toomanyconfigs --suppress=missingIncludeSystem
            COMMENT "Running cppCheck")
endif()

add_custom_target(
    format
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND scripts/format-all.sh
    COMMENT "Format sources")

add_custom_target(all-targets)
foreach(target "tests" "coverage" "cppCheck" "doc-doxygen" "doc-sphinx")
    if(TARGET ${target})
        add_dependencies(all-targets ${target})
    endif()
endforeach()

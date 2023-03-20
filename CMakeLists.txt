cmake_minimum_required(VERSION 3.8)

project(hub)

#set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

#get_property(LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
#message(STATUS "CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER}" )
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

set(CLANG 0)
set(GCC 0)
set(LLVM 0)
if(${CMAKE_CXX_COMPILER_ID} MATCHES "(Apple)?[Cc]lang")
  set(CLANG 1)
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 11.0.0)
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be 12.0.0 or greater! Aborting...")
  endif()

elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
  set(GCC 1)
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 8.1.0)
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be 8.1.0 or greater! Aborting...")
  endif()

elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "(LLVM)?[Ff]lang")
  set(LLVM 1)
  if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 0)
    message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be 0.0.0 or greater! Aborting...")
  endif()

else()
    if (NOT MSVC)
        message(FATAL_ERROR "unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()

    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 19)
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} version must be 0.0.0 or greater! Aborting...")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_POSTFIX "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

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

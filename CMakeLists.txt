cmake_minimum_required(VERSION 3.8)

project(hub)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

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
add_subdirectory(server)

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

add_custom_target(
    cppCheck
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND cppcheck --enable=all --error-exitcode=1 src tests server -I src -I server/libServer/ --inline-suppr
            --suppress=toomanyconfigs --suppress=missingIncludeSystem
    COMMENT "Running cppCheck")

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

cmake_minimum_required(VERSION 3.8)

project(hub)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 17) # depends of std::any for metadata representation
#set(CMAKE_CXX_STANDARD 20) # experimental constexpr
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_POSTFIX "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources/")
add_definitions(-DRESOURCES_DIR=\"${RESOURCES_DIR}\")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
#if (NOT CMAKE_INSTALL_PREFIX)
#	set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE})
#endif()

set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core/include")
set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/core/src")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

#option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

#option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
#if (HUB_ENABLE_COVERAGE)
#    include(CodeCoverage)
#    append_coverage_compiler_flags()
#    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
#endif()

add_subdirectory(core)
add_subdirectory(server)
add_subdirectory(doc)
add_subdirectory(api)

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if(HUB_ENABLE_TESTS)
#    set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage -DBUILD_TEST")
#    set(CMAKE_C_FLAGS "-g -fprofile-arcs -ftest-coverage -DBUILD_TEST")
    add_subdirectory(tests)
endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support UWP compilation : cmake
# -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

############################################### COVERAGE


#if (HUB_ENABLE_TESTS AND HUB_ENABLE_COVERAGE)
#    setup_target_for_coverage_lcov(
##    setup_target_for_coverage_gcovr_xml(
#        NAME coverage
##        BASE_DIRECTORY "CMakeFiles"
#        EXECUTABLE ctest
#        --test-dir tests
#        -j ${n_cores}
#        DEPENDENCIES
#        hub
#        server
#        #            test-sensorSpec
#        test-all
#        NO_DEMANGLE
#        EXCLUDE
#        "/usr/*"
#        "examples/*"
##        "include/*"
#    )
#    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
#        file(COPY ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl
#            DESTINATION ${CMAKE_BINARY_DIR}/
#        )
#    endif()

##            setup_target_for_coverage_gcovr_html(NAME coverage-html
##                EXECUTABLE ctest --schedule-random -j ${n_cores} --test-dir tests
##                EXCLUDE "/usr/*"
##                DEPENDENCIES test-file test-measures test-ram test-resolutions test-sensorSpec test-server test-serverSpeed
##            )

#endif()

############################################### EXAMPLES

option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if(HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

#file(COPY
#    ${PROJECT_SOURCE_DIR}/scripts/run_all_targets.sh DESTINATION ${CMAKE_BINARY_DIR}/
#)
add_custom_target(
    cppCheck
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND cppcheck --enable=all --error-exitcode=1 core/src tests server -I core/include -I server/libServer/ --suppress=toomanyconfigs --suppress=missingIncludeSystem
#    COMMENT "Generating API documentation with Doxygen"
#    VERBATIM
    COMMENT "Running cppCheck"
)

add_custom_target(
    format
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND scripts/format-all.sh
#    COMMENT "Generating API documentation with Doxygen"
#    VERBATIM
    COMMENT "Format sources"
)

add_custom_target(all-targets)
foreach(target "tests" "coverage" "cppCheck" "doc-doxygen" "doc-sphinx" "format")
#    message(STATUS "${target}")
    if (TARGET ${target})
        add_dependencies(all-targets ${target})
    endif()
endforeach()
#add_dependencies(all-targets tests coverage cppCheck doc-doxygen doc-sphinx format)

if(HUB_ENABLE_DOC OR HUB_ENABLE_COVERAGE)
    set(HUB_BROWSER "chromium" CACHE STRING "User browser to see html docs and coverage")
endif()

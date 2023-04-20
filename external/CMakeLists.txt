#cmake_minimum_required(VERSION 3.5)

#project(tests LANGUAGES CXX)

#option(MESHOPT_BUILD_DEMO "Build demo" OFF)
#option(MESHOPT_BUILD_GLTFPACK "Build gltfpack" OFF)
#set(MESHOPT_BUILD_SHARED_LIBS ON CACHE INTERNAL "Build shared libraries")

#include(FetchContent)
#FetchContent_Declare(meshoptimizer
#    GIT_REPOSITORY    https://github.com/zeux/meshoptimizer.git
#    GIT_TAG v0.18 # or a later release
#    GIT_PROGRESS TRUE
##    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/external/meshoptimizer
##    SUBBUILD_DIR      subbuild
##    BINARY_DIR        build
#)
##FetchContent_QUIET(meshoptimizer ON)
##option(WITH_GFLAGS "" OFF)
##option(WITH_GTEST "" OFF)
##option(WITH_GMOCK "" OFF)
##option(WITH_UNWIND "" OFF)
##option(BUILD_SHARED_LIBS "" OFF)
##option(BUILD_TESTING "" OFF)
#FetchContent_MakeAvailable(meshoptimizer)

#....

#add_library( STATIC)
#target_link_libraries(libsomething PUBLIC glog::glog)

# find_package(Catch2 QUIET)
# if(NOT ${Catch2_FOUND})
#     include(FetchContent)
#     FetchContent_Declare(
#         Catch2
#         GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#         GIT_TAG v3.0.1 # or a later release
#         GIT_PROGRESS TRUE)
#     FetchContent_MakeAvailable(Catch2)
# endif()
#
# file(GLOB_RECURSE test_srcs *.cpp)


# add_executable(test-all ${test_srcs})
# target_link_libraries(test-all Catch2::Catch2WithMain hub)
# target_include_directories(test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
# set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)
#
#file(GLOB testFiles *.cpp)



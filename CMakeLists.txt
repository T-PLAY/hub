cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

project(Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
set(SRC_TARGET hub)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
if (NOT CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE})
endif()

option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

add_subdirectory(src)

add_subdirectory(applications)

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if(HUB_ENABLE_TESTS)
    add_subdirectory(tests)
endif()

option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if(HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

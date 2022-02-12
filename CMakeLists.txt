cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (Hub)

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)

add_subdirectory(src)
add_subdirectory(applications)

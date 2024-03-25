cmake_minimum_required(VERSION 3.5)

project(doc-examples)

file(GLOB_RECURSE DOC_SRC *.cpp)

add_library(${PROJECT_NAME} ${DOC_SRC})
target_link_libraries(${PROJECT_NAME} hub)


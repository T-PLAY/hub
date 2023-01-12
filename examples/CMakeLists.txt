cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

file(GLOB_RECURSE EXAMPLE_SRC *.cpp)

string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}")
set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

foreach(name ${EXAMPLE_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME example-${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
#    target_link_libraries(${TARGET_NAME} hub libServer)
    target_link_libraries(${TARGET_NAME} hub)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
endforeach()

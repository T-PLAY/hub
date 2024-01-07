cmake_minimum_required(VERSION 3.4...3.18)
project(Hub LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(PYBIND11_PYTHON_VERSION 3.6)
set(CMAKE_DEBUG_POSTFIX "")

find_package(pybind11 QUIET)
if(NOT ${pybind11_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.10.4 # or a later release
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(pybind11)
endif()

message(STATUS "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
message(STATUS " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
message(STATUS " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

find_package(Eigen3 REQUIRED)
find_package(
    Python 3.6
    COMPONENTS Interpreter Development
    REQUIRED)

pybind11_add_module(${PROJECT_NAME} pywrap.cpp test.py)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYBIND11_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen hub_static)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/test.py" $<TARGET_FILE_DIR:${PROJECT_NAME}>)

file(GLOB_RECURSE tests *.py)

add_custom_target(
    wrapper-python-test # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ${PYTHON_EXECUTABLE} test.py
    COMMENT "Running python tests")

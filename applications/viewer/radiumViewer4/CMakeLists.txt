cmake_minimum_required(VERSION 3.13)
if(APPLE)
    cmake_policy(SET CMP0042 NEW)
endif(APPLE)

project(radiumViewer4 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(NOT IN_RADIUM_BUILD_TREE)
    find_package(Radium REQUIRED Core Engine Gui)
endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE app_sources *.cpp)
file(GLOB_RECURSE app_uis *.ui)
file(GLOB_RECURSE app_headers *.h *.hpp)

add_executable(${PROJECT_NAME} ${app_sources} ${app_uis} ${app_headers})

target_link_libraries(${PROJECT_NAME} PUBLIC Radium::Core Radium::Engine Radium::Gui
                                             ${Qt_LIBRARIES})

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    PUBLIC ${DEFAULT_COMPILE_OPTIONS}
    INTERFACE)

configure_radium_app(NAME ${PROJECT_NAME})

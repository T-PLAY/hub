cmake_minimum_required(VERSION 3.13)
if(APPLE)
    cmake_policy(SET CMP0042 NEW)
endif(APPLE)

project(app-viewer-TexturedQuad VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE}"
        CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
    message("Set install prefix to ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(NOT IN_RADIUM_BUILD_TREE)
    find_package(Radium REQUIRED Core Engine Gui)
endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(app_sources main.cpp vertexShader.glsl fragmentShader.glsl)

add_executable(${PROJECT_NAME} ${app_sources})

target_link_libraries(${PROJECT_NAME} PUBLIC Radium::Core Radium::Engine Radium::Gui Radium::IO
                                             ${Qt_LIBRARIES} DicomLoader)

configure_radium_app(NAME ${PROJECT_NAME})

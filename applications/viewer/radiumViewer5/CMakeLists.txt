cmake_minimum_required(VERSION 3.8)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
    cmake_policy(SET CMP0071 NEW)
endif()

project(radiumViewer5)
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}"
        CACHE PATH "Install path prefix, prepended onto install directories." FORCE)
    message("Set install prefix to ${CMAKE_INSTALL_PREFIX}")
endif()

find_package(
    Radium
    REQUIRED
    Core
    Engine
    Gui
    PluginBase
    IO)

find_package(
    QT NAMES Qt6 Qt5
    COMPONENTS Core Widgets OpenGL
    REQUIRED)
find_package(
    Qt${QT_VERSION_MAJOR}
    COMPONENTS Widgets
    REQUIRED)

set(Qt_LIBRARIES Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets
                 Qt${QT_VERSION_MAJOR}::OpenGL)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE app_sources *.cpp)
file(GLOB_RECURSE app_uis *.ui)
file(GLOB_RECURSE app_headers *.h *.hpp)

if(${QT_VERSION_MAJOR} EQUAL 5)
    qt5_wrap_ui(app_uis_moc ${app_uis})
else()
    qt6_wrap_ui(app_uis_moc ${app_uis})
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR} # Moc
)

add_executable(
    ${PROJECT_NAME} MACOSX_BUNDLE ${app_sources} ${app_headers} ${app_inlines}
                                  ${app_uis_moc} # ${app_resources}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC Radium::Core
           Radium::Engine
           Radium::Gui
           Radium::IO
           ${Qt_LIBRARIES}
           DicomLoader
           libGuiManager)

configure_radium_app(NAME ${PROJECT_NAME} USE_PLUGINS)

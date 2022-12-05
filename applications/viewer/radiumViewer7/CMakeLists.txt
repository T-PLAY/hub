cmake_minimum_required(VERSION 3.8)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
    cmake_policy(SET CMP0071 NEW)
endif()

project(radiumViewer7)
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
    Qt${QT_VERSION}
    COMPONENTS Core Widgets OpenGL
    REQUIRED)
set(Qt_LIBRARIES Qt${QT_VERSION}::Core Qt${QT_VERSION}::Widgets Qt${QT_VERSION}::OpenGL)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(app_sources main.cpp MainApplication.cpp Gui/ColorWidget.cpp Gui/MainWindow.cpp
                Gui/MaterialEditor.cpp Gui/TransformEditorWidget.cpp)

set(app_headers
    MainApplication.hpp
    Gui/ColorWidget.hpp
    Gui/MainWindow.hpp
    Gui/MaterialEditor.hpp
    Gui/RotationEditor.hpp
    Gui/TransformEditorWidget.hpp
    Gui/VectorEditor.hpp)

set(app_uis Gui/ui/MainWindow.ui Gui/ui/MaterialEditor.ui Gui/ui/RotationEditor.ui
            Gui/ui/VectorEditor.ui)

if(${QT_VERSION} EQUAL 5)
    qt5_wrap_ui(app_uis_moc ${app_uis})
elseif(${QT_VERSION} EQUAL 6)
    qt6_wrap_ui(app_uis_moc ${app_uis})
endif()

set(app_resources Resources/ApplicationIcons.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_BINARY_DIR} # Moc
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${app_sources} ${app_headers} ${app_inlines}
                                             ${app_uis_moc} ${app_resources})

target_link_libraries(${PROJECT_NAME} PUBLIC Radium::Core Radium::Engine Radium::Gui Radium::IO
                                             ${Qt_LIBRARIES} libGuiManager)

configure_radium_app(NAME ${PROJECT_NAME} USE_PLUGINS)

# Build Radium Sandbox
cmake_minimum_required(VERSION 3.8)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
    cmake_policy(SET CMP0071 NEW)
endif()

#-------------------------------------------------------------------------------
# exampleApp executables setup
project(radiumViewer5)
# Set default install location to installed-<Compiler_ID> folder in build dir
# we do not want to install to /usr by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}" CACHE PATH
            "Install path prefix, prepended onto install directories." FORCE )
    message( "Set install prefix to ${CMAKE_INSTALL_PREFIX}" )
endif()

find_package( Radium REQUIRED Core Engine Gui PluginBase IO)

#------------------------------------------------------------------------------
# Application specific


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


#find_package(Qt6 COMPONENTS Core Widgets OpenGL REQUIRED)
set( Qt_LIBRARIES Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE app_sources *.cpp)
file(GLOB_RECURSE app_uis *.ui)
file(GLOB_RECURSE app_headers *.h *.hpp)

if (${QT_VERSION_MAJOR} EQUAL 5)
    qt5_wrap_ui(app_uis_moc ${app_uis})
else()
    qt6_wrap_ui(app_uis_moc ${app_uis})
endif()

#set(app_resources
#        Resources/ApplicationIcons.qrc
#   )


set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR} # Moc
    )

add_executable(
    ${PROJECT_NAME} MACOSX_BUNDLE
    ${app_sources}
    ${app_headers}
    ${app_inlines}
    ${app_uis_moc}
#    ${app_resources}
    )

target_link_libraries (${PROJECT_NAME} PUBLIC
    Radium::Core
    Radium::Engine
    Radium::Gui
    Radium::IO
    ${Qt_LIBRARIES})

configure_radium_app(
    NAME ${PROJECT_NAME}
    USE_PLUGINS
)

# radium_cotire( ${app_target} )

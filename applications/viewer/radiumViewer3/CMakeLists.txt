

cmake_minimum_required(VERSION 3.13)
if(APPLE)
    cmake_policy(SET CMP0042 NEW)
endif(APPLE)

project(radiumViewer3 VERSION 1.0.0)


# ------------------------------------------------------------------------------
# set wanted application defaults for cmake settings
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
# Set default install location to installed-<Compiler_ID> folder in build dir we do not want to
# install to /usr by default
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE}"
        CACHE PATH "Install path prefix, prepended onto install directories." FORCE
    )
    message("Set install prefix to ${CMAKE_INSTALL_PREFIX}")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# ------------------------------------------------------------------------------
if(NOT IN_RADIUM_BUILD_TREE)
    find_package(Radium REQUIRED Core Engine Gui)
endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# ------------------------------------------------------------------------------

file(GLOB_RECURSE app_sources *.cpp)
file(GLOB_RECURSE app_uis *.ui)
file(GLOB_RECURSE app_headers *.h *.hpp)
set(PROJECT_SOURCES
    ${app_sources}
    ${app_uis}
    ${app_headers}
)

# to install the app as a redistribuable bundle on macos, add MACOSX_BUNDLE when calling
# add_executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(
    ${PROJECT_NAME} PRIVATE Radium::Core Radium::Engine Radium::Gui ${Qt_LIBRARIES}
)

target_compile_options(${PROJECT_NAME} PRIVATE PUBLIC ${DEFAULT_COMPILE_OPTIONS} INTERFACE)

configure_radium_app(NAME ${PROJECT_NAME})



#cmake_minimum_required(VERSION 3.5)

##project(radiumViewer3 VERSION 0.1 LANGUAGES CXX)
#project(radiumViewer3 VERSION 1.0 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)



#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

#find_package(Radium REQUIRED Core Engine Gui)

#file(GLOB_RECURSE app_sources *.cpp)
#file(GLOB_RECURSE app_uis *.ui)
#file(GLOB_RECURSE app_headers *.h *.hpp)
#set(PROJECT_SOURCES
#    ${app_sources}
#    ${app_uis}
#    ${app_headers}
#)

#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(${PROJECT_NAME}
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(${PROJECT_NAME} SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(${PROJECT_NAME}
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()

##target_include_directories(${PROJECT_NAME} PRIVATE
##        ${RADIUM_INCLUDE_DIRS}
##)

#target_link_libraries(${PROJECT_NAME} PUBLIC
#    Qt${QT_VERSION_MAJOR}::Widgets
#    Radium::Core Radium::Engine Radium::Gui
#)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#    MACOSX_BUNDLE TRUE
#    WIN32_EXECUTABLE TRUE
#)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(${PROJECT_NAME})
#endif()


#target_compile_options(${PROJECT_NAME} PRIVATE PUBLIC ${DEFAULT_COMPILE_OPTIONS} INTERFACE)

#configure_radium_app(NAME ${PROJECT_NAME})

cmake_minimum_required(VERSION 3.5)

project(
    radiumViewer6
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


set(REQUIRED_LIBS Core Widgets Gui Xml)
set(REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets
                            Qt${QT_VERSION}::Xml)
find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_LIBS})

set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)


find_package(Radium REQUIRED COMPONENTS Core Engine Gui IO NO_DEFAULT_PATH)

message(STATUS ${RadiumDIR})

file(GLOB_RECURSE app_sources *.cpp *.glsl)
file(GLOB_RECURSE app_uis *.ui)
file(GLOB_RECURSE app_headers *.h *.hpp)
set(PROJECT_SOURCES ${app_sources} ${app_uis} ${app_headers})

if(${QT_VERSION} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
    set_property(
        TARGET ${PROJECT_NAME}
        APPEND
        PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
        set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()

        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()
endif()


if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt${QT_VERSION}::QWindowsIntegrationPlugin>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/)

    foreach(QT_LIB ${REQUIRED_LIBS})
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION}::${QT_LIB}>
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach(QT_LIB)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC # Qt${QT_VERSION}::Widgets
           ${REQUIRED_LIBS_QUALIFIED} Radium::Core Radium::Gui Radium::Engine Radium::IO
           libGuiManager
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE)

if(QT_VERSION EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE
    PUBLIC ${DEFAULT_COMPILE_OPTIONS}
    INTERFACE)

configure_radium_app(NAME ${PROJECT_NAME})












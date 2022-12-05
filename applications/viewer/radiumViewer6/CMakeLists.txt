cmake_minimum_required(VERSION 3.5)

project(
    app-viewer-radium
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

set(REQUIRED_QT_LIBS Core Widgets Gui Xml)
#set(REQUIRED_QT_LIBS_QUALIFIED "")
list(APPEND REQUIRED_QT_LIBS_QUALIFIED "")
foreach(QT_LIB ${REQUIRED_QT_LIBS})
#    set(REQUIRED_QT_LIBS_QUALIFIED "${REQUIRED_QT_LIBS_QUALIFIED} Qt${QT_VERSION}::${QT_LIB}")
    list(APPEND REQUIRED_QT_LIBS_QUALIFIED "Qt${QT_VERSION}::${QT_LIB}")
endforeach()
#set(REQUIRED_QT_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets
#                            Qt${QT_VERSION}::Xml)
find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_QT_LIBS})

set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

set(REQUIRED_RADIUM_LIBS Core Engine Gui IO)
#find_package(Radium REQUIRED COMPONENTS Core Engine Gui IO NO_DEFAULT_PATH)
find_package(Radium REQUIRED COMPONENTS ${REQUIRED_RADIUM_LIBS} NO_DEFAULT_PATH)

#message(STATUS ${RadiumDIR})

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

    foreach(QT_LIB ${REQUIRED_QT_LIBS})
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION}::${QT_LIB}>
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach(QT_LIB)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)


    foreach(RADIUM_LIB ${REQUIRED_RADIUM_LIBS})
        add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Radium::${RADIUM_LIB}>
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()

#    message(STATUS "radium-external-dlls ${RadiumExternalDlls_location}")
#    set(DLL_PATHS "")
    add_imported_dir(FROM cpplocate::cpplocate TO RadiumExternalDllPath)

    foreach(RADIUM_EXTERNAL_DLL_PATH ${RadiumExternalDllPath})
#        message("DLL_LOCATION ${DLL_LOCATION}")

    #    set (DLLS "")
        file(GLOB RADIUM_EXTERNAL_DLLS "${RADIUM_EXTERNAL_DLL_PATH}/*.dll")
#	message(STATUS "DLLS ${DLLS}")
        foreach(RADIUM_EXTERNAL_DLL ${RADIUM_EXTERNAL_DLLS})
            add_custom_command(
                    TARGET ${PROJECT_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy ${RADIUM_EXTERNAL_DLL}
                            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endforeach()

    endforeach()

#    set(RADIUM_EXTERNAL_LIBS cpplocate::cpplocate glbinding::glbinding globjects::globjects tinyply PowerSlider::PowerSlider tinyEXR glbinding-aux)
    set(RADIUM_OTHER_LIBS Radium::PluginBase)
#    message(STATUS "RadiumExternals ${RADIUM_EXTERNAL_LIBS}")

    foreach(RADIUM_OTHER_LIB ${RADIUM_OTHER_LIBS})
#    foreach(RADIUM_OTHER_LIB ${RadiumExternalDlls_location})
        add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RADIUM_OTHER_LIB}>
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach()

#    message(STATUS "${RADIUM_ROOT_DIR}")
    ## copy resources path from radium install dir
        add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory "${RADIUM_ROOT_DIR}/Resources"
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC # Qt${QT_VERSION}::Widgets
           ${REQUIRED_QT_LIBS_QUALIFIED} Radium::Core Radium::Gui Radium::Engine Radium::IO
           libGuiManager)

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

cmake_minimum_required (VERSION 3.8)

project(radiumViewer)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# ------------------------------------------------------------------------------
#if(NOT IN_RADIUM_BUILD_TREE)
find_package(Radium REQUIRED Core Engine Gui)
#endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# ------------------------------------------------------------------------------



#if (WIN32)
#    set(QT_VERSION 5)
#else()
#    set(QT_VERSION 6)
#endif()
#set(QT_LIBS Core Widgets Gui)
#set(QT_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Widgets)
#find_package(Qt${QT_VERSION} COMPONENTS ${QT_LIBS} REQUIRED)


file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

set(app_uis)
qt_wrap_ui(app_uis_moc ${app_uis})


add_executable(${PROJECT_NAME}
    ${sources}
    ${headers}
    ${app_uis}
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME} PRIVATE ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
                            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC src Radium::Core Radium::Engine Radium::Gui ${Qt_LIBRARIES}
)

configure_radium_app(NAME ${PROJECT_NAME})


#target_include_directories(${PROJECT_NAME} PUBLIC
#    ${SRC_DIR}
#    ${PROJECT_SOURCE_DIR}
#    )

#if (WIN32)

#    add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        $<TARGET_FILE:Qt${QT_VERSION}::QWindowsIntegrationPlugin>
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
#        )

#    foreach (QT_LIB ${QT_LIBS})
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION}::${QT_LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#            )
#    endforeach (QT_LIB)

#    foreach (RADIUM_LIB ${RADIUM_LIBS})
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Radium::${RADIUM_LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#            )
#    endforeach (RADIUM_LIB)

#    set(RADIUM_DEP_LIBS
#        globjects::globjects
#        glbinding::glbinding
#        glbinding::glbinding-aux
#        cpplocate::cpplocate
#        Qt5::Xml
#        tinyply
#        )

#    foreach(RADIUM_DEP_LIB ${RADIUM_DEP_LIBS})
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${RADIUM_DEP_LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#            )
#    endforeach()

#    set(RADIUM_FILES
#        ${RadiumExternalDlls_location}/assimp-vc142-mtd.dll
#        )
#    foreach(RADIUM_FILE ${RADIUM_FILES})
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RADIUM_FILE} $<TARGET_FILE_DIR:${PROJECT_NAME}>
#            )
#    endforeach()

#    set(RADIUM_DIRS
#        ${RadiumExternalDlls_location}/../Resources
#        )
#    foreach(RADIUM_DIR ${RADIUM_DIRS})
#        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_directory ${RADIUM_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
#            )
#    endforeach()


#elseif(UNIX)
#    #        find_package(Threads REQUIRED)
#    #        set(UNIX_REQUIRED_LIBS Threads::Threads)
#endif()


#target_link_libraries(${PROJECT_NAME} src ${RADIUM_LIBS_QUALIFIED} ${QT_LIBS_QUALIFIED} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


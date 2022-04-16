
cmake_minimum_required (VERSION 3.8)

project(radiumViewer3)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Network Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Network Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets)
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE forms *.ui)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME}
    ${sources}
    ${forms}
    ${headers}
)


target_include_directories(${PROJECT_NAME} PUBLIC
    ${SRC_DIR}
    ${PROJECT_SOURCE_DIR}
)

if (WIN32)
    #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)

    message(STATUS ${QT_INSTALL_PATH})
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION}::QWindowsIntegrationPlugin>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
    )

foreach (QT_LIB ${REQUIRED_LIBS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION}::${QT_LIB}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach (QT_LIB)

elseif(UNIX)
    #    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    #    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} src ${REQUIRED_LIBS_QUALIFIED} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})





#cmake_minimum_required(VERSION 3.6)
#if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
#    cmake_policy(SET CMP0071 NEW)
#endif()
#if(APPLE)
#    cmake_policy(SET CMP0042 NEW)
#endif(APPLE)

#project(radiumViewer3 VERSION 1.0.0)

## ------------------------------------------------------------------------------
## set wanted application defaults for cmake settings
#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif()
## Set default install location to installed-<Compiler_ID> folder in build dir we do not want to
## install to /usr by default
#if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
#    set(CMAKE_INSTALL_PREFIX
#        "${CMAKE_CURRENT_BINARY_DIR}/installed-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE}"
#        CACHE PATH "Install path prefix, prepended onto install directories." FORCE
#    )
#    message("Set install prefix to ${CMAKE_INSTALL_PREFIX}")
#endif()
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_DISABLE_SOURCE_CHANGES ON)
#set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

## ------------------------------------------------------------------------------
#if(NOT IN_RADIUM_BUILD_TREE)
#    find_package(Radium REQUIRED Core Engine Gui IO)
#endif()

#find_qt_package(COMPONENTS Core Widgets REQUIRED)
#set(Qt_LIBRARIES Qt::Core Qt::Widgets)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
## ------------------------------------------------------------------------------

#file(GLOB_RECURSE app_sources *.cpp)
#file(GLOB_RECURSE app_headers *.h *.hpp)
#file(GLOB_RECURSE app_uis *.ui)

##set(app_uis)
##qt_wrap_ui(app_uis_moc ${app_uis})

#set(app_resources)

## to install the app as a redistribuable bundle on macos, add MACOSX_BUNDLE when calling
## add_executable
#add_executable(
#    ${PROJECT_NAME} MACOSX_BUNDLE ${app_sources} ${app_headers} ${app_uis} ${app_resources}
#)

#target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
#target_include_directories(
#    ${PROJECT_NAME} PRIVATE ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
#                            ${CMAKE_CURRENT_SOURCE_DIR}
#)

#target_link_libraries(
#    ${PROJECT_NAME} PUBLIC Radium::Core Radium::Engine Radium::Gui Radium::IO ${Qt_LIBRARIES}
#)

#configure_radium_app(NAME ${PROJECT_NAME})

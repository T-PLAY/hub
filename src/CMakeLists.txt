cmake_minimum_required (VERSION 3.8)

#project ("HubV2")
project(src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Network Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
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
    )

if (WIN32)
    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB ${REQUIRED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)

elseif(UNIX)
#    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
#    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_THREAD_LIBS_INIT})
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


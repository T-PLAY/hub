cmake_minimum_required (VERSION 3.8)

project(streamViewer)

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
        #    set(DEBUG_SUFFIX)
        #    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        #        set(DEBUG_SUFFIX "d")
        #    endif ()
        #    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
        #    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        #        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        #        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        #            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        #        endif ()
        #    endif ()
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Qt${QT_VERSION}::QWindowsIntegrationPlugin>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
                )

        foreach (QT_LIB ${REQUIRED_LIBS})
                #        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                #                COMMAND ${CMAKE_COMMAND} -E copy
                #                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}${DEBUG_SUFFIX}.dll"
                #                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
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


target_link_libraries(${PROJECT_NAME} src libViewer ${REQUIRED_LIBS_QUALIFIED} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


cmake_minimum_required(VERSION 3.8)

project(localTransform)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS Widgets Gui)

find_package(glm REQUIRED NO_DEFAULT_PATH)

set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Gui Qt${QT_VERSION}::Widgets)
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE forms *.ui)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${forms} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR})

if(WIN32)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#    message(STATUS ${QT_INSTALL_PATH})
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
    set(WIN32_REQUIRED_LIBS glm)

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
    set(UNIX_REQUIRED_LIBS glm)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${SRC_TARGET} ${REQUIRED_LIBS_QUALIFIED}
                                             ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

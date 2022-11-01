cmake_minimum_required(VERSION 3.8)

project(simplePlayer)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR})

if(WIN32)
    set(Qt_LIBS Core Gui Widgets)
    find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${Qt_LIBS})
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    foreach(QT_LIB ${Qt_LIBS})
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt${QT_VERSION}::${QT_LIB}>
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endforeach(QT_LIB)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} ${SRC_TARGET} libApp libLoader ${UNIX_REQUIRED_LIBS}
                      ${WIN32_REQUIRED_LIBS})

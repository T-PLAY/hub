cmake_minimum_required(VERSION 3.8)

project(app-server)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources main.cpp)

add_executable(${PROJECT_NAME} ${sources} # ${headers}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${SRC_DIR})

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} hub ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS}
                      server)

add_subdirectory(libServer)

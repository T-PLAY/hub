cmake_minimum_required(VERSION 3.8)

project(app-server)

set(REQUIRED_LIBS)
if(HUB_ENABLE_COVERAGE)
    set(REQUIRED_LIBS ${REQUIRED_LIBS} "gcov")
endif()

file(GLOB_RECURSE sources main.cpp)

add_executable(${PROJECT_NAME} ${sources} # ${headers}
)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} hub ${REQUIRED_LIBS} server)

add_subdirectory(libServer)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    DESTINATION bin)

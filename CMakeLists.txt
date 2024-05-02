cmake_minimum_required(VERSION 3.8)

project(testHubAsSubmodule)

add_subdirectory(hub)

#find_package(hub REQUIRED)
if (hub_FOUND)

    file(GLOB sources main.cpp)

    add_executable(${PROJECT_NAME} ${sources})
    target_link_libraries(${PROJECT_NAME} hub)

    if(WIN32)
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()

endif()

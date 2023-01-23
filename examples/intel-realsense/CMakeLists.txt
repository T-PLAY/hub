cmake_minimum_required(VERSION 3.8)

project(example-realsense)


find_package(realsense2 QUIET)

if (realsense2_FOUND)

        set(WIN32_REQUIRED_LIBS)
        set(UNIX_REQUIRED_LIBS)

        file(GLOB_RECURSE sources *.cpp)
        file(GLOB_RECURSE headers *.h)

        add_executable(${PROJECT_NAME} ${sources} ${headers})

        #target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${SRC_DIR})
        target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

        if(WIN32)
            add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub>
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>)

        elseif(UNIX)

        endif()

        target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} hub
                              ${realsense2_LIBRARY})

        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:realsense2::realsense2>
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>)

endif()

cmake_minimum_required(VERSION 3.8)

project(example-opencv)

# depends : libfmt

find_package(OpenCV QUIET)

if(OpenCV_FOUND)

    set(WIN32_REQUIRED_LIBS)
    set(UNIX_REQUIRED_LIBS)

    file(GLOB_RECURSE sources *.cpp)
    file(GLOB_RECURSE headers *.h)

    add_executable(${PROJECT_NAME} ${sources} ${headers})

    target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR} ${OpenCV_INCLUDE_DIRS})

    if(WIN32)
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

        message(STATUS ${OpenCV_LIBS})
        message(STATUS ${OpenCV_SHARED})

        foreach(OpenCV_LIB ${OpenCV_LIBS})
            add_custom_command(
                TARGET ${PROJECT_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${OpenCV_LIB}>
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
        endforeach(OpenCV_LIB)

        # add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        # $<TARGET_FILE:${OpenCV_LIBS}> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    elseif(UNIX)

    endif()

    target_link_libraries(${PROJECT_NAME} hub ${OpenCV_LIBS} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

endif()

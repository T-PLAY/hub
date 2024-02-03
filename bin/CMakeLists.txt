cmake_minimum_required(VERSION 3.5)

project(bin LANGUAGES CXX)

if(TARGET hub)

    set(bin_srcs)
    foreach(HUB_MODULE ${HUB_MODULES})
        file(GLOB_RECURSE bin_src ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp)
        list(APPEND bin_srcs ${bin_src})
    endforeach()

    string(REPLACE ".cpp" "" SOURCES_BASENAME "${bin_srcs}")
    set(SOURCES_BASENAME ${SOURCES_BASENAME})

    foreach(name ${SOURCES_BASENAME})
        get_filename_component(basename ${name} NAME)
        set(TARGET_NAME hub-${basename})
        add_executable(${TARGET_NAME} ${name}.cpp)
        target_link_libraries(${TARGET_NAME} hub)

        if(WIN32)
            if(HUB_BUILD_SHARED AND NOT HUB_BUILD_STATIC)
                add_custom_command(
                    TARGET ${TARGET_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
            endif()
        endif()
    endforeach()

endif()

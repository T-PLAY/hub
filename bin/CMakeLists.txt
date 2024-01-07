cmake_minimum_required(VERSION 3.5)

# set(CMAKE_CXX_STANDARD 20) # filesystem
# set(CMAKE_CXX_STANDARD 17) # filesystem
project(bin LANGUAGES CXX)
# if (XCODE)
#         set(LINK_LIBRARIES hub_static)
# else()
#     if (TARGET hub_static)
#         set(LINK_LIBRARIES hub_static)
#     else()
#         set(LINK_LIBRARIES hub_shared)
#     endif()
# endif()

if (NOT TARGET hub)
    message(FATAL_ERROR "target hub not found")
endif()

# if (HUB_BUILD_SERVER)

    set(bin_srcs)
    foreach(HUB_MODULE ${HUB_MODULES})
        file(
            GLOB_RECURSE
            bin_src
            ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp
        )
        list(APPEND bin_srcs ${bin_src})
    endforeach()

    string(REPLACE ".cpp" "" SOURCES_BASENAME "${bin_srcs}")
    set(SOURCES_BASENAME ${SOURCES_BASENAME})

    foreach(name ${SOURCES_BASENAME})
        get_filename_component(basename ${name} NAME)
        set(TARGET_NAME hub-${basename})
        add_executable(${TARGET_NAME} ${name}.cpp)
        # target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})
        target_link_libraries(${TARGET_NAME} hub)

        if(WIN32)
            if (HUB_BUILD_SHARED)
                add_custom_command(
                    TARGET ${TARGET_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
            endif()
        endif()
    endforeach()

# endif()

# if (HUB_BUILD_SENSOR)
# file(
#     # GLOB_RECURSE
#     GLOB
#     streamers
#     ${CMAKE_CURRENT_SOURCE_DIR}/streamer/*
# )
# message(STATUS "streamers: ${streamers}")
# # list(APPEND streamers ${bin_src})
# foreach(streamer ${streamers})
#     get_filename_component(basename ${streamer} NAME)
#     message(STATUS "streamer: ${basename}")
#     file(
#         GLOB_RECURSE
#         sources
#         ${CMAKE_CURRENT_SOURCE_DIR}/streamer/${basename}/*.cpp
#         ${CMAKE_CURRENT_SOURCE_DIR}/streamer/${basename}/*.c
#         ${CMAKE_CURRENT_SOURCE_DIR}/streamer/${basename}/*.h
#     )
#     add_executable(bin-streamer-${basename} ${sources})
#     # target_link_libraries(bin-streamer-${basename} ${LINK_LIBRARIES})
#     target_link_libraries(bin-streamer-${basename} hub)
# endforeach()
# endif()

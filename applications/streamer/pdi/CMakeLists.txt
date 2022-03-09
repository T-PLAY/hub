cmake_minimum_required (VERSION 3.8)

project(pdiStreamer)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


#find_package(realsense2 REQUIRED)
find_package(pdi REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)


add_executable(${PROJECT_NAME}
        ${sources}
        ${headers}
        )


target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${SRC_DIR}
        )

if (WIN32)
        #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)


elseif(UNIX)
        #    set(THREADS_PREFER_PTHREAD_FLAG ON)
#        find_package(Threads REQUIRED)
#        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} src pdi ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

if (WIN32)
    foreach(dll ${PDI_DLL})
#            message("${dll}")
            add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                      COMMAND ${CMAKE_COMMAND} -E copy_if_different
                      ${dll}
                      ${CMAKE_CURRENT_BINARY_DIR}/
                      )
    endforeach()
endif()

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:realsense2::realsense2> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#		)

cmake_minimum_required (VERSION 3.8)

#project ("HubV2")
project(simpleStreamer)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME}
    ${sources}
    ${headers}
    )


target_include_directories(${PROJECT_NAME} PUBLIC
    ${SRC_DIR}
    ${PROJECT_SOURCE_DIR}
    )

if (WIN32)
    #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)


elseif(UNIX)
    #    set(THREADS_PREFER_PTHREAD_FLAG ON)
    #        find_package(Threads REQUIRED)
    #        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} src ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


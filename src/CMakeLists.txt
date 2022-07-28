cmake_minimum_required (VERSION 3.8)

#set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#project ("HubV2")
project(src)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.h)

#add_library(${PROJECT_NAME} STATIC
add_library(${PROJECT_NAME} SHARED
    #add_library(${PROJECT_NAME} SHARED
    ${sources}
    ${headers}
    )


target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
    )

if (WIN32)
    #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)


elseif(UNIX)
    #    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${headers} DESTINATION include)


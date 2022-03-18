cmake_minimum_required (VERSION 3.8)

project(simpleViewer)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")


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


elseif(UNIX)
    #        find_package(Threads REQUIRED)
    #        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} src ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


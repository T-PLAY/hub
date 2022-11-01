cmake_minimum_required(VERSION 3.8)

project(libApp)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_library(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC # ${SRC_DIR}
                                                  ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)
    # #    set(THREADS_PREFER_PTHREAD_FLAG ON) find_package(Threads REQUIRED) #
endif()

target_link_libraries(
    ${PROJECT_NAME} PUBLIC ${SRC_TARGET} # # ${REQUIRED_LIBS_QUALIFIED} # ${UNIX_REQUIRED_LIBS} #
)

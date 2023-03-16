cmake_minimum_required(VERSION 3.8)

project(server)

set(REQUIRED_LIBS)
if(HUB_ENABLE_COVERAGE)
    set(REQUIRED_LIBS ${REQUIRED_LIBS} "gcov")
endif()

file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.hpp *.h)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS} hub)

cmake_minimum_required(VERSION 3.8)

project(nativeViewer)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} ${SRC_TARGET} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

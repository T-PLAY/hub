cmake_minimum_required(VERSION 3.8)

project(webcam)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(OpenCV REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR}
                                                  ${OpenCV_INCLUDE_DIRS})

if(WIN32)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} ${SRC_TARGET} ${OpenCV_LIBS} ${UNIX_REQUIRED_LIBS}
                      ${WIN32_REQUIRED_LIBS})

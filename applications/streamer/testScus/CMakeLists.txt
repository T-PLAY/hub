cmake_minimum_required(VERSION 3.8)

project(app-streamer-testScus)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

find_package(Scus REQUIRED)
find_package(TBB REQUIRED)
find_package(DCMTK REQUIRED)
find_package(OpenMP REQUIRED)

message(STATUS ${SELF_DIR})

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(
    ${PROJECT_NAME} PUBLIC # ${PROJECT_SOURCE_DIR} ${SRC_DIR}
                           ${SCUS_INCLUDE_DIR} DCMTK::dcmdata DCMTK::dcmimage)

if(WIN32)

elseif(UNIX)

endif()

target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} Scus)

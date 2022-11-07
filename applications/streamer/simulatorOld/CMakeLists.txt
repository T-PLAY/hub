cmake_minimum_required(VERSION 3.8)

project(simulatorOld)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(TBB REQUIRED)
find_package(Scus REQUIRED)
find_package(DCMTK REQUIRED)
find_package(OpenMP REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR})

if(WIN32)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:TBB::tbb> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

elseif(UNIX)

endif()

target_link_libraries(
    ${PROJECT_NAME}
    ${SRC_TARGET}

    Scus
    ${UNIX_REQUIRED_LIBS}
    ${WIN32_REQUIRED_LIBS}
    DCMTK::dcmdata
    DCMTK::dcmimage
    DicomUtils)

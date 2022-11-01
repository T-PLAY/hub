cmake_minimum_required(VERSION 3.8)

project(dicomImporter)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(DCMTK REQUIRED)

find_package(glm REQUIRED)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME} ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR} ${PROJECT_SOURCE_DIR})

if(WIN32)

    set(WIN32_REQUIRED_LIBS glm)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

elseif(UNIX)
    set(UNIX_REQUIRED_LIBS glm)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    ${SRC_TARGET}
    libApp
    ${UNIX_REQUIRED_LIBS}
    ${WIN32_REQUIRED_LIBS}
    DCMTK::dcmdata
    DCMTK::dcmimage
    DicomUtils)

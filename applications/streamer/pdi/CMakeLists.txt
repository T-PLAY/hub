cmake_minimum_required(VERSION 3.8)

project(app-streamer-pdi)

set(UNIX_INCLUDE_DIRS)
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(pdi REQUIRED)

if(NOT WIN32)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    add_definitions(-DPACKAGE_LOCAL_STATE_DIR_PITERM=\"${PDI_SDK}/src\")
    add_definitions(-DPACKAGE_DATA_ROOT_DIR_PITERM=\"${PDI_SDK}/src\")
    set(UNIX_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS})
endif()

if(WIN32)
    set(sources main.cpp)
else()
    set(sources main2.cpp main2.h config.h)
endif()

add_executable(${PROJECT_NAME} ${sources} # ${headers}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${SRC_DIR}
                                                  ${UNIX_INCLUDE_DIRS})

if(WIN32)

elseif(UNIX)
    set(UNIX_REQUIRED_LIBS usb-1.0 ${GTK3_LIBRARIES})
endif()

target_link_libraries(${PROJECT_NAME} pdi ${SRC_TARGET} ${UNIX_REQUIRED_LIBS}
                      ${WIN32_REQUIRED_LIBS})

if(WIN32)

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}>
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    foreach(dll ${PDI_DLL})
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${dll} ${CMAKE_CURRENT_BINARY_DIR}/)
    endforeach()

endif()

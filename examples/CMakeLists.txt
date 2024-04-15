cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

if(TARGET hub)

    set(example_srcs)
    foreach(HUB_MODULE ${HUB_MODULES})
        file(GLOB example_src ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp)
        list(APPEND example_srcs ${example_src})
    endforeach()

    list(APPEND example_srcs ${CMAKE_CURRENT_SOURCE_DIR}/example-wip.cpp)
    list(APPEND example_srcs ${CMAKE_CURRENT_SOURCE_DIR}/example-readme.cpp)

    string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${example_srcs}")
    set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

    find_package(PahoMqttCpp QUIET)
    if(HUB_BUILD_MQTT AND PahoMqttCpp_FOUND)
        file(GLOB example_srcs mqtt/*.cpp)
        string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME2 "${example_srcs}")
        set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME} ${EXAMPLE_SRC_BASENAME2})
        set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
    endif()

    foreach(name ${EXAMPLE_SRC_BASENAME})
        get_filename_component(basename ${name} NAME)
        set(TARGET_NAME hub-${basename})
        add_executable(${TARGET_NAME} ${name}.cpp)
        target_link_libraries(${TARGET_NAME} hub ${LINK_LIBRARIES})

        if(WIN32)
            if(HUB_BUILD_SHARED AND NOT HUB_BUILD_STATIC)
                add_custom_command(
                    TARGET ${TARGET_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
            endif()
        endif()
    endforeach()

endif()

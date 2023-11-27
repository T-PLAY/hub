cmake_minimum_required(VERSION 3.5)

project(bin LANGUAGES CXX)

#if (true)

#if (TARGET hub_static OR TARGET hub_shared AND TARGET hub_static)
#if (TARGET ${PROJECT_NAME}_static)
#if (TARGET hub_static)
# if (BUILD_IMPL AND BUILD_SENSOR)
if (HUB_BUILD_IMPL_SERVER2)
   # message(FATAL_ERROR "ouou")

    #set(LINK_LIBRARIES hub PahoMqttCpp::paho-mqttpp3)
    if (XCODE)
            set(LINK_LIBRARIES hub_static)
            #set(LINK_LIBRARIES hub_shared)
    else()
    #        set(LINK_LIBRARIES hub)
        if (TARGET hub_static)
            set(LINK_LIBRARIES hub_static)
        else()
            set(LINK_LIBRARIES hub_shared)
        endif()
    endif()
    #set(LINK_LIBRARIES hub eclipse-paho-mqtt-c::paho-mqtt3c eclipse-paho-mqtt-c::paho-mqtt3a)
    #set(LINK_LIBRARIES hub PahoMqttCpp)

    set(bin_srcs)
    foreach(HUB_SOURCE_DIR ${HUB_SOURCE_DIRS})
        file(
#            GLOB
            GLOB_RECURSE
            bin_src
            ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.cpp
        )
        list(APPEND bin_srcs ${bin_src})
    endforeach()

    # if (NOT HUB_BUILD_SERVER)
        # list(FILTER bin_srcs EXCLUDE REGEX ".*server\\.cpp$")
    # endif()
#    message(STATUS "bin_srcs: ${bin_srcs}")

    string(REPLACE ".cpp" "" SOURCES_BASENAME "${bin_srcs}")
    set(SOURCES_BASENAME ${SOURCES_BASENAME})
#    message(STATUS "${SOURCES_BASENAME}")


    foreach(name ${SOURCES_BASENAME})
        get_filename_component(basename ${name} NAME)
        set(TARGET_NAME ${basename})
        add_executable(${TARGET_NAME} ${name}.cpp)
    #    target_link_libraries(${TARGET_NAME} hub paho-mqttpp3 paho-mqtt3as)
    #    target_link_libraries(${TARGET_NAME} paho-mqttpp3 paho-mqtt3as)
        target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})
    #    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    #    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
    #    target_include_directories(
    #        ${TARGET_NAME}
    #        PRIVATE ${HUB_EXTERNAL_DIR})

    #    add_custom_target(
    #        run-${TARGET_NAME} # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    #        COMMAND ${TARGET_NAME}
    #        COMMENT "Running executable")
    #    custom_target_added(run-${TARGET_NAME})
    #target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_EXPORTS)
	#target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_STATIC) # -> SRC_API = void, future shared

        if(WIN32)
        if (HUB_BUILD_SHARED)
            add_custom_command(
                # TARGET ${PROJECT_NAME}
                TARGET ${TARGET_NAME}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
            endif()
        endif()
    endforeach()

endif()

#endif()

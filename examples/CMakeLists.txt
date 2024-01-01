cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

if (TARGET hub_static OR TARGET hub_shared AND TARGET hub_static)
#set(LINK_LIBRARIES hub PahoMqttCpp::paho-mqttpp3)
if (TARGET hub_static)
    set(LINK_LIBRARIES hub_static)
endif()
#set(LINK_LIBRARIES hub)
#set(LINK_LIBRARIES hub eclipse-paho-mqtt-c::paho-mqtt3c eclipse-paho-mqtt-c::paho-mqtt3a)

#set(LINK_LIBRARIES hub PahoMqttCpp)

#file(GLOB EXAMPLE_SRC *.cpp)
set(example_srcs)
foreach(HUB_MODULE ${HUB_MODULES})
    file(
        GLOB
    #    GLOB_RECURSE
        example_src
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp
    )
    list(APPEND example_srcs ${example_src})
endforeach()

list(APPEND example_srcs ${CMAKE_CURRENT_SOURCE_DIR}/example-wip.cpp)

string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${example_srcs}")
set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})


find_package(PahoMqttCpp QUIET)
#find_package(PahoMqttCpp QUIET NO_DEFAULT_PATH)
if (HUB_BUILD_MQTT AND PahoMqttCpp_FOUND)
    file(GLOB example_srcs mqtt/*.cpp)
    string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME2 "${example_srcs}")
    set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME} ${EXAMPLE_SRC_BASENAME2})
#    set(LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3")
    set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
endif()

#find_package(eclxpse-paho-mqtt-c QUIET NO_DEFAULT_PATH)
#find_package(OpenSSL REQUIRED)
#find_package(eclipse-paho-mqtt-c REQUIRED)
#find_package(PahoMqttCpp REQUIRED)
#if (NOT ${PahoMqttCpp_FOUND})
#    include(FetchContent)
##    FetchContent_Declare(
##        PahoMqttC
##        GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.c.git
##        GIT_TAG v1.3.12 # or a later release
##        GIT_PROGRESS TRUE)
##    FetchContent_MakeAvailable(PahoMqttC)

#    FetchContent_Declare(
#        PahoMqttCpp
#        GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.cpp.git
#        GIT_TAG v1.2.0 # or a later release
#        GIT_PROGRESS TRUE)
#    FetchContent_MakeAvailable(PahoMqttCpp)
##    set(LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3")
#endif()
#find_package(PahoMqttCpp REQUIRED NO_DEFAULT_PATH)

#FetchContent_GetProperties(PahoMqttCpp)
#if(NOT PahoMqttCpp_POPULATED)
##  FetchContent_Populate(PahoMqttCpp)
#  message(STATUS "PahoMqttCpp source dir: ${PahoMqttCpp_SOURCE_DIR}")
#  message(STATUS "PahoMqttCpp binary dir: ${PahoMqttCpp_BINARY_DIR}")
##  add_subdirectory(${PahoMqttCpp_SOURCE_DIR} ${PahoMqttCpp_BINARY_DIR})
#endif()


#find_package(PkgConfig REQUIRED)
#pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)
#find_package(Mosquitto REQUIRED)
#find_package(PahoMqttC REQUIRED)
#find_package(mqttCpp REQUIRED)
#find_package(mqtt REQUIRED)

foreach(name ${EXAMPLE_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME hub-${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
#    target_link_libraries(${TARGET_NAME} hub paho-mqttpp3 paho-mqtt3as)
#    target_link_libraries(${TARGET_NAME} paho-mqttpp3 paho-mqtt3as)
    target_link_libraries(${TARGET_NAME} ${LINK_LIBRARIES})
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
#    target_include_directories(
#        ${TARGET_NAME}
#        PRIVATE ${HUB_EXTERNAL_DIR})

    if(WIN32)
        add_custom_command(
            # TARGET ${PROJECT_NAME}
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
    endif()
endforeach()

endif()

cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

#set(LINK_LIBRARIES hub PahoMqttCpp::paho-mqttpp3)
set(LINK_LIBRARIES hub)
#set(LINK_LIBRARIES hub eclipse-paho-mqtt-c::paho-mqtt3c eclipse-paho-mqtt-c::paho-mqtt3a)

#set(LINK_LIBRARIES hub PahoMqttCpp)

file(GLOB SOURCES *.cpp)

if (NOT HUB_BUILD_SERVER)
    list(FILTER SOURCES EXCLUDE REGEX ".*server\\.cpp$")
endif()

string(REPLACE ".cpp" "" SOURCES_BASENAME "${SOURCES}")
set(SOURCES_BASENAME ${SOURCES_BASENAME})



#find_package(PahoMqttCpp QUIET)
##find_package(PahoMqttCpp QUIET NO_DEFAULT_PATH)
#if (PahoMqttCpp_FOUND)
#    file(GLOB SOURCES mqtt/*.cpp)
#    string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME2 "${SOURCES}")
#    set(SOURCES_BASENAME ${SOURCES_BASENAME} ${EXAMPLE_SRC_BASENAME2})
##    set(LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3")
#    set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
#endif()

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

    if(WIN32)
        add_custom_command(
            # TARGET ${PROJECT_NAME}
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:${TARGET_NAME}>)
    endif()
endforeach()

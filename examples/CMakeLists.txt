cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

file(GLOB EXAMPLE_SRC *.cpp)

string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}")
set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

find_package(PahoMqttCpp REQUIRED)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(Mosquitto IMPORTED_TARGET libmosquitto REQUIRED)
#find_package(Mosquitto REQUIRED)
#find_package(PahoMqttC REQUIRED)
#find_package(mqttCpp REQUIRED)
#find_package(mqtt REQUIRED)

foreach(name ${EXAMPLE_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME ${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
#    target_link_libraries(${TARGET_NAME} hub paho-mqttpp3 paho-mqtt3as)
#    target_link_libraries(${TARGET_NAME} paho-mqttpp3 paho-mqtt3as)
    target_link_libraries(${TARGET_NAME} hub PahoMqttCpp::paho-mqttpp3)
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

    if(WIN32)
        add_custom_command(
            # TARGET ${PROJECT_NAME}
            TARGET ${TARGET_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${TARGET_NAME}>)
    endif()
endforeach()

cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
#set(LINK_LIBRARIES Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)

find_package(Catch2 QUIET)
if(NOT ${Catch2_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        # GIT_TAG v3.0.1 # or a later release
        GIT_TAG v3.3.2 # or a later release
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(Catch2)
endif()

#find_package(PahoMqttCpp QUIET)
#if (NOT ${PahoMqttCpp_FOUND})
#    include(FetchContent)
#    FetchContent_Declare(
#        PahoMqttCpp
#        GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.cpp.git
#        GIT_TAG v1.2.0 # or a later release
#        GIT_PROGRESS TRUE)
#    FetchContent_MakeAvailable(PahoMqttCpp)
#    set(LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3")
#endif()
#find_package(PahoMqttCpp REQUIRED)

file(GLOB_RECURSE test_srcs *.cpp)
#file(GLOB test_srcs *.cpp)

find_package(PahoMqttCpp QUIET)
#find_package(PahoMqttCpp QUIET NO_DEFAULT_PATH)
if (PahoMqttCpp_FOUND)

#    file(GLOB EXAMPLE_SRC mqtt/*.cpp)
#    string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME2 "${EXAMPLE_SRC}")
#    set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME} ${EXAMPLE_SRC_BASENAME2})
#    set(LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3")
    set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
else()
    list(FILTER test_srcs EXCLUDE REGEX ".*Mqtt\\.cpp$")

    file(GLOB mqtt_srcs mqtt/*.cpp)
    list(REMOVE_ITEM test_srcs ${mqtt_srcs})
#    message(STATUS "#################### ${mqtt_srcs}")
endif()
#message(STATUS "#################### ${test_srcs}")

if (NOT HUB_BUILD_SERVER)
    list(FILTER test_srcs EXCLUDE REGEX ".*Server\\.cpp$")
    list(FILTER test_srcs EXCLUDE REGEX ".*-server-.*\\.cpp$")

#    file(GLOB server_srcs server/*.cpp)
#    list(REMOVE_ITEM test_srcs ${server_srcs})
endif()


add_executable(test-all ${test_srcs})
# target_link_libraries(test-all Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
target_link_libraries(test-all ${LINK_LIBRARIES})
target_include_directories(test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)

if(WIN32)
    add_custom_command(
        TARGET test-all
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:test-all>)
endif()

include(CTest)
enable_testing()
foreach(testFile ${test_srcs})

    get_filename_component(test ${testFile} NAME_WE)

    add_executable(${test} ${testFile})
    # target_link_libraries(${test} PRIVATE Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
	target_link_libraries(${test} PRIVATE ${LINK_LIBRARIES})
    target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    add_test(NAME ${test} COMMAND ${test})

endforeach()

add_custom_target(
    tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V # COMMENT "Generating API documentation with Doxygen"
    COMMENT "Running tests")
custom_target_added(tests)

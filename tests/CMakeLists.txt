cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
# Threads::Threads ) set( LINK_LIBRARIES Catch2::Catch2WithMain hub PahoMqttCpp::paho - mqttpp3 )

find_package(Catch2 QUIET)
if(NOT ${Catch2_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        # GIT_TAG v3 .0.1 # or a later release
        GIT_TAG v3.3.2 # or a later release
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(Catch2)
endif()

set(test_srcs)
set(test_headers)
foreach(HUB_MODULE ${HUB_MODULES})
    file(GLOB_RECURSE test_src ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp)
    list(APPEND test_srcs ${test_src})

    file(GLOB_RECURSE test_header ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.hpp)
    list(APPEND test_headers ${test_header})
endforeach()

list(APPEND test_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp)

add_library(test-common STATIC ${test_headers} ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
target_link_libraries(test-common PUBLIC ${LINK_LIBRARIES})

if(BUILD_CORE)
    if(NOT Eigen3_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatEigen3.cpp)
    endif()
    if(NOT OpenCV_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOpenCV.cpp)
    endif()
    if(NOT OIIO_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOIIO.cpp)
    endif()
endif()

find_package(PahoMqttCpp QUIET)
set(LINK_LIBRARIES)
if(PahoMqttCpp_FOUND)

    set(LINK_LIBRARIES ${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3)
else()
    list(FILTER test_srcs EXCLUDE REGEX ".*Mqtt\\.cpp$")
endif()

if(NOT HUB_BUILD_SERVER)
    list(FILTER test_srcs EXCLUDE REGEX ".*inputOutputStream/.*\\.cpp$")
    list(FILTER test_srcs EXCLUDE REGEX ".*test-io-InputOutputStream.cpp$")

endif()

if(NOT HUB_BUILD_SENSOR)
    list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/client/test-client-ViewerSensor.cpp)

endif()

list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)
list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)

list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)
list(PREPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)

add_executable(hub-test-all ${test_srcs} ${test_headers})
target_link_libraries(hub-test-all PRIVATE ${LINK_LIBRARIES} test-common)
target_include_directories(hub-test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# CXX_EXTENSIONS OFF)
if(PahoMqttCpp_FOUND)
    target_compile_definitions(hub-test-all PUBLIC -DHUB_TESTS_MQTT_FOUND)
endif()

if(WIN32)
    if(HUB_BUILD_SHARED AND NOT HUB_BUILD_STATIC)
        add_custom_command(
            TARGET hub-test-all
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:hub-test-all>)
    endif()
endif()

include(CTest)

if(HUB_SERVICE_PORT)
    set(HUB_SERVER_TEST_BIN_PORT ${HUB_SERVICE_PORT})
else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(HUB_SERVER_TEST_BIN_PORT 6000) # 5000 used by macos
    else()
        set(HUB_SERVER_TEST_BIN_PORT 8000) # 7000 used by macos
    endif()
endif()

enable_testing()
foreach(testFile ${test_srcs})

    get_filename_component(test ${testFile} NAME_WE)

    add_executable(hub-${test} ${testFile})
    target_link_libraries(hub-${test} PRIVATE ${LINK_LIBRARIES} test-common)
    target_include_directories(hub-${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    if(PahoMqttCpp_FOUND)
        target_compile_definitions(hub-${test} PUBLIC -DHUB_TESTS_MQTT_FOUND)
    endif()

    add_test(NAME hub-${test} COMMAND hub-${test})

    add_custom_target(
        hub-tests-bin-${test}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port
                ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port
                ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port
                ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
        USES_TERMINAL
        COMMENT "Running server")

endforeach()

if(HUB_BUILD_NATIVE)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port
                ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port
                ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port
                ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running server")

elseif(HUB_BUILD_CLIENT)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port
                ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port
                ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running server")

elseif(HUB_BUILD_SERVER)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port
                ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        USES_TERMINAL
        COMMENT "Running server")
endif()

if(TARGET hub-tests-bin)
    custom_target_added(tests-bin)
endif()

add_custom_target(
    hub-tests
    COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
    USES_TERMINAL
    COMMENT "Running tests")
custom_target_added(tests)


# if (UNIX)
add_custom_command(
    TARGET test-common
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${HUB_PROJECT_DIR}/scripts/showDurationTest.sh" ${CMAKE_CURRENT_BINARY_DIR})
# endif()

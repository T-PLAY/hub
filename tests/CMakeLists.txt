cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
#if (XCODE)
#    #set(LINK_LIBRARIES Catch2::Catch2WithMain hub_static)
#    set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
#else()
##set( LINK_LIBRARIES Catch2::Catch2WithMain hub )
#    set(LINK_LIBRARIES Catch2::Catch2WithMain hub_static)
#endif()
#set( LINK_LIBRARIES ${ LINK_LIBRARIES } Threads::Threads )
#set( LINK_LIBRARIES Catch2::Catch2WithMain hub PahoMqttCpp::paho - mqttpp3 )

find_package(Catch2 QUIET)
if(NOT ${Catch2_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
#GIT_TAG v3 .0.1 # or a later release
        GIT_TAG v3.3.2 # or a later release
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(Catch2)
endif()

#find_package( TBB REQUIRED )
#set( LINK_LIBRARIES ${ LINK_LIBRARIES } tbb )

#find_package( PahoMqttCpp QUIET )
#if ( NOT ${ PahoMqttCpp_FOUND } )
#    include( FetchContent )
#    FetchContent_Declare(
#    PahoMqttCpp
#    GIT_REPOSITORY https: // github.com/eclipse/paho.mqtt.cpp.git
#    GIT_TAG v1 .2.0 # or a later release
#    GIT_PROGRESS TRUE )
#    FetchContent_MakeAvailable( PahoMqttCpp )
#    set( LINK_LIBRARIES "${LINK_LIBRARIES} PahoMqttCpp::paho-mqttpp3" )
#endif()
#find_package( PahoMqttCpp REQUIRED )

#file( GLOB test_srcs*.cpp )
set(test_srcs)
set(test_headers)
foreach(HUB_MODULE ${HUB_MODULES})
#set( DIRS "core" #"net" #"io" )
#foreach ( HUB_MODULE ${ DIRS } )
    file(
        GLOB_RECURSE
        test_src
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp
    )
    list(APPEND test_srcs ${test_src})

    file(
        #    GLOB
        GLOB_RECURSE
        test_header
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.hpp
    )
    list(APPEND test_headers ${test_header})
endforeach()

# list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
list(APPEND test_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp)

# add_library(test-common OBJECT ${test_headers} ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
# add_library(test-common SHARED ${test_headers} ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
add_library(test-common STATIC ${test_headers} ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
#target_link_libraries(test-common PRIVATE ${LINK_LIBRARIES})
target_link_libraries(test-common PUBLIC ${LINK_LIBRARIES})
# target_include_directories(test-common PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#set(test_headers_src)
if (BUILD_CORE)
    #    list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/io/test_core_io_common.cpp)
    #    list(APPEND test_headers_src ${CMAKE_CURRENT_SOURCE_DIR}/core/io/test_core_io_common.cpp)
    if (NOT Eigen3_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatEigen3.cpp)
    endif()
    if (NOT OpenCV_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOpenCV.cpp)
    endif()
    if (NOT OIIO_FOUND)
        list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/core/matrix/test-core-matrix-compatOIIO.cpp)
    endif()
endif()


#list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/server/test-server-end.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-end.cpp)
#list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/server/test-server-end.cpp)



find_package(PahoMqttCpp QUIET)
#find_package(PahoMqttCpp QUIET NO_DEFAULT_PATH)
set(LINK_LIBRARIES)
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
    list(FILTER test_srcs EXCLUDE REGEX ".*inputOutputStream/.*\\.cpp$")
    list(FILTER test_srcs EXCLUDE REGEX ".*test-io-InputOutputStream.cpp$")
    #    list(FILTER test_srcs EXCLUDE REGEX ".*-server-.*\\.cpp$")

    #    file(GLOB server_srcs server/*.cpp)
    #    list(REMOVE_ITEM test_srcs ${server_srcs})
endif()

if (NOT HUB_BUILD_SENSOR)
#    list(FILTER test_srcs EXCLUDE REGEX ".*Server\\.cpp$")
#    list(FILTER test_srcs EXCLUDE REGEX ".*-server-.*\\.cpp$")
    list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/client/test-client-ViewerSensor.cpp)

#    file(GLOB server_srcs server/*.cpp)
#    list(REMOVE_ITEM test_srcs ${server_srcs})
endif()


#if (NOT HUB_BUILD_SERVER)
#    list(FILTER test_srcs EXCLUDE REGEX ".*Server\\.cpp$")
#    list(FILTER test_srcs EXCLUDE REGEX ".*-server-.*\\.cpp$")

#    file(GLOB server_srcs server/*.cpp)
#    list(REMOVE_ITEM test_srcs ${server_srcs})
#endif()

# set(test_srcs
#     ${CMAKE_CURRENT_SOURCE_DIR}/client/test-client-Viewer.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/sensor/test-sensor-InputOutputSensor.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/sensor/test-sensor-InputSensor.cpp
# #     ${CMAKE_CURRENT_SOURCE_DIR}/core/io/test-core-io-Serializer.cpp
# #     ${CMAKE_CURRENT_SOURCE_DIR}/sensor/test-sensor-InputSensor.cpp
# #     ${CMAKE_CURRENT_SOURCE_DIR}/sensor/test-sensor-OutputSensor.cpp
# #     ${CMAKE_CURRENT_SOURCE_DIR}/sensor/test-sensor-OutputSensorT.cpp
# )

# set(test_srcs)
# file(
#     GLOB_RECURSE
#     test_srcs
#     # ${CMAKE_CURRENT_SOURCE_DIR}/sensor/*.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
# )

#file(GLOB test_srcs *.cpp)
list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-end.cpp)
list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)

list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-start.cpp)
list(PREPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)


# gathering tests
#add_executable(test-all ${test_srcs} ${test_headers} ${test_headers_src})
add_executable(hub-test-all ${test_srcs} ${test_headers})
#add_executable(test-all ${test_srcs})
# target_link_libraries(test-all Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
target_link_libraries(hub-test-all PRIVATE ${LINK_LIBRARIES} test-common)
target_include_directories(hub-test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)
if (PahoMqttCpp_FOUND)
    target_compile_definitions(hub-test-all PUBLIC -DHUB_TESTS_MQTT_FOUND)
endif()
# target_compile_definitions(test-all PUBLIC -DHUB_TEST_ALL)

if(WIN32)
#    if (HUB_BUILD_SHARED)
    if (HUB_BUILD_SHARED AND NOT HUB_BUILD_STATIC)
        add_custom_command(
            TARGET hub-test-all
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:hub-test-all>)
    endif()
endif()

#add_definitions(-DHUB_TESTS_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/\")

#if (WIN32)
# if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
#     set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
# else()
#     set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)
# endif()

# foreach *.cpp tests
include(CTest)

if (HUB_SERVICE_PORT)
    set(HUB_SERVER_TEST_BIN_PORT ${HUB_SERVICE_PORT})
else()
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(HUB_SERVER_TEST_BIN_PORT 6000) # 5000 used by macos
    else()
        set(HUB_SERVER_TEST_BIN_PORT 8000) # 7000 used by macos
    endif()
endif()

#target_sources(Continuous
#  PRIVATE
#    core/io/test-core-io-common.hpp
#    core/io/test_core_common.cpp
#)
enable_testing()
foreach(testFile ${test_srcs})
    #  message(STATUS "test src: ${testFile}")
    #endforeach()

    get_filename_component(test ${testFile} NAME_WE)

    #    add_executable(${test} ${testFile} ${test_common})
    #    add_executable(${test} ${testFile} ${test_headers_src})
# list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test_common.cpp)
    add_executable(hub-${test} ${testFile})
    # target_link_libraries(${test} PRIVATE Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
    target_link_libraries(hub-${test} PRIVATE ${LINK_LIBRARIES} test-common)
    target_include_directories(hub-${test}
        #            PUBLIC
        #                ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    if (PahoMqttCpp_FOUND)
        target_compile_definitions(hub-${test} PUBLIC -DHUB_TESTS_MQTT_FOUND)
    endif()
    #target_compile_definitions(${test} PUBLIC -DHUB_TESTS_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/\")

    add_test(NAME hub-${test} COMMAND hub-${test})

    # if (HUB_BUILD_IMPL_SERVER2)
    #     target_compile_definitions(${test} PUBLIC -DHUB_SERVER_PORT=5000)
    # endif()

    #if (HUB_BUILD_SERVER)
    #    add_custom_target(
    #        run-${test}
    ##        COMMAND "${CMAKE_BINARY_DIR}/bin/bin-server &"
    ##        COMMAND ls ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launcher.sh
    ##        COMMAND ls ${CMAKE_BINARY_DIR}/bin/bin-server.exe
    ##        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launcher.sh
    #        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/server/bin-server
    #        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/client/bin-viewer --exitWhenServerLost
    #        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/client/bin-nativeViewer --exitWhenServerLost
    #        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
    #        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
    #        VERBATIM
    #        USES_TERMINAL
    #        COMMENT "Running single test"
    #    )
    #    custom_target_added(run-${test})
    #endif()

    add_custom_target(
        hub-tests-bin-${test}
        # COMMAND bash echo "${HUB_SERVER_TEST_BIN_PORT}" > hubServerPort.txt
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        # COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
        # COMMAND bash ${HUB_PROJECT_DIR}/scripts/deleteServerPort.sh
        # COMMAND bash rm hubServerBinPort.txt
        USES_TERMINAL
        COMMENT "Running server"
    )

endforeach()

#if ("cor" IN_LIST HUB_MODULES)
#    message(FATAL_ERROR "ouou")
#endif()




if (HUB_BUILD_NATIVE)
    add_custom_target(
        hub-tests-bin
        # COMMAND bash echo "${HUB_SERVER_TEST_BIN_PORT}" > hubServerPort.txt
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-nativeViewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        # COMMAND bash ${HUB_PROJECT_DIR}/scripts/deleteServerPort.sh
        # COMMAND bash rm hubServerBinPort.txt
        USES_TERMINAL
        COMMENT "Running server"
    )

elseif (HUB_BUILD_CLIENT)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-viewer --port ${HUB_SERVER_TEST_BIN_PORT} --exitWhenServerLost
        # COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${BIN_DIR}/hub-nativeViewer --exitWhenServerLost
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        # COMMAND bash ${HUB_PROJECT_DIR}/scripts/deleteServerPort.sh
        # VERBATIM
        USES_TERMINAL
        COMMENT "Running server"
        #        OUTPUT tests.log
    )

elseif (HUB_BUILD_SERVER)
    add_custom_target(
        hub-tests-bin
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/generateServerPort.sh ${HUB_SERVER_TEST_BIN_PORT}
        COMMAND bash ${HUB_PROJECT_DIR}/scripts/launcher.sh ${HUB_BIN_DIR}/hub-bin-server --port ${HUB_SERVER_TEST_BIN_PORT} --daemon
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        # COMMAND bash ${HUB_PROJECT_DIR}/scripts/deleteServerPort.sh
        USES_TERMINAL
        COMMENT "Running server"
    )
endif()

if (TARGET hub-tests-bin)
    custom_target_added(tests-bin)
endif()
# endif()

add_custom_target(
    hub-tests
    COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
    USES_TERMINAL
    COMMENT "Running tests")
custom_target_added(tests)

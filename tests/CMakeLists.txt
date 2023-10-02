cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

if (XCODE)
        set(LINK_LIBRARIES Catch2::Catch2WithMain hub_static)
else()
#        set(LINK_LIBRARIES Catch2::Catch2WithMain hub)
        set(LINK_LIBRARIES Catch2::Catch2WithMain hub_static)
endif()
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

#file(GLOB test_srcs *.cpp)
set(test_srcs)
foreach(HUB_SOURCE_DIR ${HUB_SOURCE_DIRS})
    file(
        GLOB
    #    GLOB_RECURSE
        test_src
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.cpp
    )
    list(APPEND test_srcs ${test_src})
endforeach()
#list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp)

#file(GLOB
#    test_common
#    ${CMAKE_CURRENT_SOURCE_DIR}/test_common.hpp
#)

list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/server/test-server-end.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-end.cpp)
list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/server/test-server-end.cpp)

#file(GLOB test_srcs *.cpp)
list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-end.cpp)
list(APPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-end.cpp)

list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)
# list(REMOVE_ITEM test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/./test-start.cpp)
list(PREPEND test_srcs ${CMAKE_CURRENT_SOURCE_DIR}/test-start.cpp)


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

## gathering tests
##add_executable(test-all ${test_srcs} ${test_common})
#add_executable(test-all ${test_srcs})
## target_link_libraries(test-all Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
#target_link_libraries(test-all ${LINK_LIBRARIES})
#target_include_directories(test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
#set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)
#if (PahoMqttCpp_FOUND)
#    target_compile_definitions(test-all PUBLIC -DHUB_TESTS_MQTT_FOUND)
#endif()

#if(WIN32)
#    add_custom_command(
#        TARGET test-all
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub_shared> $<TARGET_FILE_DIR:test-all>)
#endif()

#add_definitions(-DHUB_TESTS_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/\")

#if (WIN32)
if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
    set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
else()
    set(BIN_DIR ${CMAKE_BINARY_DIR}/bin)
endif()

# foreach *.cpp tests
include(CTest)
enable_testing()
foreach(testFile ${test_srcs})
#    message(STATUS "test src: ${testFile}")
#endforeach()

    get_filename_component(test ${testFile} NAME_WE)

#    add_executable(${test} ${testFile} ${test_common})
    add_executable(${test} ${testFile})
    # target_link_libraries(${test} PRIVATE Catch2::Catch2WithMain hub PahoMqttCpp::paho-mqttpp3)
	target_link_libraries(${test} PRIVATE ${LINK_LIBRARIES})
    target_include_directories(${test}
#            PUBLIC
#                ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    if (PahoMqttCpp_FOUND)
        target_compile_definitions(${test} PUBLIC -DHUB_TESTS_MQTT_FOUND)
    endif()
	#target_compile_definitions(${test} PUBLIC -DHUB_TESTS_BIN_DIR=\"${CMAKE_CURRENT_BINARY_DIR}/\")

    add_test(NAME ${test} COMMAND ${test})

#if (HUB_BUILD_SERVER)
#    add_custom_target(
#        run-${test}
##        COMMAND "${CMAKE_BINARY_DIR}/bin/bin-server &"
##        COMMAND ls ${CMAKE_SOURCE_DIR}/scripts/launcher.sh
##        COMMAND ls ${CMAKE_BINARY_DIR}/bin/bin-server.exe
##        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh
#        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/server/bin-server
#        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/client/bin-viewer --exitWhenServerLost
#        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/client/bin-nativeViewer --exitWhenServerLost
#        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R ${test}
#        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V -R test-end
#        VERBATIM
#        USES_TERMINAL
#        COMMENT "Running single test"
#    )
#    custom_target_added(run-${test})
#endif()

endforeach()

if (HUB_BUILD_SERVER)

    add_custom_target(
        tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#        COMMAND ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${CMAKE_BINARY_DIR}/bin/bin-server --maxClient 36
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/bin-server
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/bin-viewer --exitWhenServerLost
        COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/launcher.sh ${BIN_DIR}/bin-nativeViewer --exitWhenServerLost
#        COMMAND echo "######################### starting tests #######################################"
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
#        COMMAND echo "########################## ending tests ########################################"
#        VERBATIM
        USES_TERMINAL
#        COMMAND pkill bin-server
        COMMENT "Running tests"
#        OUTPUT tests.log
    )
else()
    add_custom_target(
        tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ctest -C ${CMAKE_BUILD_TYPE} -V
        COMMENT "Running tests")
endif()
custom_target_added(tests)

cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

# set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")

# set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage -DBUILD_TEST --coverage") set(CMAKE_C_FLAGS "-g -fprofile-arcs
# -ftest-coverage -DBUILD_TEST --coverage")

# find_package(GTest REQUIRED)

find_package(Catch2 QUIET)
if(NOT ${Catch2_FOUND})
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.0.1 # or a later release
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(Catch2)
endif()
# find_package(Catch2 REQUIRED)

file(GLOB_RECURSE TEST_SRC *.cpp)

# string(REPLACE ".cpp" "" TEST_SRC_BASENAME "${TEST_SRC}") set(TEST_SRC_BASENAME ${TEST_SRC_BASENAME})

# foreach(name ${TEST_SRC_BASENAME}) get_filename_component(basename ${name} NAME) set(TARGET_NAME ${basename})
# add_executable(${TARGET_NAME} ${name}.cpp) target_link_libraries(${TARGET_NAME} Catch2::Catch2WithMain hub server
# GTest::gtest GTest::gtest_main) target_link_libraries(${TARGET_NAME} Catch2::Catch2WithMain hub server) #
# set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17) set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD
# ${CMAKE_CXX_STANDARD}) set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF) endforeach()

add_executable(test-all ${TEST_SRC})
# target_link_libraries(test-all Catch2::Catch2WithMain hub server GTest::gtest GTest::gtest_main)
target_link_libraries(test-all Catch2::Catch2WithMain hub server)
set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)

# add_custom_command( TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub>
# $<TARGET_FILE_DIR:${TARGET_NAME}>)

include(CTest)
enable_testing()
# set(TEST_NAMES "")
foreach(
    test
    "acquisition"
#    "any"
#    "asker"
#    "inputSensor"
#    "native"
#    "outputSensor"
#    "resolution"
#    "sensor"
#    "sensorSpec"
#    "streamer"
#    "viewer"
#    "data-dof6"
#    "data-mat4"
#    "data-measure"
#    "data-userData"
#    "io-file"
#    "io-input"
#    "io-inputStream"
#    "io-output"
#    "io-outputStream"
#    "io-ram"
#    "io-memory"
#    "net-clientSocket"
#    "net-serverSocket"
#    "net-socket"
#    "net-utils"
    # "file" "inputSensor" "measures" "ram" "resolutions" "sensorSpec" "server" "serverSpeed"
)
    add_executable(test-${test} test-${test}.cpp)
    target_link_libraries(test-${test} PRIVATE Catch2::Catch2WithMain hub server)
    # target_include_directories(test-${test} PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_include_directories(test-${test} PRIVATE ${HUB_INCLUDE_DIR})
    add_test(NAME test-${test} COMMAND test-${test})
endforeach()

add_custom_target(
    tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ctest -V # COMMENT "Generating API documentation with Doxygen"
    COMMENT "Running tests"
    # VERBATIM
)

# find_package(codecov REQUIRED) add_coverage(test-sensorSpec)

# add_test(NAME test-sensorSpec COMMAND test-sensorSpec) add_test(NAME test-file COMMAND test-file) add_test(NAME
# test-measures COMMAND test-measures) add_test(NAME test-ram COMMAND test-ram) add_test(NAME test COMMAND test-file)
# add_test(test test-all) add_test(NAME test COMMAND $<TARGET_FILE:test-all>)

# ############################################## COVERAGE

# include(code-coverage) add_code_coverage() target_code_coverage(test-sensorSpec EXCLUDE _deps/* catch2
# test-sensorSpec.cpp tests/*) add_code_coverage_all_targets(EXCLUDE tests/*)

# target_code_coverage(test-sensorSpec AUTO ALL EXCLUDE tests/*) # As an executable target, adds to the 'ccov' and
# ccov-all' targets, and the reports will exclude the non-covered.cpp file, and any files in a test/ folder.

cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1 # or a later release
    GIT_PROGRESS TRUE)

FetchContent_MakeAvailable(Catch2)

file(GLOB_RECURSE TEST_SRC *.cpp)

string(REPLACE ".cpp" "" TEST_SRC_BASENAME "${TEST_SRC}")
set(TEST_SRC_BASENAME ${TEST_SRC_BASENAME})

foreach(name ${TEST_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME test-${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
    target_link_libraries(${TARGET_NAME} Catch2WithMain ${SRC_TARGET} libServer)
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

endforeach()

add_executable(test-all ${TEST_SRC})
target_link_libraries(test-all Catch2WithMain ${SRC_TARGET} libServer)
set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)
add_custom_command(
    TARGET ${TARGET_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SRC_TARGET}> $<TARGET_FILE_DIR:${TARGET_NAME}>)


enable_testing()
add_test(NAME test-sensorSpec COMMAND test-sensorSpec)
add_test(NAME test-file COMMAND test-file)
add_test(NAME test-measures COMMAND test-measures)
#add_test(NAME test COMMAND test-file)
#add_test(test test-all)
#add_test(NAME test COMMAND $<TARGET_FILE:test-all>)

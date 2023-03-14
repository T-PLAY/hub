cmake_minimum_required(VERSION 3.5)

project(tests LANGUAGES CXX)

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

file(GLOB_RECURSE TEST_SRC *.cpp)

# GTest::gtest GTest::gtest_main) target_link_libraries(${TARGET_NAME} Catch2::Catch2WithMain hub server) #

add_executable(test-all ${TEST_SRC})
target_link_libraries(test-all Catch2::Catch2WithMain hub server)
set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)

file(GLOB testFiles test-*.cpp)

include(CTest)
enable_testing()
foreach(testFile ${testFiles})
    get_filename_component(test ${testFile} NAME_WE)

    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} PRIVATE Catch2::Catch2WithMain hub server)
    target_include_directories(${test} PRIVATE ${HUB_INCLUDE_DIR})
    add_test(NAME ${test} COMMAND ${test})

    # "*.inl" "/usr/*" NO_DEMANGLE # "examples/*" "include/*" COMMAND ${HUB_BROWSER}

endforeach()

add_custom_target(
    tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ctest -V # COMMENT "Generating API documentation with Doxygen"
    COMMENT "Running tests")

# ############################################## COVERAGE

# target_code_coverage(test-sensorSpec AUTO ALL EXCLUDE tests/*) # As an executable target, adds to the 'ccov' and

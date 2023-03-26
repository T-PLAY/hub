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

file(GLOB_RECURSE test_srcs *.cpp)


add_executable(test-all ${test_srcs})
target_link_libraries(test-all Catch2::Catch2WithMain hub)
target_include_directories(test-all PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET test-all PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
set_property(TARGET test-all PROPERTY CXX_EXTENSIONS OFF)

#file(GLOB testFiles *.cpp)


if(WIN32)
    add_custom_command(
        TARGET test-all
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:test-all>)
endif()


include(CTest)
enable_testing()
foreach(testFile ${test_srcs})

#  if("${testFile}" MATCHES "test-server*|test-client*")
  if("${testFile}" MATCHES "test-server*")
#    message(STATUS ${testFile})
    continue()
  endif()

    get_filename_component(test ${testFile} NAME_WE)

#    add_executable(${test} ${test}.cpp)
    add_executable(${test} ${testFile})
    target_link_libraries(${test} PRIVATE Catch2::Catch2WithMain hub)
#    target_include_directories(${test} PRIVATE ${HUB_INCLUDE_DIR})
    target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    add_test(NAME ${test} COMMAND ${test})

    # "*.inl" "/usr/*" NO_DEMANGLE # "examples/*" "include/*" COMMAND ${HUB_BROWSER}

endforeach()

add_custom_target(
    tests # WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ctest -V # COMMENT "Generating API documentation with Doxygen"
    COMMENT "Running tests")

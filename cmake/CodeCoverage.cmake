# Copyright (c) 2012 - 2017, Lars Bilke All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# CHANGES:
#
# USAGE:
#

include(CMakeParseArguments)

option(CODE_COVERAGE_VERBOSE "Verbose information" FALSE)

# Check prereqs
find_program(GCOV_PATH gcov PATHS ${LCOV_ROOT_DIR})
find_program(
    LCOV_PATH
    NAMES lcov lcov.bat lcov.exe lcov.perl
    PATHS ${LCOV_ROOT_DIR})
find_program(
    FASTCOV_PATH
    NAMES fastcov fastcov.py
    PATHS ${LCOV_ROOT_DIR})
find_program(
    GENHTML_PATH
    NAMES genhtml genhtml.perl genhtml.bat
    PATHS ${LCOV_ROOT_DIR})
find_program(
    GCOVR_PATH gcovr
    PATHS ${CMAKE_SOURCE_DIR}/scripts/test
    PATHS ${LCOV_ROOT_DIR})
find_program(
    CPPFILT_PATH
    NAMES c++filt
    PATHS ${LCOV_ROOT_DIR})

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif() # NOT GCOV_PATH
if(NOT LCOV_PATH)
    message(FATAL_ERROR "lcov not found! Aborting...")
endif() # NOT GCOV_PATH

# Check supported compiler (Clang, GNU and Flang)
get_property(LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
foreach(LANG ${LANGUAGES})
    if("${CMAKE_${LANG}_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        if("${CMAKE_${LANG}_COMPILER_VERSION}" VERSION_LESS 3)
            message(FATAL_ERROR "Clang version must be 3.0.0 or greater! Aborting...")
        endif()
    elseif(NOT "${CMAKE_${LANG}_COMPILER_ID}" MATCHES "GNU" AND NOT "${CMAKE_${LANG}_COMPILER_ID}" MATCHES
                                                                "(LLVM)?[Ff]lang")
        message(FATAL_ERROR "Compiler is not GNU or Flang! Aborting...")
    endif()
endforeach()

set(COVERAGE_COMPILER_FLAGS
    "-g --coverage"
    CACHE INTERNAL "")
if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag(-fprofile-abs-path HAVE_fprofile_abs_path)
    if(HAVE_fprofile_abs_path)
        set(COVERAGE_COMPILER_FLAGS "${COVERAGE_COMPILER_FLAGS} -fprofile-abs-path")
    endif()
endif()

set(CMAKE_Fortran_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the Fortran compiler during coverage builds." FORCE)
set(CMAKE_CXX_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C++ compiler during coverage builds." FORCE)
set(CMAKE_C_FLAGS_COVERAGE
    ${COVERAGE_COMPILER_FLAGS}
    CACHE STRING "Flags used by the C compiler during coverage builds." FORCE)
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds." FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds." FORCE)
mark_as_advanced(CMAKE_Fortran_FLAGS_COVERAGE CMAKE_CXX_FLAGS_COVERAGE CMAKE_C_FLAGS_COVERAGE
                 CMAKE_EXE_LINKER_FLAGS_COVERAGE CMAKE_SHARED_LINKER_FLAGS_COVERAGE)

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR GENERATOR_IS_MULTI_CONFIG))
    message(WARNING "Code coverage results with an optimised (non-Debug) build may be misleading")
endif() # NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR GENERATOR_IS_MULTI_CONFIG)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
    link_libraries(gcov)
endif()

# Defines a target for running and collection code coverage information Builds dependencies, runs the given executable
#
function(setup_target_for_coverage_lcov)

    set(options NO_DEMANGLE SONARQUBE)
    set(oneValueArgs BASE_DIRECTORY NAME)
    set(multiValueArgs EXCLUDE EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES LCOV_ARGS GENHTML_ARGS)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif() # NOT LCOV_PATH

    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif() # NOT GENHTML_PATH

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(DEFINED Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    # Collect excludes (CMake 3.4+: Also compute absolute paths)
    set(LCOV_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE} ${COVERAGE_EXCLUDES} ${COVERAGE_LCOV_EXCLUDES})
        if(CMAKE_VERSION VERSION_GREATER 3.4)
            get_filename_component(EXCLUDE ${EXCLUDE} ABSOLUTE BASE_DIR ${BASEDIR})
        endif()
        list(APPEND LCOV_EXCLUDES "${EXCLUDE}")
    endforeach()
    list(REMOVE_DUPLICATES LCOV_EXCLUDES)

    # Conditional arguments
    if(CPPFILT_PATH AND NOT ${Coverage_NO_DEMANGLE})
        set(GENHTML_EXTRA_ARGS "--demangle-cpp")
    endif()

    # Setting up commands which will be run to generate coverage data. Cleanup lcov
    set(LCOV_CLEAN_CMD
        ${LCOV_PATH}
        ${Coverage_LCOV_ARGS}
        --gcov-tool
        ${GCOV_PATH}
        -directory
        .
        -b
        ${BASEDIR}
        --zerocounters)
    # Create baseline to make sure untouched files show up in the report
    set(LCOV_BASELINE_CMD
        ${LCOV_PATH}
        ${Coverage_LCOV_ARGS}
        --gcov-tool
        ${GCOV_PATH}
        -c
        -i
        -d
        .
        -b
        ${BASEDIR}
        -o
        ${Coverage_NAME}.base)
    # Run tests
    set(LCOV_EXEC_TESTS_CMD ${Coverage_EXECUTABLE} ${Coverage_EXECUTABLE_ARGS})
    # Capturing lcov counters and generating report
    set(LCOV_CAPTURE_CMD
        ${LCOV_PATH}
        ${Coverage_LCOV_ARGS}
        --gcov-tool
        ${GCOV_PATH}
        --directory
        .
        -b
        ${BASEDIR}
        --capture
        --output-file
        ${Coverage_NAME}.capture)
    set(LCOV_BASELINE_COUNT_CMD
        ${LCOV_PATH}
        ${Coverage_LCOV_ARGS}
        --gcov-tool
        ${GCOV_PATH}
        -a
        ${Coverage_NAME}.base
        -a
        ${Coverage_NAME}.capture
        --output-file
        ${Coverage_NAME}.total)
    set(LCOV_FILTER_CMD
        ${LCOV_PATH}
        ${Coverage_LCOV_ARGS}
        --gcov-tool
        ${GCOV_PATH}
        --remove
        ${Coverage_NAME}.total
        ${LCOV_EXCLUDES}
        --output-file
        ${Coverage_NAME}.info)
    # Generate HTML output
    set(LCOV_GEN_HTML_CMD ${GENHTML_PATH} ${GENHTML_EXTRA_ARGS} ${Coverage_GENHTML_ARGS} -o ${Coverage_NAME}
                          ${Coverage_NAME}.info)
    if(${Coverage_SONARQUBE})
        # Generate SonarQube output
        set(GCOVR_XML_CMD
            ${GCOVR_PATH}
            --sonarqube
            ${Coverage_NAME}_sonarqube.xml
            -r
            ${BASEDIR}
            ${GCOVR_ADDITIONAL_ARGS}
            ${GCOVR_EXCLUDE_ARGS}
            --object-directory=${PROJECT_BINARY_DIR})
        set(GCOVR_XML_CMD_COMMAND COMMAND ${GCOVR_XML_CMD})
        set(GCOVR_XML_CMD_BYPRODUCTS ${Coverage_NAME}_sonarqube.xml)
        set(GCOVR_XML_CMD_COMMENT COMMENT
                                  "SonarQube code coverage info report saved in ${Coverage_NAME}_sonarqube.xml.")
    endif()

    if(CODE_COVERAGE_VERBOSE)
        message(STATUS "Executed command report")
        message(STATUS "Command to clean up lcov: ")
        string(REPLACE ";" " " LCOV_CLEAN_CMD_SPACED "${LCOV_CLEAN_CMD}")
        message(STATUS "${LCOV_CLEAN_CMD_SPACED}")

        message(STATUS "Command to create baseline: ")
        string(REPLACE ";" " " LCOV_BASELINE_CMD_SPACED "${LCOV_BASELINE_CMD}")
        message(STATUS "${LCOV_BASELINE_CMD_SPACED}")

        message(STATUS "Command to run the tests: ")
        string(REPLACE ";" " " LCOV_EXEC_TESTS_CMD_SPACED "${LCOV_EXEC_TESTS_CMD}")
        message(STATUS "${LCOV_EXEC_TESTS_CMD_SPACED}")

        message(STATUS "Command to capture counters and generate report: ")
        string(REPLACE ";" " " LCOV_CAPTURE_CMD_SPACED "${LCOV_CAPTURE_CMD}")
        message(STATUS "${LCOV_CAPTURE_CMD_SPACED}")

        message(STATUS "Command to add baseline counters: ")
        string(REPLACE ";" " " LCOV_BASELINE_COUNT_CMD_SPACED "${LCOV_BASELINE_COUNT_CMD}")
        message(STATUS "${LCOV_BASELINE_COUNT_CMD_SPACED}")

        message(STATUS "Command to filter collected data: ")
        string(REPLACE ";" " " LCOV_FILTER_CMD_SPACED "${LCOV_FILTER_CMD}")
        message(STATUS "${LCOV_FILTER_CMD_SPACED}")

        message(STATUS "Command to generate lcov HTML output: ")
        string(REPLACE ";" " " LCOV_GEN_HTML_CMD_SPACED "${LCOV_GEN_HTML_CMD}")
        message(STATUS "${LCOV_GEN_HTML_CMD_SPACED}")

        if(${Coverage_SONARQUBE})
            message(STATUS "Command to generate SonarQube XML output: ")
            string(REPLACE ";" " " GCOVR_XML_CMD_SPACED "${GCOVR_XML_CMD}")
            message(STATUS "${GCOVR_XML_CMD_SPACED}")
        endif()
    endif()

    # Setup target
    add_custom_target(
        ${Coverage_NAME}
        COMMAND ${LCOV_CLEAN_CMD}
        COMMAND ${LCOV_BASELINE_CMD}
        COMMAND ${LCOV_EXEC_TESTS_CMD}
        COMMAND ${LCOV_CAPTURE_CMD}
        COMMAND ${LCOV_BASELINE_COUNT_CMD}
        COMMAND ${LCOV_FILTER_CMD}
        COMMAND ${LCOV_GEN_HTML_CMD} ${GCOVR_XML_CMD_COMMAND}
        # Set output files as GENERATED (will be removed on 'make clean')
        BYPRODUCTS ${Coverage_NAME}.base ${Coverage_NAME}.capture ${Coverage_NAME}.total ${Coverage_NAME}.info
                   ${GCOVR_XML_CMD_BYPRODUCTS} ${Coverage_NAME}/index.html
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM # Protect arguments to commands
        COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report.")

    # Show where to find the lcov info report
    add_custom_command(
        TARGET ${Coverage_NAME}
        POST_BUILD
        COMMAND ;
        COMMENT "Lcov code coverage info report saved in ${Coverage_NAME}.info." ${GCOVR_XML_CMD_COMMENT})

    # Show info where to find the report
    add_custom_command(
        TARGET ${Coverage_NAME}
        POST_BUILD
        COMMAND ;
        COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report.")

endfunction() # setup_target_for_coverage_lcov

# Defines a target for running and collection code coverage information Builds dependencies, runs the given executable
#
# The user can set the variable GCOVR_ADDITIONAL_ARGS to supply additional flags to the GCVOR command.
function(setup_target_for_coverage_gcovr_xml)

    set(options NONE)
    set(oneValueArgs BASE_DIRECTORY NAME)
    set(multiValueArgs EXCLUDE EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif() # NOT GCOVR_PATH

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(DEFINED Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    # Collect excludes (CMake 3.4+: Also compute absolute paths)
    set(GCOVR_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE} ${COVERAGE_EXCLUDES} ${COVERAGE_GCOVR_EXCLUDES})
        if(CMAKE_VERSION VERSION_GREATER 3.4)
            get_filename_component(EXCLUDE ${EXCLUDE} ABSOLUTE BASE_DIR ${BASEDIR})
        endif()
        list(APPEND GCOVR_EXCLUDES "${EXCLUDE}")
    endforeach()
    list(REMOVE_DUPLICATES GCOVR_EXCLUDES)

    # Combine excludes to several -e arguments
    set(GCOVR_EXCLUDE_ARGS "")
    foreach(EXCLUDE ${GCOVR_EXCLUDES})
        list(APPEND GCOVR_EXCLUDE_ARGS "-e")
        list(APPEND GCOVR_EXCLUDE_ARGS "${EXCLUDE}")
    endforeach()

    # Set up commands which will be run to generate coverage data Run tests
    set(GCOVR_XML_EXEC_TESTS_CMD ${Coverage_EXECUTABLE} ${Coverage_EXECUTABLE_ARGS})
    # Running gcovr
    set(GCOVR_XML_CMD
        ${GCOVR_PATH}
        --xml
        ${Coverage_NAME}.xml
        -r
        ${BASEDIR}
        ${GCOVR_ADDITIONAL_ARGS}
        ${GCOVR_EXCLUDE_ARGS}
        --object-directory=${PROJECT_BINARY_DIR})

    if(CODE_COVERAGE_VERBOSE)
        message(STATUS "Executed command report")

        message(STATUS "Command to run tests: ")
        string(REPLACE ";" " " GCOVR_XML_EXEC_TESTS_CMD_SPACED "${GCOVR_XML_EXEC_TESTS_CMD}")
        message(STATUS "${GCOVR_XML_EXEC_TESTS_CMD_SPACED}")

        message(STATUS "Command to generate gcovr XML coverage data: ")
        string(REPLACE ";" " " GCOVR_XML_CMD_SPACED "${GCOVR_XML_CMD}")
        message(STATUS "${GCOVR_XML_CMD_SPACED}")
    endif()

    add_custom_target(
        ${Coverage_NAME}
        COMMAND ${GCOVR_XML_EXEC_TESTS_CMD}
        COMMAND ${GCOVR_XML_CMD}
        BYPRODUCTS ${Coverage_NAME}.xml
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM # Protect arguments to commands
        COMMENT "Running gcovr to produce Cobertura code coverage report.")

    # Show info where to find the report
    add_custom_command(
        TARGET ${Coverage_NAME}
        POST_BUILD
        COMMAND ;
        COMMENT "Cobertura code coverage report saved in ${Coverage_NAME}.xml.")
endfunction() # setup_target_for_coverage_gcovr_xml

# Defines a target for running and collection code coverage information Builds dependencies, runs the given executable
#
# The user can set the variable GCOVR_ADDITIONAL_ARGS to supply additional flags to the GCVOR command.
function(setup_target_for_coverage_gcovr_html)

    set(options NONE)
    set(oneValueArgs BASE_DIRECTORY NAME)
    set(multiValueArgs EXCLUDE EXECUTABLE EXECUTABLE_ARGS DEPENDENCIES)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif() # NOT GCOVR_PATH

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(DEFINED Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    # Collect excludes (CMake 3.4+: Also compute absolute paths)
    set(GCOVR_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE} ${COVERAGE_EXCLUDES} ${COVERAGE_GCOVR_EXCLUDES})
        if(CMAKE_VERSION VERSION_GREATER 3.4)
            get_filename_component(EXCLUDE ${EXCLUDE} ABSOLUTE BASE_DIR ${BASEDIR})
        endif()
        list(APPEND GCOVR_EXCLUDES "${EXCLUDE}")
    endforeach()
    list(REMOVE_DUPLICATES GCOVR_EXCLUDES)

    # Combine excludes to several -e arguments
    set(GCOVR_EXCLUDE_ARGS "")
    foreach(EXCLUDE ${GCOVR_EXCLUDES})
        list(APPEND GCOVR_EXCLUDE_ARGS "-e")
        list(APPEND GCOVR_EXCLUDE_ARGS "${EXCLUDE}")
    endforeach()

    # Set up commands which will be run to generate coverage data Run tests
    set(GCOVR_HTML_EXEC_TESTS_CMD ${Coverage_EXECUTABLE} ${Coverage_EXECUTABLE_ARGS})
    # Create folder
    set(GCOVR_HTML_FOLDER_CMD ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/${Coverage_NAME})
    # Running gcovr
    set(GCOVR_HTML_CMD
        ${GCOVR_PATH}
        --html
        ${Coverage_NAME}/index.html
        --html-details
        -r
        ${BASEDIR}
        ${GCOVR_ADDITIONAL_ARGS}
        ${GCOVR_EXCLUDE_ARGS}
        --object-directory=${PROJECT_BINARY_DIR})

    if(CODE_COVERAGE_VERBOSE)
        message(STATUS "Executed command report")

        message(STATUS "Command to run tests: ")
        string(REPLACE ";" " " GCOVR_HTML_EXEC_TESTS_CMD_SPACED "${GCOVR_HTML_EXEC_TESTS_CMD}")
        message(STATUS "${GCOVR_HTML_EXEC_TESTS_CMD_SPACED}")

        message(STATUS "Command to create a folder: ")
        string(REPLACE ";" " " GCOVR_HTML_FOLDER_CMD_SPACED "${GCOVR_HTML_FOLDER_CMD}")
        message(STATUS "${GCOVR_HTML_FOLDER_CMD_SPACED}")

        message(STATUS "Command to generate gcovr HTML coverage data: ")
        string(REPLACE ";" " " GCOVR_HTML_CMD_SPACED "${GCOVR_HTML_CMD}")
        message(STATUS "${GCOVR_HTML_CMD_SPACED}")
    endif()

    add_custom_target(
        ${Coverage_NAME}
        COMMAND ${GCOVR_HTML_EXEC_TESTS_CMD}
        COMMAND ${GCOVR_HTML_FOLDER_CMD}
        COMMAND ${GCOVR_HTML_CMD}
        BYPRODUCTS ${PROJECT_BINARY_DIR}/${Coverage_NAME}/index.html # report directory
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM # Protect arguments to commands
        COMMENT "Running gcovr to produce HTML code coverage report.")

    # Show info where to find the report
    add_custom_command(
        TARGET ${Coverage_NAME}
        POST_BUILD
        COMMAND ;
        COMMENT "Open ./${Coverage_NAME}/index.html in your browser to view the coverage report.")

endfunction() # setup_target_for_coverage_gcovr_html

# Defines a target for running and collection code coverage information Builds dependencies, runs the given executable
#
function(setup_target_for_coverage_fastcov)

    set(options NO_DEMANGLE SKIP_HTML)
    set(oneValueArgs BASE_DIRECTORY NAME)
    set(multiValueArgs
        EXCLUDE
        EXECUTABLE
        EXECUTABLE_ARGS
        DEPENDENCIES
        FASTCOV_ARGS
        GENHTML_ARGS
        POST_CMD)
    cmake_parse_arguments(Coverage "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT FASTCOV_PATH)
        message(FATAL_ERROR "fastcov not found! Aborting...")
    endif()

    if(NOT Coverage_SKIP_HTML AND NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    # Set base directory (as absolute path), or default to PROJECT_SOURCE_DIR
    if(Coverage_BASE_DIRECTORY)
        get_filename_component(BASEDIR ${Coverage_BASE_DIRECTORY} ABSOLUTE)
    else()
        set(BASEDIR ${PROJECT_SOURCE_DIR})
    endif()

    # Collect excludes (Patterns, not paths, for fastcov)
    set(FASTCOV_EXCLUDES "")
    foreach(EXCLUDE ${Coverage_EXCLUDE} ${COVERAGE_EXCLUDES} ${COVERAGE_FASTCOV_EXCLUDES})
        list(APPEND FASTCOV_EXCLUDES "${EXCLUDE}")
    endforeach()
    list(REMOVE_DUPLICATES FASTCOV_EXCLUDES)

    # Conditional arguments
    if(CPPFILT_PATH AND NOT ${Coverage_NO_DEMANGLE})
        set(GENHTML_EXTRA_ARGS "--demangle-cpp")
    endif()

    # Set up commands which will be run to generate coverage data
    set(FASTCOV_EXEC_TESTS_CMD ${Coverage_EXECUTABLE} ${Coverage_EXECUTABLE_ARGS})

    set(FASTCOV_CAPTURE_CMD
        ${FASTCOV_PATH}
        ${Coverage_FASTCOV_ARGS}
        --gcov
        ${GCOV_PATH}
        --search-directory
        ${BASEDIR}
        --process-gcno
        --output
        ${Coverage_NAME}.json
        --exclude
        ${FASTCOV_EXCLUDES})

    set(FASTCOV_CONVERT_CMD ${FASTCOV_PATH} -C ${Coverage_NAME}.json --lcov --output ${Coverage_NAME}.info)

    if(Coverage_SKIP_HTML)
        set(FASTCOV_HTML_CMD ";")
    else()
        set(FASTCOV_HTML_CMD ${GENHTML_PATH} ${GENHTML_EXTRA_ARGS} ${Coverage_GENHTML_ARGS} -o ${Coverage_NAME}
                             ${Coverage_NAME}.info)
    endif()

    set(FASTCOV_POST_CMD ";")
    if(Coverage_POST_CMD)
        set(FASTCOV_POST_CMD ${Coverage_POST_CMD})
    endif()

    if(CODE_COVERAGE_VERBOSE)
        message(STATUS "Code coverage commands for target ${Coverage_NAME} (fastcov):")

        message("   Running tests:")
        string(REPLACE ";" " " FASTCOV_EXEC_TESTS_CMD_SPACED "${FASTCOV_EXEC_TESTS_CMD}")
        message("     ${FASTCOV_EXEC_TESTS_CMD_SPACED}")

        message("   Capturing fastcov counters and generating report:")
        string(REPLACE ";" " " FASTCOV_CAPTURE_CMD_SPACED "${FASTCOV_CAPTURE_CMD}")
        message("     ${FASTCOV_CAPTURE_CMD_SPACED}")

        message("   Converting fastcov .json to lcov .info:")
        string(REPLACE ";" " " FASTCOV_CONVERT_CMD_SPACED "${FASTCOV_CONVERT_CMD}")
        message("     ${FASTCOV_CONVERT_CMD_SPACED}")

        if(NOT Coverage_SKIP_HTML)
            message("   Generating HTML report: ")
            string(REPLACE ";" " " FASTCOV_HTML_CMD_SPACED "${FASTCOV_HTML_CMD}")
            message("     ${FASTCOV_HTML_CMD_SPACED}")
        endif()
        if(Coverage_POST_CMD)
            message("   Running post command: ")
            string(REPLACE ";" " " FASTCOV_POST_CMD_SPACED "${FASTCOV_POST_CMD}")
            message("     ${FASTCOV_POST_CMD_SPACED}")
        endif()
    endif()

    # Setup target
    add_custom_target(
        ${Coverage_NAME}
        # Cleanup fastcov
        COMMAND ${FASTCOV_PATH} ${Coverage_FASTCOV_ARGS} --gcov ${GCOV_PATH} --search-directory ${BASEDIR}
                --zerocounters
        COMMAND ${FASTCOV_EXEC_TESTS_CMD}
        COMMAND ${FASTCOV_CAPTURE_CMD}
        COMMAND ${FASTCOV_CONVERT_CMD}
        COMMAND ${FASTCOV_HTML_CMD}
        COMMAND ${FASTCOV_POST_CMD}
        # Set output files as GENERATED (will be removed on 'make clean')
        BYPRODUCTS ${Coverage_NAME}.info ${Coverage_NAME}.json ${Coverage_NAME}/index.html # report directory
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        DEPENDS ${Coverage_DEPENDENCIES}
        VERBATIM # Protect arguments to commands
        COMMENT "Resetting code coverage counters to zero. Processing code coverage counters and generating report.")

    set(INFO_MSG "fastcov code coverage info report saved in ${Coverage_NAME}.info and ${Coverage_NAME}.json.")
    if(NOT Coverage_SKIP_HTML)
        string(APPEND INFO_MSG
               " Open ${PROJECT_BINARY_DIR}/${Coverage_NAME}/index.html in your browser to view the coverage report.")
    endif()
    # Show where to find the fastcov info report
    add_custom_command(
        TARGET ${Coverage_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo ${INFO_MSG})

endfunction() # setup_target_for_coverage_fastcov

function(append_coverage_compiler_flags)
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} ${COVERAGE_COMPILER_FLAGS}"
        PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILER_FLAGS}"
        PARENT_SCOPE)
    set(CMAKE_Fortran_FLAGS
        "${CMAKE_Fortran_FLAGS} ${COVERAGE_COMPILER_FLAGS}"
        PARENT_SCOPE)
    message(STATUS "Appending code coverage compiler flags: ${COVERAGE_COMPILER_FLAGS}")
endfunction() # append_coverage_compiler_flags

# Setup coverage for specific library
function(append_coverage_compiler_flags_to_target name)
    separate_arguments(_flag_list NATIVE_COMMAND "${COVERAGE_COMPILER_FLAGS}")
    target_compile_options(${name} PRIVATE ${_flag_list})
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_Fortran_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(${name} PRIVATE gcov)
    endif()
endfunction()

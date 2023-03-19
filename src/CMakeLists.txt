cmake_minimum_required(VERSION 3.8)

option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
if(HUB_ENABLE_COVERAGE)
    include(CodeCoverage)
    set(COVERAGE_COMPILER_FLAGS "-g --coverage -fno-elide-constructors -fno-default-inline")
    append_coverage_compiler_flags()
    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
endif()

option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

file(GLOB_RECURSE sources ${HUB_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${HUB_INCLUDE_DIR}/*.hpp ${HUB_INCLUDE_DIR}/*.inl)

list(APPEND SOURCE_FILES ${sources} ${headers})

source_group("Header Files\\" FILES ${headers})
source_group("Source Files\\" FILES ${sources})

set(REQUIRED_LIBS)

if(HUB_BUILD_STATIC_LIBRARY)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_STATIC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) # octave
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_EXPORTS)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC -DINTERNAL_BUILD)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNDEBUG)
endif()

if(MINGW)
    set(REQUIRED_LIBS ws2_32)
    get_filename_component(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll
                                          ${Mingw_Path}/libwinpthread-1.dll)
    install(
        PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION bin
        COMPONENT System)
elseif(WIN32)

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(REQUIRED_LIBS Threads::Threads)
endif()

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS})
#set(CMAKE_CXX_STANDARD 11)
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})

# ########################### INSTALL/EXPORT ################################

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    DESTINATION lib)

if(NOT HUB_BUILD_STATIC_LIBRARY)
    install(PROGRAMS $<TARGET_FILE:hub> DESTINATION bin)
endif()

install(
    DIRECTORY ${HUB_INCLUDE_DIR}/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.inl")

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION lib/cmake/${PROJECT_NAME})

export(EXPORT ${PROJECT_NAME} FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# ########################### COVERAGE ################################

if(HUB_ENABLE_TESTS AND HUB_ENABLE_COVERAGE)

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    setup_target_for_coverage_lcov(
        NAME
        coverage-lcov
        EXECUTABLE
        ctest
        --test-dir
        tests
        -j
        ${n_cores}
        DEPENDENCIES
        hub
        test-all
        EXCLUDE
        "/usr/*"
        "*.hpp"
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl
                    ${CMAKE_BINARY_DIR}/)

    endif()

    add_custom_target(coverage COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/coverage-lcov/index.html)
    add_dependencies(coverage coverage-lcov)

    add_custom_target(coverage-all)

    foreach(file ${sources})
        get_filename_component(basename ${file} NAME)
        get_filename_component(parentDir ${file} DIRECTORY)
        get_filename_component(parentName ${parentDir} NAME)
        get_filename_component(name ${basename} NAME_WE)
        set(parentMacro "")
        if(parentName STREQUAL src)
            set(parentMacro "")
        else()
            set(parentMacro "${parentName}-")
        endif()
        if(NOT EXISTS "${PROJECT_SOURCE_DIR}/tests/test-${parentMacro}${name}.cpp")
            message(FATAL_ERROR "test-${parentMacro}${name}.cpp not found in tests directory")
        endif()

        set(sourcesTmp ${SOURCE_FILES})
        list(FILTER sourcesTmp EXCLUDE REGEX ".*/${name}\\.cpp$")

        setup_target_for_coverage_lcov(
            NAME
            coverage-lcov-${name}
            EXECUTABLE
            tests/test-${parentMacro}${name}
            DEPENDENCIES
            hub
            test-${parentMacro}${name}
            EXCLUDE
            "/usr/*"
            ${sourcesTmp})

        add_custom_target(
            coverage-${name}
            COMMAND
                ls ${CMAKE_BINARY_DIR}/coverage-lcov-${name}/index.html && head -n 40
                ${CMAKE_BINARY_DIR}/coverage-lcov-${name}/index.html | grep '100.0 %' || ${HUB_BROWSER}
                ${CMAKE_BINARY_DIR}/coverage-lcov-${name}/index.html)

        add_dependencies(coverage-${name} coverage-lcov-${name})
        add_dependencies(coverage-all coverage-${name})

    endforeach()

endif()

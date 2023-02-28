cmake_minimum_required(VERSION 3.8)

# set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include") set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
if(HUB_ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
endif()

option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

file(GLOB_RECURSE sources ${HUB_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${HUB_INCLUDE_DIR}/*.hpp ${HUB_INCLUDE_DIR}/*.inl)

list(APPEND SOURCE_FILES ${sources} ${headers})

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} PREFIX ../include) source_group("Header Files\\"
# FILES     ${headers}) source_group("Source Files\\" FILES     ${sources})

# Set up such that your IDE organizes the files correctly
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

# add_library(cpp_doxygen_sphinx ${SOURCE_FILES})

# set(CPACK_PROJECT_NAME ${PROJECT_NAME}) set(CPACK_PROJECT_VERSION ${PROJECT_VERSION}) include(CPack)

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

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support UWP compilation : cmake
# -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS})

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ########################### INSTALL/EXPORT ################################

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    DESTINATION lib)

# set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll
# ${Mingw_Path}/libwinpthread-1.dll ) install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin COMPONENT
# System )
if(NOT HUB_BUILD_STATIC_LIBRARY)
    install(# PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/hub${CMAKE_POSTFIX}.dll PROGRAMS $<TARGET_FILE:hub>
            # $<TARGET_FILE_DIR:${PROJECT_NAME}>)
            PROGRAMS $<TARGET_FILE:hub> DESTINATION bin)
endif()

# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(# DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DIRECTORY ${HUB_INCLUDE_DIR}/ DESTINATION include # FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl"
)

# install include headers install(FILES MathFunctions.h DESTINATION include)

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

# get_target_property(properties ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES) message(STATUS ${properties})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           # $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
           $<INSTALL_INTERFACE:include>)

include(CMakePackageConfigHelpers)

# generate the config file that is includes the exports
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

    if (NOT HUB_BROWSER)
        set(HUB_BROWSER "ls" CACHE STRING "User browser to see html docs and coverage")
    endif()

    setup_target_for_coverage_lcov(
        # setup_target_for_coverage_gcovr_xml(
        NAME
        coverage-lcov
        BASE_DIRECTORY
        ${CMAKE_BINARY_DIR}
        EXECUTABLE
        ctest
        --test-dir
        tests
        -j
        ${n_cores}
        DEPENDENCIES
        hub
        test-all
        NO_DEMANGLE
        EXCLUDE
        "/usr/*"
        # "examples/*" "include/*"
        # COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/coverage/index.html
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
#         add_custom_command( TARGET tests  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl ${CMAKE_BINARY_DIR}/)
         add_custom_command(
             TARGET ${PROJECT_NAME}
             POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy_if_different
             ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl
             ${CMAKE_BINARY_DIR}/
         )


#        file(COPY ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl DESTINATION ${CMAKE_BINARY_DIR}/ DEPENDS tests)
    endif()

    add_custom_target(
        coverage
        # COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/coverage-lcov/index.html
        # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        # COMMENT "Generating API documentation with Doxygen"
        # VERBATIM
    )
    add_dependencies(coverage coverage-lcov)

    # setup_target_for_coverage_gcovr_html(NAME coverage-html EXECUTABLE ctest --schedule-random -j ${n_cores}
    # --test-dir tests EXCLUDE "/usr/*" DEPENDENCIES test-file test-measures test-ram test-resolutions test-sensorSpec
    # test-server test-serverSpeed )

endif()

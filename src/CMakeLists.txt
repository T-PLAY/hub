cmake_minimum_required(VERSION 3.8)

#include(FetchContent)
#FetchContent_Declare(meshoptimizer
#    GIT_REPOSITORY    https://github.com/zeux/meshoptimizer.git
#    GIT_TAG v0.18 # or a later release
#    GIT_PROGRESS TRUE
##    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/external/meshoptimizer
##    SUBBUILD_DIR      subbuild
##    BINARY_DIR        build
#)
##FetchContent_QUIET(meshoptimizer ON)
##option(WITH_GFLAGS "" OFF)
##option(WITH_GTEST "" OFF)
##option(WITH_GMOCK "" OFF)
##option(WITH_UNWIND "" OFF)
##option(BUILD_SHARED_LIBS "" OFF)
##option(BUILD_TESTING "" OFF)
#FetchContent_MakeAvailable(meshoptimizer)

find_package(Git)
set(VERSION_UPDATE_FROM_GIT 1)
include(GetVersionFromGitTag)

#set (${PROJECT_NAME}_VERSION_MAJOR 1)
#set (${PROJECT_NAME}_VERSION_MINOR 0)
#set (${PROJECT_NAME}_VERSION_PATCH 0)
#set (${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
message(STATUS "Hub version: ${${PROJECT_NAME}_VERSION}")


option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
if(HUB_ENABLE_COVERAGE)
    include(CodeCoverage)
    set(COVERAGE_COMPILER_FLAGS "-g --coverage -fno-elide-constructors -fno-default-inline")
    append_coverage_compiler_flags()
    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
endif()

option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

file(GLOB_RECURSE sources ${HUB_SOURCE_DIR}/*.cpp ${HUB_EXTERNAL_DIR}/*.cpp)
file(GLOB_RECURSE headers ${HUB_INCLUDE_DIR}/*.hpp ${HUB_INCLUDE_DIR}/*.h ${HUB_INCLUDE_DIR}/*.inl
                          ${HUB_EXTERNAL_DIR}/*.hpp ${HUB_EXTERNAL_DIR}/*.h ${HUB_EXTERNAL_DIR}/*.inl
)

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

#set (${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
set_target_properties(${PROJECT_NAME} PROPERTIES
#        VERSION ${PROJECT_VERSION}
        #SOVERSION 1
        SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
#        VERSION 1.0.0
        VERSION ${${PROJECT_NAME}_VERSION}
)

#if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE MATCHES "Release")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNDEBUG)
endif()

if (HUB_ENABLE_COVERAGE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DCOVERAGE)
endif()

if(MINGW)
    if (CLANG)
            find_package(Threads REQUIRED)
            set(REQUIRED_LIBS Threads::Threads)

    elseif(GCC)
            set(REQUIRED_LIBS ws2_32)
            get_filename_component(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
            message(STATUS "${Mingw_Path}")
            set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll
                                                  ${Mingw_Path}/libwinpthread-1.dll)

            install(
                PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
                DESTINATION ${CMAKE_INSTALL_BINDIR}/${ARCHITECTURE}
                COMPONENT System)
#    elseif(GNU)
    else()
        message(FATAL_ERROR "unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()

elseif(WIN32)
    if (MSVC)
#        get_target_property(rname ${PROJECT_NAME} RUNTIME_OUTPUT_NAME)
#        get_target_property(rdir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)
#        message("rname: ${rname}")  # rname: rname-NOTFOUND
#        message("rdir: ${rdir}")  # rdir: rdir-NOTFOUND
    elseif (Clang)
    else()
        message(FATAL_ERROR "unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()

elseif(APPLE)
    message(FATAL_ERROR "apple detected")

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(REQUIRED_LIBS Threads::Threads)

else()
    message(FATAL_ERROR "unknown system")
endif()

#    find_package(Threads REQUIRED)
#    set(REQUIRED_LIBS Threads::Threads)

target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS})
#target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS} PRIVATE meshoptimizer)
#set(CMAKE_CXX_STANDARD 11)
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})

#add_custom_target(VersionHeader
##  ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/VersionHeader.cmake
#  ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/VersionHeader.cmake
#)
#add_dependencies(${PROJECT_NAME} VersionHeader)

# ########################### INSTALL/EXPORT ################################


if (HUB_BUILD_STATIC_LIBRARY)
        install(
            TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}
#            DESTINATION lib/${ARCHITECTURE}
            DESTINATION ${ARCHITECTURE}/lib
        )
#    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        if (MSVC)
		#if(${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
		if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
            target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
            # Tell linker to include symbol data
            set_target_properties(${PROJECT_NAME} PROPERTIES
                    LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF"
            )

            # Set file name & location
            set_target_properties(${PROJECT_NAME} PROPERTIES
                    #COMPILE_PDB_NAME ${PROJECT_NAME}${CMAKE_POSTFIX}
                    COMPILE_PDB_NAME ${PROJECT_NAME}
                    COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
                    #DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
            )
		endif()

#        message(STATUS "#############$<TARGET_COMPILE_PDB_FILE:${PROJECT_NAME}>")
        #if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb")
		#if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
		if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb DESTINATION ${ARCHITECTURE}/lib OPTIONAL)
        #elseif (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb")
			install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb DESTINATION ${ARCHITECTURE}/lib OPTIONAL)
		endif()
		#else()
        #    message(FATAL_ERROR "pdb not found")
		#endif()
#            install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARCHITECTURE}/lib)
    endif()

else() # Shared library
        install(
            TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}
#            DESTINATION ${ARCHITECTURE}/bin # Linux
            RUNTIME DESTINATION ${ARCHITECTURE}/bin # Windows
#            DESTINATION ${ARCHITECTURE}/bin
        )

    if(MSVC)
#    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
                if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
	#        install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARCHITECTURE}/bin OPTIONAL)
			install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARCHITECTURE}/bin)
        endif()
    endif()
endif()


#if(NOT HUB_BUILD_STATIC_LIBRARY)
#else()
#install(
#    TARGETS ${PROJECT_NAME}
#    EXPORT ${PROJECT_NAME}
#    DESTINATION ${ARCHITECTURE}/bin)


#install(TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}
#        LIBRARY DESTINATION lib/${ARCHITECTURE}
#        ARCHIVE DESTINATION lib/${ARCHITECTURE}
#        RUNTIME DESTINATION bin/${ARCHITECTURE}
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)


#install(TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}
#        LIBRARY DESTINATION ${ARCHITECTURE}/lib
##        ARCHIVE DESTINATION share
#        RUNTIME DESTINATION ${ARCHITECTURE}/bin
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)

#install(TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ARCHITECTURE}
##        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${ARCHITECTURE}
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)

#    install(PROGRAMS $<TARGET_FILE:hub> DESTINATION ${ARCHITECTURE}/bin)
#    install(PROGRAMS hub DESTINATION ${ARCHITECTURE}/bin)
#endif()

install(
    DIRECTORY ${HUB_INCLUDE_DIR}/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "*.inl")

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
#    DESTINATION lib/cmake/${PROJECT_NAME})
    DESTINATION share/${PROJECT_NAME}/cmake)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.h (autogenerate)
    $<INSTALL_INTERFACE:include>
    PRIVATE
    ${HUB_EXTERNAL_DIR}
)

#target_include_directories(${PROJECT_NAME} PRIVATE
#    ${HUB_EXTERNAL_DIR}
#)


#add_definitions(-DHUB_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/\")
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DHUB_VERSION_MAJOR=${hub_VERSION_MAJOR})
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DHUB_VERSION_MINOR=${hub_VERSION_MINOR})
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DHUB_VERSION_PATCH=${hub_VERSION_PATCH})
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DHUB_VERSION=\"${hub_VERSION}\")
#set (HUB_VERSION STRING ${hub_VERSION})

string(TIMESTAMP DATE "%Y/%m/%d %H:%M UTC+0" UTC)
#message(STATUS "###############${DATE}")

configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/Version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Version.h"
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Version.h
    DESTINATION include
)


include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
    INSTALL_DESTINATION "share/${PROJECT_NAME}/cmake/"
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#    VERSION 1.0.0
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
#              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION lib/cmake/${PROJECT_NAME})
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION share/${PROJECT_NAME}/cmake)

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
        "*tiny_obj_loader.h"
#        "*.hpp"
    )

    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tests/DartConfiguration.tcl
                    ${CMAKE_CURRENT_BINARY_DIR}/)

    endif()

    add_custom_target(coverage COMMAND ${HUB_BROWSER} ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov/index.html)
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
#        list(FILTER sourcesTmp EXCLUDE REGEX ".*/${name}\\.hpp$")

        setup_target_for_coverage_lcov(
            NAME
            coverage-lcov-${name}
#            BASE_DIRECTORY "src/"                        # Base directory for report
            EXECUTABLE
            tests/test-${parentMacro}${name}
            DEPENDENCIES
            hub
            test-${parentMacro}${name}
            EXCLUDE
            "/usr/*"
            "*tiny_obj_loader.h"
            ${sourcesTmp})

        add_custom_target(
            coverage-${name}
            COMMAND
                ls ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html && head -n 40
                ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html | grep '100.0 %' || ${HUB_BROWSER}
                ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html)

        add_dependencies(coverage-${name} coverage-lcov-${name})
        add_dependencies(coverage-all coverage-${name})

    endforeach()

endif()

#        install(CODE [[
#        file(GET_RUNTIME_DEPENDENCIES
#            LIBRARIES $<TARGET_FILE:hubd>
#            RESOLVED_DEPENDENCIES_VAR _r_deps
#            UNRESOLVED_DEPENDENCIES_VAR _u_deps
#            CONFLICTING_DEPENDENCIES_PREFIX conflict
#            DIRECTORIES $<TARGET_PROPERTY:hubd,RUNTIME_OUTPUT_DIRECTORY>
#            PRE_INCLUDE_REGEXES "^.*([Hh][Dd][Ff][5]|[Hh][5][Gg][Ee][Oo]).*$"
#            PRE_EXCLUDE_REGEXES "[^\n]+"
#          )
#          foreach(_file ${_r_deps})
#            message(STATUS "Dependency R: ${_file}")
#          endforeach()

#          foreach(_file ${_u_deps})
#            message(STATUS "Dependency U: ${_file}")
#          endforeach()

#          foreach(_file ${conflict_FILENAMES})
#            message(STATUS "Conflict: ${_file}")
#          endforeach()

#          message("RUNTIME_OUTPUT_DIRECTORY: $<TARGET_PROPERTY:hubd,RUNTIME_OUTPUT_DIRECTORY>")    # shows empty
#          ]])

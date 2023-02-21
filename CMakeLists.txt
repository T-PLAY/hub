cmake_minimum_required(VERSION 3.8)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 17) # depends of std::any for metadata representation
#set(CMAKE_CXX_STANDARD 20) # experimental constexpr
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY False)

#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(CMAKE_POSTFIX "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_POSTFIX ${CMAKE_DEBUG_POSTFIX})
endif()

add_definitions(-DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}/\")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources/")
#add_definitions(-DRESOURCES_DIR=\"${CMAKE_SOURCE_DIR}/resources/\")
add_definitions(-DRESOURCES_DIR=\"${RESOURCES_DIR}\")

project(hub)


#set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/)
#set(SRC_TARGET hub)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
#if (NOT CMAKE_INSTALL_PREFIX)
#	set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/${CMAKE_BUILD_TYPE})
#endif()

set(HUB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(HUB_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")

option(HUB_BUILD_STATIC_LIBRARY "Build static library" OFF)

#add_subdirectory(src)
add_subdirectory(server)
add_subdirectory(api)

option(HUB_ENABLE_TESTS "Enable tests" OFF)
if(HUB_ENABLE_TESTS)
#    set(CMAKE_CXX_FLAGS "-g -fprofile-arcs -ftest-coverage -DBUILD_TEST")
#    set(CMAKE_C_FLAGS "-g -fprofile-arcs -ftest-coverage -DBUILD_TEST")
    add_subdirectory(tests)
endif()


#set(ENABLE_COVERAGE "")
#mark_as_advanced(ENABLE_COVERAGE)
#if(HUB_ENABLE_COVERAGE)
#    include(CoverageFunc)
#    # sets ENABLE_COVERAGE to ON if ok
#    setup_coverage()

#    include(CodeCoverage)
#    append_coverage_compiler_flags()
#endif()

#option(HUB_ENABLE_COVERAGE "Collect coverage from hub library" ON)
#if (HUB_ENABLE_COVERAGE)
#    include(CodeCoverage)
#    APPEND_COVERAGE_COMPILER_FLAGS()
#    SETUP_TARGET_FOR_COVERAGE_LCOV(
#            NAME coverage
##            BASE_DIRECTORY ".."
#            BASE_DIRECTORY "${CMAKE_BINARY_DIR}"
#            EXECUTABLE ctest -j ${n_cores} --test-dir tests # Executable in PROJECT_BINARY_DIR
##            DEPENDENCIES
##                test-file
#                EXCLUDE "/usr/*" "${CMAKE_BINARY_DIR}/_deps/*" "${CMAKE_SOURCE_DIR}/tests/*" "_deps/"
##                DEPENDENCIES test-sensorSpec
##                test-file test-measures test-ram test-resolutions test-sensorSpec test-server test-serverSpeed
#                )

##            setup_target_for_coverage_gcovr_html(NAME coverage-html
##                EXECUTABLE ctest --schedule-random -j ${n_cores} --test-dir tests
##                EXCLUDE "/usr/*"
##                DEPENDENCIES test-file test-measures test-ram test-resolutions test-sensorSpec test-server test-serverSpeed
##            )


##    append_coverage_compiler_flags()
##    setup_target_for_coverage_lcov(NAME coverage EXECUTABLE ./bin/tests BASE_DIRECTORY ../coverage)
#endif()



#install(EXPORT hub
#  FILE hub.cmake
#  DESTINATION lib/cmake/hub
#)

add_subdirectory(doc)

#cmake_minimum_required(VERSION 3.8)

# project(hub)
#project(cpp_doxygen_sphinx VERSION 0.1.0)

#include(CTest)
#enable_testing()

# Source and header dir
#set(PROJECT_SOURCE_DIR "src")


## Source files, without extensions or directory names Here assuming each file has both header and impl
#set(SRC_FILES Acquisition InputSensor Measure Native OutputSensor Resolution Sensor SensorSpec Streamer Viewer)
##file(GLOB_RECURSE SRC_FILES *.cpp)
file(GLOB_RECURSE sources ${HUB_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE headers ${HUB_INCLUDE_DIR}/*.hpp ${HUB_INCLUDE_DIR}/*.inl)

### Create SOURCE_FILES
#foreach(file ${SRC_FILES})
#    list(APPEND SOURCE_FILES ${PROJECT_SOURCE_DIR}/${file}.cpp ${PROJECT_INCLUDE_DIR}/${file}.hpp)
#endforeach()

list(APPEND SOURCE_FILES ${sources} ${headers})

#source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} PREFIX ../include)
#source_group("Header Files\\" FILES     ${headers})
#source_group("Source Files\\" FILES     ${sources})

### Set up such that your IDE organizes the files correctly
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

#add_library(cpp_doxygen_sphinx ${SOURCE_FILES})

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

#set(WIN32_REQUIRED_LIBS)
#set(UNIX_REQUIRED_LIBS)
set(REQUIRED_LIBS)



if(HUB_BUILD_STATIC_LIBRARY)
#    add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_STATIC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) #octave
else()
#    add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_EXPORTS)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC -DINTERNAL_BUILD)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNDEBUG)
endif()

# target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

#find_package(Octave REQUIRED)

if(MINGW)

    set(REQUIRED_LIBS ws2_32)
#    message( STATUS "    Installing system-libraries: MinGW DLLs." )
    get_filename_component( Mingw_Path ${CMAKE_CXX_COMPILER} PATH )
#    message( STATUS ${Mingw_Path})
    set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll ${Mingw_Path}/libwinpthread-1.dll )
    install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin COMPONENT System )

elseif( WIN32 )


elseif(UNIX)
    find_package(Threads REQUIRED)
#    set(UNIX_REQUIRED_LIBS Threads::Threads)
    set(REQUIRED_LIBS Threads::Threads)

endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support UWP compilation : cmake
# -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

if (HUB_ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
endif()

#    target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} gcov)
#else()
#    target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS})

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(HUB_ENABLE_EXAMPLES "Enable examples" OFF)
if(HUB_ENABLE_EXAMPLES)
    add_subdirectory(examples)
endif()

# ########################### EXPORT ################################

# set(installable_libs MathFunctions tutorial_compiler_flags) if(TARGET SqrtLibrary) list(APPEND installable_libs
# SqrtLibrary) endif()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    DESTINATION lib)

#    set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll ${Mingw_Path}/libwinpthread-1.dll )
#install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin COMPONENT System )
install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/hub${CMAKE_POSTFIX}.dll
    DESTINATION bin)

# install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(
#    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DIRECTORY ${HUB_INCLUDE_DIR}/
    DESTINATION include
#    FILES_MATCHING
#    PATTERN "*.hpp"
#    PATTERN "*.inl"
)

# install include headers install(FILES MathFunctions.h DESTINATION include)

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION lib/cmake/${PROJECT_NAME})

# get_target_property(properties ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES) message(STATUS ${properties})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
#	    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
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


############################################### COVERAGE

#include(code-coverage)
#add_code_coverage()
#target_code_coverage(${PROJECT_NAME})
#add_code_coverage_all_targets(EXCLUDE tests/*)

#target_code_coverage(test-sensorSpec AUTO ALL EXCLUDE tests/*) # As an executable target, adds to the 'ccov' and ccov-all' targets, and the reports will exclude the non-covered.cpp file, and any files in a test/ folder.


option(HUB_ENABLE_COVERAGE "Collect coverage from hub library" OFF)
if (HUB_ENABLE_TESTS AND HUB_ENABLE_COVERAGE)
    setup_target_for_coverage_lcov(
#    setup_target_for_coverage_gcovr_xml(
        NAME coverage
        #            BASE_DIRECTORY ".."
        EXECUTABLE ctest
        --test-dir tests
        -j ${n_cores}
        DEPENDENCIES
        hub
        server
        #            test-sensorSpec
        test-all
        EXCLUDE
        "/usr/*"
        #                "_deps"
        #                "catch2"
        #                "src"
        #                "${CMAKE_BINARY_DIR}/_deps/*"
        #                "${CMAKE_SOURCE_DIR}/tests/*"
        #                test-file test-measures test-ram test-resolutions test-sensorSpec test-server test-serverSpeed
    )
    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
        file(COPY ${CMAKE_BINARY_DIR}/tests/DartConfiguration.tcl
            DESTINATION ${CMAKE_BINARY_DIR}/
        )
    endif()

#            setup_target_for_coverage_gcovr_html(NAME coverage-html
#                EXECUTABLE ctest --schedule-random -j ${n_cores} --test-dir tests
#                EXCLUDE "/usr/*"
#                DEPENDENCIES test-file test-measures test-ram test-resolutions test-sensorSpec test-server test-serverSpeed
#            )

endif()


###    append_coverage_compiler_flags()
###    setup_target_for_coverage_lcov(NAME coverage EXECUTABLE ./bin/tests BASE_DIRECTORY ../coverage)
#endif()

#if(ENABLE_COVERAGE)
#    set(LCOV_REMOVES
#        "'*catch_*.hpp';'catch_tostring.hpp';'/usr/*';'/home';'*examples*';'*server*';'*external*';'*install*';'*/Catch2/*';'*catch2*';'*_autogen/*';'*build*';'src/*'"
#    )
#    setup_coverage_targets(${ENABLE_COVERAGE} "${LCOV_REMOVES}")

#    # Fastcov is not supported with gcov llvm: disabling for MacOS Source:
#    # https://github.com/RPGillespie6/fastcov/issues/36
#    if(UNIX AND NOT APPLE)
##        setup_target_for_coverage_fastcov(
#        setup_target_for_coverage_lcov(
#            NAME
#            coverage
##            BASE_DIRECTORY "${PROJECT_SOURCE_DIR}"
##            BASE_DIRECTORY "${CMAKE_BINARY_DIR}"
##            BASE_DIRECTORY ../coverage
#            EXECUTABLE
#            ctest
#            --test-dir tests
#            -j $ENV{CMAKE_BUILD_PARALLEL_LEVEL}
#            DEPENDENCIES
#            hub
#            server
#            test-sensorSpec
#            EXCLUDE
#            "catch_tostring.hpp"
#            "*deps*"
#            "*catch_*.hpp"
#            "*build*"
#            "*catch2-src*"
#            "*server*"
#            "/usr/*"
#            "*catch2*"
#            "examples"
#            "${CMAKE_BINARY_DIR}/_deps/*"
#            "${CMAKE_SOURCE_DIR}/tests/*"
#            "external"
#            "install"
#            "/Catch2/"
#            "/catch2/"
#            "/home"
#            "_autogen/"
#            "build"
#            "mocs"
#            "src"
#            "*catch2/"
#        )
##        message(STATUS "Setup coverage")
#    endif()

#endif()


#find_package(codecov REQUIRED)
#add_coverage(${PROJECT_NAME})
#coverage_evaluate()




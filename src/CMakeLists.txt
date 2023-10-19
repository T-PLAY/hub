cmake_minimum_required(VERSION 3.8)

#project(core LANGUAGES CXX)

find_package(Git)
#set(VERSION_UPDATE_FROM_GIT 1)
include(GetVersionFromGitTag)

set(REQUIRED_LIBS "")

message(STATUS "Hub version: ${${PROJECT_NAME}_VERSION}")

option(HUB_BUILD_SERVER "Build implement server" ON)

option(HUB_BUILD_MQTT "Build implement using mqtt" OFF)
if (HUB_BUILD_MQTT)
    find_package(PahoMqttCpp QUIET)
#    find_package(PahoMqttCpp REQUIRED)
    if (PahoMqttCpp_FOUND)
        set(REQUIRED_LIBS ${REQUIRED_LIBS} PahoMqttCpp::paho-mqttpp3)
    else()
        message(FATAL_ERROR "Mqtt not found")
    endif()
endif()

#find_package(Boost QUIET)

option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
if(HUB_ENABLE_COVERAGE)
    include(CodeCoverage)
    set(COVERAGE_COMPILER_FLAGS "-g --coverage -fno-elide-constructors -fno-default-inline -fprofile-update=atomic")
    append_coverage_compiler_flags()
    set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
endif()

#if(WIN32)
#          add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#    set(LIBS_SYSTEM ws2_32)
#elseif(UNIX)
#    set(LIBS_SYSTEM c stdc++)
#endif()


#set(SOURCES_DIR ${HUB_SOURCE_DIR})
#set(HEADERS_DIR ${HUB_INCLUDE_DIR})

#list(REMOVE_ITEM headers ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsync.h)
#list(REMOVE_ITEM headers ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsyncUtils.h)



#if (NOT HUB_BUILD_SERVER)
#    list(FILTER sources EXCLUDE REGEX ".*Server\\.cpp$")
#    list(FILTER headers EXCLUDE REGEX ".*Server\\.hpp$")

#    file(GLOB server_srcs server/*.cpp)
#    list(REMOVE_ITEM sources ${server_srcs})

#    file(GLOB server_headers server/*.hpp)
#    list(REMOVE_ITEM headers ${server_headers})
#endif()

#list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsync.c)
#list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsyncUtils.c)
#file(
#    GLOB
##    GLOB_RECURSE
#    core_headers
#    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
##            ${HUB_EXTERNAL_DIR}/*.hpp
##            ${HUB_EXTERNAL_DIR}/*.h
##    ${HUB_EXTERNAL_DIR}/*.inl
#)
#add_library(core INTERFACE ${core_headers})

file(
#            GLOB
    GLOB_RECURSE
    external_sources
#    ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.cpp
            ${HUB_EXTERNAL_DIR}/*.cpp
            ${HUB_EXTERNAL_DIR}/*.c
)
#list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsync.c)
#list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsyncUtils.c)
file(
#            GLOB
    GLOB_RECURSE
    external_headers
#    ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.hpp
            ${HUB_EXTERNAL_DIR}/*.hpp
            ${HUB_EXTERNAL_DIR}/*.h
#    ${HUB_EXTERNAL_DIR}/*.inl
)
#add_library(external INTERFACE ${external_headers} ${external_sources})
add_library(external OBJECT ${external_headers} ${external_sources})
#add_library(external OBJECT ${external_headers} ${external_sources})
##add_library(external SHARED ${external_headers} ${external_sources})
target_include_directories(external
#    PUBLIC
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
#	$<INSTALL_INTERFACE:include>
    PRIVATE
        ${HUB_EXTERNAL_DIR}
)
#target_compile_options(external PRIVATE -fPIC)
if (UNIX)
	set_property(TARGET external PROPERTY POSITION_INDEPENDENT_CODE ON) # gltfpack
endif()


#list(APPEND SOURCE_FILES ${sources} ${headers})

#source_group("Header Files\\" FILES ${headers})
#source_group("Source Files\\" FILES ${sources})

#set(REQUIRED_LIBS)

set (SRC_TARGETS)

if (NOT XCODE) # unable to have object files as native build with Xcode

    foreach(HUB_SOURCE_DIR ${HUB_SOURCE_DIRS})

        set(TARGET_NAME ${HUB_SOURCE_DIR})
#        set(TARGET_NAME ${PROJECT_NAME})

#        message(STATUS "HUB_SOURCE_DIR: ${HUB_SOURCE_DIR}")

        file(
#            GLOB
            GLOB_RECURSE
            sources
            ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.cpp
#            ${HUB_EXTERNAL_DIR}/*.cpp
#            ${HUB_EXTERNAL_DIR}/*.c
        )
        #list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsync.c)
        #list(REMOVE_ITEM sources ${HUB_EXTERNAL_DIR}/mqtt/MQTTAsyncUtils.c)
        file(
#            GLOB
            GLOB_RECURSE
            headers
            ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_SOURCE_DIR}/*.hpp
#            ${HUB_EXTERNAL_DIR}/*.hpp
#            ${HUB_EXTERNAL_DIR}/*.h
        #    ${HUB_EXTERNAL_DIR}/*.inl
        )

        if (NOT PahoMqttCpp_FOUND)
            list(FILTER sources EXCLUDE REGEX ".*Mqtt\\.cpp$")
            list(FILTER headers EXCLUDE REGEX ".*Mqtt\\.hpp$")
        endif()

        if (NOT Boost_FOUND)
#            message(FATAL_ERROR "ouou")
            list(FILTER sources EXCLUDE REGEX ".*boost/.*\\.cpp$")
            list(FILTER headers EXCLUDE REGEX ".*boost/.*\\.hpp$")
        endif()

#        add_library(${PROJECT_NAME} OBJECT ${sources} ${headers})
#        add_library(${TARGET_NAME} INTERFACE ${sources} ${headers})
#        add_library(${TARGET_NAME} INTERFACE ${headers})
        add_library(${TARGET_NAME} OBJECT ${sources} ${headers}
        )
#        add_library(${TARGET_NAME} SHARED ${sources} ${headers})

        list(APPEND SRC_TARGETS "$<TARGET_OBJECTS:${TARGET_NAME}>")

        target_include_directories(${TARGET_NAME}
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${HUB_EXTERNAL_DIR}
        )

        #target_compile_definitions(${TARGET_NAME} PUBLIC -DINTERNAL_BUILD)
       target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_STATIC) # -> SRC_API = void, future shared
#        target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_EXPORTS) # -> SRC_API = void, future shared
#        target_compile_options(${TARGET_NAME} PRIVATE -fPIC)

        if (PahoMqttCpp_FOUND)
            target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_BUILD_MQTT)
        endif()

#        if (Boost_FOUND)
#            target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_USE_BOOST)
#        endif()

        if (HUB_BUILD_SERVER)
            target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_BUILD_SERVER)
            if (HUB_SERVER_PORT)
                target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
            endif()
        endif()

        #set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden")
        #set_target_properties(${TARGET_NAME} PROPERTIES SOVERSION ${${TARGET_NAME}_VERSION_MAJOR} VERSION
        #                                                                                            ${${TARGET_NAME}_VERSION})


        ############################# COMPILER SPECIFICS

        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            target_compile_definitions(${TARGET_NAME} PUBLIC -DDEBUG)
        else()
            target_compile_definitions(${TARGET_NAME} PUBLIC -DNDEBUG)
        endif()

        if(HUB_ENABLE_COVERAGE)
            target_compile_definitions(${TARGET_NAME} PUBLIC -DCOVERAGE)
        endif()


        if(MINGW)
            if(CLANG)
                set (THREADS_PREFER_PTHREAD_FLAG ON)
                find_package(Threads REQUIRED)
                set(REQUIRED_LIBS ${REQUIRED_LIBS} Threads::Threads)

            elseif(GNU)
                message(FATAL_ERROR "GNU not supported")

            elseif(GCC)
                set(REQUIRED_LIBS ${REQUIRED_LIBS} ws2_32)
                get_filename_component(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
                message(STATUS "${Mingw_Path}")
                set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS ${Mingw_Path}/libgcc_s_seh-1.dll ${Mingw_Path}/libstdc++-6.dll
                                                      ${Mingw_Path}/libwinpthread-1.dll)

                install(
                    PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
                    DESTINATION ${CMAKE_INSTALL_BINDIR}/${ARCHITECTURE}
                    COMPONENT System)
            else()
                message(FATAL_ERROR "unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
            endif()

        elseif(WIN32)
            if(MSVC)

            elseif(Clang)

            else()
                message(FATAL_ERROR "unsupported compiler ${CMAKE_CXX_COMPILER_ID}")
            endif()

        elseif(APPLE)
            #message(FATAL_ERROR "eee")
            #target_compile_options(${TARGET_NAME} PRIVATE "-g")

        elseif(UNIX)
            set (THREADS_PREFER_PTHREAD_FLAG ON)
            find_package(Threads REQUIRED)
            set(REQUIRED_LIBS ${REQUIRED_LIBS} Threads::Threads)
            set_property(TARGET ${TARGET_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) # gltfpack

            if (Clang)
            elseif (GCC)
                message(FATAL_ERROR "eee")
            elseif(GNU)
            else()
                message(FATAL_ERROR "unknown linux system")
            endif()

        else()
            message(FATAL_ERROR "unknown system")
        endif()

#        set(REQUIRED_LIBS ${REQUIRED_LIBS} external)

        #target_link_libraries(${TARGET_NAME} PUBLIC ${REQUIRED_LIBS})
        target_link_libraries(${TARGET_NAME} PRIVATE ${REQUIRED_LIBS})

        install(
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
            DESTINATION include
            FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h"
            PATTERN "*.inl")

    endforeach() # (HUB_SOURCE_DIR ${HUB_SOURCE_DIRS})

endif()  # NOT XCODE

#target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_STATIC)
#target_link_libraries(${PROJECT_NAME} PRIVATE external)

############################# STATIC LIBRARY ###############################

option(HUB_BUILD_STATIC "Build static library" ON)
if (HUB_BUILD_STATIC)

#    if(HUB_BUILD_STATIC_LIBRARY)
#        add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

    if (XCODE)
            add_library(${PROJECT_NAME}_static STATIC ${sources} ${headers})
        target_include_directories(${PROJECT_NAME}_static
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${HUB_EXTERNAL_DIR}
        )
        if (PahoMqttCpp_FOUND)
            target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DHUB_BUILD_MQTT)
        endif()
#        if (Boost_FOUND)
#            target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_USE_BOOST)
#        endif()

        if (HUB_BUILD_SERVER)
            target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DHUB_BUILD_SERVER)
            if (HUB_SERVER_PORT)
                target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
            endif()
        endif()
    else()
#            add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
        add_library(${PROJECT_NAME}_static STATIC ${SRC_TARGETS})
#        add_library(${PROJECT_NAME}_static STATIC ${SRC_TARGETS} ${external_sources} ${external_headers})
        target_include_directories(${PROJECT_NAME}_static
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                $<INSTALL_INTERFACE:include>
#            PRIVATE
#                ${HUB_EXTERNAL_DIR}
        )
        if (HUB_BUILD_SERVER)
            target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DHUB_BUILD_SERVER)
            if (HUB_SERVER_PORT)
                target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
            endif()
        endif()
    endif()

    target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DSRC_STATIC)
    target_link_libraries(${PROJECT_NAME}_static PRIVATE external)

    #target_link_libraries(${PROJECT_NAME}_static PRIVATE ${REQUIRED_LIBS})
#    target_include_directories(${PROJECT_NAME}_static
#        PUBLIC
#            $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}>
#            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
#            $<INSTALL_INTERFACE:include>
#        PRIVATE
#        ${HUB_EXTERNAL_DIR}
#    )
#    set_property(TARGET ${PROJECT_NAME}_static PROPERTY POSITION_INDEPENDENT_CODE ON) # octave, python

    #install(
    #    TARGETS ${PROJECT_NAME}_static
    #    EXPORT ${PROJECT_NAME}
    #    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #)

    install(
        TARGETS ${PROJECT_NAME}_static
        EXPORT ${PROJECT_NAME}_static
        # DESTINATION lib/${ARCHITECTURE}
        DESTINATION ${ARCHITECTURE}/lib)
    if(MSVC)
        if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
            target_compile_options(${PROJECT_NAME}_static PRIVATE /Zi)
            # Tell linker to include symbol data
            set_target_properties(${PROJECT_NAME}_static PROPERTIES LINK_FLAGS "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF")

            # Set file name & location
            set_target_properties(
                ${PROJECT_NAME}_static
                PROPERTIES # COMPILE_PDB_NAME ${PROJECT_NAME}${CMAKE_POSTFIX}
                           COMPILE_PDB_NAME ${PROJECT_NAME} COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
                           # DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
            )
        endif()

        if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
            install(
                FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb
                DESTINATION ${ARCHITECTURE}/lib
                OPTIONAL)
            install(
                FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb
                DESTINATION ${ARCHITECTURE}/lib
                OPTIONAL)
        endif()
    endif()

    if (UNIX)
        set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    endif()

endif() # static library

############################# SHARED LIBRARY

option(HUB_BUILD_SHARED "Build shared library" ON)
if (HUB_BUILD_SHARED)

    if (XCODE)
            add_library(${PROJECT_NAME}_shared SHARED ${sources} ${headers})
        target_include_directories(${PROJECT_NAME}_shared
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                $<INSTALL_INTERFACE:include>
            PRIVATE
                ${HUB_EXTERNAL_DIR}
        )
        if (PahoMqttCpp_FOUND)
            target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DHUB_BUILD_MQTT)
        endif()
#        if (Boost_FOUND)
#            target_compile_definitions(${TARGET_NAME} PUBLIC -DHUB_USE_BOOST)
#        endif()

        if (HUB_BUILD_SERVER)
            target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DHUB_BUILD_SERVER)
            if (HUB_SERVER_PORT)
                target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
            endif()
        endif()
    else()
#            add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
        add_library(${PROJECT_NAME}_shared SHARED ${SRC_TARGETS})
#        target_compile_options(${PROJECT_NAME}_shared PRIVATE -fPIC)
#        add_library(${PROJECT_NAME}_shared SHARED ${SRC_TARGETS} ${external_sources} ${external_headers})
        target_include_directories(${PROJECT_NAME}_shared
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                $<INSTALL_INTERFACE:include>
#            PRIVATE
#                ${HUB_EXTERNAL_DIR}
        )
        if (HUB_BUILD_SERVER)
            target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DHUB_BUILD_SERVER)
            if (HUB_SERVER_PORT)
                target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DHUB_SERVER_PORT=${HUB_SERVER_PORT})
            endif()
        endif()
    endif()

    target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DSRC_EXPORTS)
    target_link_libraries(${PROJECT_NAME}_shared PRIVATE external)

#	target_compile_options(${PROJECT_NAME} PRIVATE "-fpic")
#    target_link_libraries(${PROJECT_NAME} PRIVATE ${REQUIRED_LIBS})
#    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
#    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) # gltfpack
#    target_include_directories(${PROJECT_NAME}
#        PUBLIC
#            $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}>
#            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
#            $<INSTALL_INTERFACE:include>
#        PRIVATE
#        ${HUB_EXTERNAL_DIR}
#    )
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES
        VERSION ${${PROJECT_NAME}_VERSION}
        SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
    )

    install(
        TARGETS ${PROJECT_NAME}_shared
        EXPORT ${PROJECT_NAME}_shared
         DESTINATION ${ARCHITECTURE}/bin # Linux
#        DESTINATION ${ARCHITECTURE}/lib)
        RUNTIME DESTINATION ${ARCHITECTURE}/bin # Windows
    )

    if(MSVC)
        if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
            install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}_shared> DESTINATION ${ARCHITECTURE}/bin)
        endif()
    endif()

    install(
        EXPORT ${PROJECT_NAME}_shared
        FILE ${PROJECT_NAME}Targets.cmake
        # DESTINATION lib/cmake/${PROJECT_NAME})
        DESTINATION share/${PROJECT_NAME}/cmake)
    export(EXPORT ${PROJECT_NAME}_shared FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

#    if (UNIX)
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
#    endif()

endif() # shared library


#else()
#    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
#    target_compile_definitions(${PROJECT_NAME} PRIVATE -DSRC_EXPORTS)
#endif()


# ########################### STATIC BUILD ################################
# Static build for python, octave, matlab APIs

#if(HUB_ENABLE_WRAPPER)
#    add_library(${PROJECT_NAME}_static STATIC ${SOURCE_FILES})
#    target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DSRC_STATIC)
#    set_property(TARGET ${PROJECT_NAME}_static PROPERTY POSITION_INDEPENDENT_CODE ON) # octave, python
#    target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DINTERNAL_BUILD)
#    target_link_libraries(${PROJECT_NAME}_static PUBLIC ${REQUIRED_LIBS})
#    target_include_directories(
#        ${PROJECT_NAME}_static
#        PUBLIC $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
#               $<INSTALL_INTERFACE:include>
#        PRIVATE ${HUB_EXTERNAL_DIR})
#endif()

# ########################### INSTALL/EXPORT ################################

#if(HUB_BUILD_STATIC_LIBRARY)

#else()
#    install(
#        TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}
#        # DESTINATION ${ARCHITECTURE}/bin # Linux
#        RUNTIME DESTINATION ${ARCHITECTURE}/bin # Windows
#    )

#    if(MSVC)
#        if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
#            install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION ${ARCHITECTURE}/bin)
#        endif()
#    endif()
#endif()


string(TIMESTAMP DATE "%Y/%m/%d %H:%M UTC+0" UTC)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.hpp" @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Version.hpp DESTINATION include)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    # INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
    INSTALL_DESTINATION "share/${PROJECT_NAME}/cmake/"
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    # VERSION 1.0.0
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION share/${PROJECT_NAME}/cmake)


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
        "external/*.h"
        "external/*.cpp"
        "/usr/*"
    )
    custom_target_added(coverage-lcov)
#        "*tiny_obj_loader.h"

    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tests/DartConfiguration.tcl
                    ${CMAKE_CURRENT_BINARY_DIR}/
                )
    endif()

    add_custom_target(coverage COMMAND ${HUB_BROWSER} ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov/index.html)
    custom_target_added(coverage)
    add_dependencies(coverage coverage-lcov)

    add_custom_target(coverage-all)
    custom_target_added(coverage-all)

    file(
        GLOB_RECURSE
        src_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_INCLUDE_DIR}/*.cpp)

    foreach(file ${src_headers})
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
            message("parentMacro :${parentMacro}")
            message("name :${name}")
            message(FATAL_ERROR "test-${parentMacro}${name}.cpp not found in tests directory")
        endif()

        set(sourcesTmp ${SOURCE_FILES})
        list(FILTER sourcesTmp EXCLUDE REGEX ".*/${name}\\.cpp$")

        setup_target_for_coverage_lcov(
            NAME
            coverage-lcov-${name}
            # BASE_DIRECTORY "src/"                        # Base directory for report
            EXECUTABLE
            tests/test-${parentMacro}${name}
            DEPENDENCIES
            hub
            test-${parentMacro}${name}
            EXCLUDE
            "external/*.h"
            "external/*.cpp"
            "/usr/*"
            ${sourcesTmp})
#            "*tiny_obj_loader.h"

        add_custom_target(
            coverage-${name}
            COMMAND
                ls ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html && head -n 40
                ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html | grep '100.0 %' || ${HUB_BROWSER}
                ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov-${name}/index.html)
#        custom_target_added(coverage-${name})

        add_dependencies(coverage-${name} coverage-lcov-${name})
        add_dependencies(coverage-all coverage-${name})

    endforeach()

endif()

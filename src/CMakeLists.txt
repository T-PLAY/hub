cmake_minimum_required(VERSION 3.8)

find_package(Git)
include(GetVersionFromGitTag)

set(HUB_LIBRARIES)
set(REQUIRED_LIBS ${HUB_TBB_TARGET})

message(STATUS "${HEADER_MSG} Version: ${${PROJECT_NAME}_VERSION}")

if (HUB_ENABLE_TESTS)
    option(HUB_ENABLE_COVERAGE "Collect coverage from hub library (required tests enable)" OFF)
    if(HUB_ENABLE_COVERAGE)
        include(CodeCoverage)
        set(COVERAGE_COMPILER_FLAGS "-g --coverage -fno-elide-constructors -fno-default-inline -fprofile-update=atomic")
        append_coverage_compiler_flags()
        set(REQUIRED_LIBS ${REQUIRED_LIBS} gcov)
    endif()
endif()

option(HUB_BUILD_STATIC "Build static library" ON)
option(HUB_BUILD_SHARED "Build shared library" ON)

########################## OS specifics ###########################

if(MINGW)
    if(CLANG)
        set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)
        set(REQUIRED_LIBS ${REQUIRED_LIBS} Threads::Threads)

    elseif(GNU)
        message(WARNING "GNU not tested with MinGW")

    elseif(GCC)
        set(REQUIRED_LIBS ${REQUIRED_LIBS} ws2_32)
        get_filename_component(Mingw_Path ${CMAKE_CXX_COMPILER} PATH)
        message(STATUS "[hub] ${Mingw_Path}")
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

elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(REQUIRED_LIBS ${REQUIRED_LIBS} Threads::Threads)

    if(Clang)

    elseif(GCC)
        message(FATAL_ERROR "not supported")
    elseif(GNU)

    else()
        message(FATAL_ERROR "unknown linux system")
    endif()

else()
    message(FATAL_ERROR "unknown system")
endif()

if (REQUIRED_LIBS)
        message(STATUS "${HEADER_MSG} Dependencies: ${REQUIRED_LIBS}")
endif()

############################# Externals #################################

file(GLOB_RECURSE external_sources ${HUB_EXTERNAL_DIR}/*.cpp ${HUB_EXTERNAL_DIR}/*.c)
file(GLOB_RECURSE external_headers ${HUB_EXTERNAL_DIR}/*.hpp ${HUB_EXTERNAL_DIR}/*.h)

if(NOT HUB_USE_ZPP_BITS)
    list(REMOVE_ITEM external_headers ${HUB_EXTERNAL_DIR}/zpp_bits.h)
endif()
add_library(external OBJECT ${external_headers} ${external_sources})
target_include_directories(
    external
    # PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
    PRIVATE # PUBLIC
            ${HUB_EXTERNAL_DIR})
if(UNIX)
    set_property(TARGET external PROPERTY POSITION_INDEPENDENT_CODE ON) # gltfpack
endif()

#set(external_headers_only ${HUB_EXTERNAL_DIR}/serializer/serializer.h)
install(
#    DIRECTORY ${HUB_EXTERNAL_DIR}/
    DIRECTORY ${HUB_EXTERNAL_DIR}/serializer/
    DESTINATION include/external/serializer/
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.h"
    PATTERN "*.inl"
)

# Todo export static lib
#install(
#	EXPORT external
#	FILE ${PROJECT_NAME}Targets.cmake
#	# DESTINATION lib/cmake/${PROJECT_NAME})
#	DESTINATION share/${PROJECT_NAME}/cmake)
#export(EXPORT external FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

set(SRC_TARGETS)
list(APPEND SRC_TARGETS "$<TARGET_OBJECTS:external>")

############################# Modules #################################

if(NOT XCODE) # unable to have object files as native build with Xcode

    foreach(HUB_MODULE ${HUB_MODULES})

        set(TARGET_NAME ${HUB_MODULE})
        string(REPLACE "/" "-" TARGET_NAME "${TARGET_NAME}")

        set(object_sources)
        file(GLOB_RECURSE object_sources ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.cpp)
        set(object_headers)
        file(GLOB_RECURSE object_headers ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.hpp
             ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/*.h)
        if(object_headers MATCHES "Types.hpp")
            message(FATAL_ERROR "Types.hpp shallow Radium Core/Types.hpp definitions")
        endif()

        if(NOT PahoMqttCpp_FOUND)
            list(FILTER object_sources EXCLUDE REGEX ".*Mqtt\\.cpp$")
            list(FILTER object_headers EXCLUDE REGEX ".*Mqtt\\.hpp$")
        endif()

        if(NOT Boost_FOUND)
            list(FILTER object_sources EXCLUDE REGEX ".*boost/.*\\.cpp$")
            list(FILTER object_headers EXCLUDE REGEX ".*boost/.*\\.hpp$")
        endif()

        list(FILTER object_sources EXCLUDE REGEX ".*server/impl/.*\\.cpp$")
        list(FILTER object_headers EXCLUDE REGEX ".*server/impl/.*\\.hpp$")
        list(FILTER object_sources EXCLUDE REGEX ".*Server\\.cpp$")
        list(FILTER object_headers EXCLUDE REGEX ".*Server\\.hpp$")

        if(NOT HUB_BUILD_SENSOR)
            list(FILTER object_sources EXCLUDE REGEX ".*sensor/.*\\.cpp$")
            list(FILTER object_headers EXCLUDE REGEX ".*sensor/.*\\.hpp$")
        endif()

        add_library(${TARGET_NAME} OBJECT ${object_sources} ${object_headers})

        list(APPEND SRC_TARGETS "$<TARGET_OBJECTS:${TARGET_NAME}>")

        target_include_directories(
            ${TARGET_NAME}
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # auto generated Version.hpp from Version.hpp.in
                   ${HUB_EXTERNAL_DIR})

        if(NOT HUB_BUILD_STATIC AND HUB_BUILD_SHARED)
            target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_EXPORTS) # -> SRC_API = void, future shared
        else()
            target_compile_definitions(${TARGET_NAME} PUBLIC -DSRC_STATIC) # -> SRC_API = void, future shared
        endif()

        # ############################ COMPILER SPECIFICS

        if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            target_compile_definitions(${TARGET_NAME} PUBLIC -DDEBUG)
        else()
            target_compile_definitions(${TARGET_NAME} PUBLIC -DNDEBUG)
        endif()

        if(HUB_ENABLE_COVERAGE)
            target_compile_definitions(${TARGET_NAME} PUBLIC -DCOVERAGE)
        endif()

        if(UNIX)
            set_property(TARGET ${TARGET_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON) # gltfpack
        endif()

        target_link_libraries(${TARGET_NAME} PRIVATE ${REQUIRED_LIBS})

        install(
            # DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
            DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_MODULE}/
            DESTINATION include/${HUB_MODULE}
            FILES_MATCHING
            PATTERN "*.hpp"
            PATTERN "*.h"
            PATTERN "*.inl")

    endforeach() # (HUB_MODULE ${HUB_MODULES})

endif() # NOT XCODE

# ############################ STATIC LIBRARY ###############################
# Static build for python, octave, matlab APIs

if(HUB_BUILD_STATIC)

    if(XCODE)
        add_library(${PROJECT_NAME}_static STATIC ${sources} ${headers})
        target_include_directories(
            ${PROJECT_NAME}_static
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                   $<INSTALL_INTERFACE:include>
            PRIVATE ${HUB_EXTERNAL_DIR})

    else()
        add_library(${PROJECT_NAME}_static STATIC ${SRC_TARGETS})
        target_include_directories(
            ${PROJECT_NAME}_static
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                   $<BUILD_INTERFACE:${HUB_EXTERNAL_DIR}>
                   $<INSTALL_INTERFACE:include>
                   $<INSTALL_INTERFACE:include/external>
#                   PRIVATE
#                   ${HUB_EXTERNAL_DIR}
        )
    endif() # (XCODE)

    target_compile_features(${PROJECT_NAME}_static PUBLIC cxx_std_17)
    target_compile_definitions(${PROJECT_NAME}_static PUBLIC -DSRC_STATIC)
#    target_link_libraries(${PROJECT_NAME}_static PRIVATE external ${REQUIRED_LIBS})
    target_link_libraries(${PROJECT_NAME}_static PRIVATE ${REQUIRED_LIBS})

	# Todo export static lib
#           set_target_properties(${PROJECT_NAME}_static PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
#                                                                    SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
#																	OUTPUT_NAME ${PROJECT_NAME}
#           )

if (MSVC AND HUB_BUILD_SHARED)
        if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
                add_custom_command(
                TARGET ${PROJECT_NAME}_static
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
                        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}${CMAKE_POSTFIX}.lib
        #        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
            )
        else()
                add_custom_command(
                TARGET ${PROJECT_NAME}_static
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
                        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}${CMAKE_POSTFIX}.lib
        #        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
            )
        endif()
endif()

    install(
        TARGETS ${PROJECT_NAME}_static
        EXPORT ${PROJECT_NAME}_static
#        # DESTINATION lib/${ARCHITECTURE}
        DESTINATION ${ARCHITECTURE}/lib
#        #RUNTIME DESTINATION ${ARCHITECTURE}/bin # Todo fix
        )

    # Copy lib file with the name of dll (implib not found when using dll)
    if(MSVC)
                #FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}${CMAKE_POSTFIX}.pdb
if(${CMAKE_GENERATOR} MATCHES "Visual Studio.*")
		install(
			FILES ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
			DESTINATION ${ARCHITECTURE}/lib
			RENAME ${PROJECT_NAME}${CMAKE_POSTFIX}.lib
		)
else()
		install(
			FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
			DESTINATION ${ARCHITECTURE}/lib
			RENAME ${PROJECT_NAME}${CMAKE_POSTFIX}.lib
		)
endif()
        install(
			CODE "
				execute_process(
					COMMAND ${CMAKE_COMMAND} -E remove ${PROJECT_NAME}_static${CMAKE_POSTFIX}.lib
					WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${ARCHITECTURE}/lib/
				)
			"
#    COMPONENT my_component
        )
#        install(CODE "execute_process(COMMAND \"${CMAKE_COMMAND} -E remove hub_staticd.lib\" WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${ARCHITECTURE}/lib/)")
        # Todo remove hub_static.lib file (only on windows)
		#COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tests/DartConfiguration.tcl
    endif()

    if(MSVC)
        if(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
            target_compile_options(${PROJECT_NAME}_static PRIVATE /Zi)
            # Tell linker to include symbol data
            set_target_properties(${PROJECT_NAME}_static PROPERTIES LINK_FLAGS
                                                                    "/INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF")

            # Set file name & location
            set_target_properties(
                ${PROJECT_NAME}_static
                PROPERTIES # COMPILE_PDB_NAME ${PROJECT_NAME}${CMAKE_POSTFIX}
                           COMPILE_PDB_NAME ${PROJECT_NAME}_static COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
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
    endif() # MSVC

    # Todo export static lib
            install(
                EXPORT ${PROJECT_NAME}_static
                FILE ${PROJECT_NAME}StaticTargets.cmake
                # DESTINATION lib/cmake/${PROJECT_NAME})
                DESTINATION share/${PROJECT_NAME}/cmake)
#            export(EXPORT ${PROJECT_NAME}_static FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

        # todo check
    if(NOT MSVC)
        set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME}) # multiple rules generate hub.lib (hub.dll do so) on windows
    endif()

    list(APPEND HUB_LIBRARIES static)

endif() # static library

# ############################ SHARED LIBRARY

if(HUB_BUILD_SHARED)

    if(XCODE)
        add_library(${PROJECT_NAME}_shared SHARED ${sources} ${headers})
        target_include_directories(
            ${PROJECT_NAME}_shared
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                   $<INSTALL_INTERFACE:include>
            PRIVATE ${HUB_EXTERNAL_DIR})

    else()
        add_library(${PROJECT_NAME}_shared SHARED ${SRC_TARGETS})
        target_include_directories(
            ${PROJECT_NAME}_shared
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
                   $<BUILD_INTERFACE:${HUB_EXTERNAL_DIR}>
                   $<INSTALL_INTERFACE:include>
                   $<INSTALL_INTERFACE:include/external>
            #PRIVATE
            #${HUB_EXTERNAL_DIR}
        )
    endif() # (XCODE)

    target_compile_features(${PROJECT_NAME}_shared PUBLIC cxx_std_17)
    target_compile_definitions(${PROJECT_NAME}_shared PUBLIC -DSRC_EXPORTS)
#    target_link_libraries(${PROJECT_NAME}_shared PRIVATE external ${REQUIRED_LIBS})
    target_link_libraries(${PROJECT_NAME}_shared PRIVATE ${REQUIRED_LIBS})

    # PUBLIC $<BUILD_INTERFACE:${HUB_INCLUDE_DIR}> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # Version.hpp
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES VERSION ${${PROJECT_NAME}_VERSION}
                                                            SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
                                                            OUTPUT_NAME ${PROJECT_NAME}
#                                                            RUNTIME_OUTPUT_NAME ${PROJECT_NAME}
#                                                            PREFIX ""
#                                                            SUFFIX ".dll"
#	                                                        VS_GLOBAL_IgnoreImportLibrary "false"
#                                                            ARCHIVE_OUTPUT_NAME ${PROJECT_NAME}_shared
#                                                            IMPORT_PREFIX ""
#                                                            IMPORT_SUFFIX ".lib"
    )


    install(
        TARGETS ${PROJECT_NAME}_shared
        EXPORT ${PROJECT_NAME}_shared
        #DESTINATION ${ARCHITECTURE}/bin # Linux
        DESTINATION ${ARCHITECTURE}/lib # Linux
        RUNTIME DESTINATION ${ARCHITECTURE}/bin # Windows
        #RUNTIME DESTINATION ${ARCHITECTURE}/lib # Windows
        #RENAME ${PROJECT_NAME}
    )

    if(MSVC)
#		if (HUB_BUILD_STATIC)
#			add_custom_command(TARGET ${PROJECT_NAME}_shared POST_BUILD
#						COMMAND copy ${CMAKE_CURRENT_BINARY_DIR}/hub_staticd.lib ${CMAKE_CURRENT_BYNARY_DIR}/hubd.lib
#			)
#			add_custom_command(
#				TARGET ${PROJECT_NAME}_shared
#				COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/hub_staticd.lib ${CMAKE_CURRENT_BYNARY_DIR}/hubd.lib)
#		endif()
		# target_compile_options(${PROJECT_NAME}_shared PRIVATE /noimplib)
		# Tell linker to include symbol data
		# set_target_properties(${PROJECT_NAME}_shared PROPERTIES LINK_FLAGS
		# 														"/noimplib")

        if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release")
            install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}_shared> DESTINATION ${ARCHITECTURE}/bin)


            # Set file name & location
            #set_target_properties(
            #    ${PROJECT_NAME}_static
            #    PROPERTIES # COMPILE_PDB_NAME ${PROJECT_NAME}${CMAKE_POSTFIX}
            #               COMPILE_PDB_NAME ${PROJECT_NAME}_static COMPILE_PDB_OUTPUT_DIR ${CMAKE_BINARY_DIR}
            #               # DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
            #)
        endif()
    endif()

    install(
        EXPORT ${PROJECT_NAME}_shared
        FILE ${PROJECT_NAME}SharedTargets.cmake
        DESTINATION share/${PROJECT_NAME}/cmake)
#    export(EXPORT ${PROJECT_NAME}_shared FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")


    list(APPEND HUB_LIBRARIES shared)

endif() # (HUB_BUILD_SHARED)

message(STATUS "${HEADER_MSG} Libraries: (${HUB_LIBRARIES})")

if(HUB_BUILD_SHARED OR HUB_BUILD_STATIC)
    if(HUB_BUILD_SHARED AND HUB_BUILD_STATIC)
        #add_library(hub ALIAS hub_static)
        add_library(hub ALIAS hub_shared) # default
    elseif(HUB_BUILD_SHARED)
        add_library(hub ALIAS hub_shared)
    else()
        add_library(hub ALIAS hub_static)
    endif()
endif()

# ########################### INSTALL/EXPORT ################################

string(TIMESTAMP DATE "%Y/%m/%d %H:%M UTC+0" UTC)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Version.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/Version.hpp" @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Version.hpp DESTINATION include)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "share/${PROJECT_NAME}/cmake/"
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION share/${PROJECT_NAME}/cmake)

# ########################### COVERAGE ################################

# cmake-format: off
if(HUB_ENABLE_TESTS AND HUB_ENABLE_COVERAGE)

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    setup_target_for_coverage_lcov(
        NAME
        hub-coverage-lcov
        EXECUTABLE
        ctest
        --test-dir
        tests
        -j
        ${n_cores}
        DEPENDENCIES
        hub
        hub-test-all
        EXCLUDE
        "external/*.h"
        "external/*.cpp"
        "/usr/*"
    )
    custom_target_added(coverage-lcov)

    if(${CMAKE_VERSION} VERSION_LESS "3.20") # ctest --test-dir option
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_BINARY_DIR}/tests/DartConfiguration.tcl
            ${CMAKE_CURRENT_BINARY_DIR}/
        )
    endif()

    add_custom_target(hub-coverage COMMAND ${HUB_BROWSER} ${CMAKE_CURRENT_BINARY_DIR}/../coverage-lcov/index.html)
    custom_target_added(coverage)
    add_dependencies(hub-coverage hub-coverage-lcov)

    add_custom_target(hub-coverage-all)
    custom_target_added(coverage-all)

    file(
        GLOB_RECURSE
        src_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/${HUB_INCLUDE_DIR}/*.cpp)

    foreach(file ${src_headers})
        message(STATUS "file : ${file}")
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

        message(STATUS "lcov target : ${name}")

        setup_target_for_coverage_lcov(
            NAME
            hub-coverage-lcov-${name}
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

        add_custom_target(
            hub-coverage-${name}
            COMMAND
            ls ${CMAKE_CURRENT_BINARY_DIR}/../hub-coverage-lcov-${name}/index.html && head -n 40
            ${CMAKE_CURRENT_BINARY_DIR}/../hub-coverage-lcov-${name}/index.html | grep '100.0 %' || ${HUB_BROWSER}
            ${CMAKE_CURRENT_BINARY_DIR}/../hub-coverage-lcov-${name}/index.html)

        add_dependencies(hub-coverage-${name} hub-coverage-lcov-${name})
        add_dependencies(hub-coverage-all hub-coverage-${name})

    endforeach()
endif()
# cmake-format: on

cmake_minimum_required(VERSION 3.8)

#project(hub)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.hpp *.h *.inl)


if(HUB_BUILD_STATIC_LIBRARY)
    add_library(${PROJECT_NAME} STATIC ${sources} ${headers})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_STATIC)
else()
    add_library(${PROJECT_NAME} SHARED ${sources} ${headers})
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_EXPORTS)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC -DINTERNAL_BUILD)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNDEBUG)
endif()

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support
## UWP compilation :
## cmake -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0




target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

############################ EXPORT ################################

#set(installable_libs MathFunctions tutorial_compiler_flags)
#if(TARGET SqrtLibrary)
#        list(APPEND installable_libs SqrtLibrary)
#endif()

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}
        DESTINATION lib)

#install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*.inl" 
    )


# install include headers
#install(FILES MathFunctions.h DESTINATION include)

install(EXPORT ${PROJECT_NAME}
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
)

#get_target_property(properties ${PROJECT_NAME} INTERFACE_INCLUDE_DIRECTORIES)
#message(STATUS ${properties})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        #        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        #        $<BUILD_INTERFACE:${SRC_DIR}>
        $<INSTALL_INTERFACE:include>
)


include(CMakePackageConfigHelpers)

# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY AnyNewerVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/${PROJECT_NAME}
)

export(EXPORT ${PROJECT_NAME}
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
)

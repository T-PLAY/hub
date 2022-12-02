cmake_minimum_required(VERSION 3.8)

project(${SRC_TARGET})

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

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support
## UWP compilation :
## cmake -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0


target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(
    DIRECTORY ${SRC_DIR}
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.hpp")

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DNDEBUG)
endif()

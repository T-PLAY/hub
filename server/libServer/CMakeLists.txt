cmake_minimum_required(VERSION 3.8)

project(server)

set(REQUIRED_LIBS)
if(HUB_ENABLE_COVERAGE)
    set(REQUIRED_LIBS ${REQUIRED_LIBS} "gcov")
endif()

file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.hpp *.h)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)

endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support

target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS} hub)

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

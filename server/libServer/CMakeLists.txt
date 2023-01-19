cmake_minimum_required(VERSION 3.8)

project(server)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.hpp *.h)

add_library(${PROJECT_NAME} STATIC ${sources} ${headers})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

if(WIN32)

elseif(UNIX)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()

# if (MSVC) # Here I set the target System to WindowsStore, which *should* enable UWP-support

target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} hub)

# set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14 set(CMAKE_CXX_STANDARD_REQUIRED ON)

# install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION lib)

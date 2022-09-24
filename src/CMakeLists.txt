cmake_minimum_required (VERSION 3.8)

project(${SRC_TARGET})

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

#add_definitions(-DUSE_NATIVE=1)

file(GLOB_RECURSE sources *.cpp *.c)
file(GLOB_RECURSE headers *.hpp *.h)
#set(sources
#        Acquisition.cpp
#        InputSensor.cpp
#        Native.cpp
#        OutputSensor.cpp
#        Sensor.cpp
#        SensorSpec.cpp
#        Viewer.cpp
#        IO/interface.cpp
#        Net/ClientSocket.cpp
#        Net/Socket.cpp
#        IO/Stream.cpp
#)
#set(headers
#        Acquisition.hpp
#        InputSensor.hpp
#        Macros.hpp
#        Native.hpp
#        OutputSensor.hpp
#        Sensor.hpp
#        SensorSpec.hpp
#        Viewer.hpp
#        IO/interface.hpp
#        Net/ClientSocket.hpp
#        Net/Socket.hpp
#        Net/Utils.hpp
#        IO/Stream.hpp
#)


if (HUB_BUILD_STATIC_LIBRARY)
        add_library(${PROJECT_NAME} STATIC
                ${sources}
                ${headers}
        )
        target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_STATIC)
else()
        add_library(${PROJECT_NAME} SHARED
                ${sources}
                ${headers}
        )
        target_compile_definitions(${PROJECT_NAME} PUBLIC -DSRC_EXPORTS)
#        target_compile_definitions(${PROJECT_NAME} PUBLIC -DDEBUG)
endif()


target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}
)

if (WIN32)
        #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)


elseif(UNIX)
        #    set(THREADS_PREFER_PTHREAD_FLAG ON)
        find_package(Threads REQUIRED)
        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()

#if (MSVC)
#	# Here I set the target System to WindowsStore, which *should* enable UWP-support
#	set_target_properties(${PROJECT_NAME}
#		PROPERTIES
#			CMAKE_SYSTEM_NAME WindowsStore
#			CMAKE_SYSTEM_VERSION 10.0
#	)
#    add_compile_options(/ZW)
#endif()

target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${headers} DESTINATION include)


#set(CMAKE_CXX_STANDARD 14) #ULA-OP C++14
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMake -G "Visual Studio 16 2019" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION="10.0" ..

#set(PLATFORM_WINDOWS TRUE)
#set(PLATFORM_UWP TRUE)
#set(UNICODE TRUE)
#add_compile_definitions(PLATFORM_WINDOWS PLATFORM_UWP)
#add_compile_definitions(WINAPI_FAMILY=WINAPI_FAMILY_PC_APP)
#add_compile_definitions(_UNICODE UNICODE)
#add_compile_definitions(UWP)
#add_compile_definitions(WRL_NO_DEFAULT_LIB)
#add_compile_definitions(_DEBUG)
#set(COMPILER_MSVC_64 TRUE)
#set(CMAKE_SYSTEM_PROCESSOR ARM64)
#add_compile_definitions(JM_SSE4_2)
#set(CMAKE_VS_WINRT_BY_DEFAULT TRUE)
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/ZW:nostdlib>)
#add_compile_options($<$<COMPILE_LANGUAGE:CXX>:/FU:platform.winmd>)
#add_link_options(/DEFAULTLIB:WindowsApp.lib)
#add_link_options(/SUBSYSTEM:WINDOWS)
#add_link_options(/APPCONTAINER)
#add_link_options(/WINMD:NO)
#add_compile_options(/EHsc)
#add_compile_options(/permissive-)
#add_compile_options(/W4)
#add_compile_options(/fp:precise)
#add_compile_options(/FC)
#add_compile_options(/Gd)
#add_compile_options(/std:c++17)
#add_compile_options(/Zc:wchar_t)
#add_compile_options(/Zc:inline)
#add_compile_options(/Zc:forScope)
#add_compile_options(/GT)
#add_compile_options(/sdl)
#add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
#add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)
#add_compile_options(/wd26812)
#add_compile_definitions(VMEM_REQUIRE_COMMIT)
#add_link_options(/NXCOMPAT)
#add_link_options(/DYNAMICBASE)
#add_link_options(/MANIFESTUAC:NO)
#add_link_options(/OPT:NOICF)
#add_compile_options(/Zi)
#add_compile_options(/MDd)
#add_link_options(/DEBUG)
#add_link_options(/OPT:NOREF)
#add_link_options(/MACHINE:ARM64)

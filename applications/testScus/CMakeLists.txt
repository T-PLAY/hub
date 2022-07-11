cmake_minimum_required (VERSION 3.8)

#project ("HubV2")
project(testScus)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

find_package(Scus REQUIRED)
find_package(DCMTK REQUIRED)

message(STATUS ${SELF_DIR})

add_executable(${PROJECT_NAME}
    ${sources}
    ${headers}
    )


target_include_directories(${PROJECT_NAME} PUBLIC
    #    ${PROJECT_SOURCE_DIR}
    #    ${SRC_DIR}
    ${SCUS_INCLUDE_DIR}
    DCMTK::dcmdata DCMTK::dcmimage
    )

if (WIN32)
    #    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)


elseif(UNIX)
    #    set(THREADS_PREFER_PTHREAD_FLAG ON)
    #        find_package(Threads REQUIRED)
    #        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} Scus)


#message(STATUS "------------")
#message(STATUS ${DCMTK_LIBRARIES})
#foreach(DCMTK_LIB ${DCMTK_LIBRARIES})
##    message(STATUS ${DCMTK_LIB})
#    get_target_property(DCMTK_LIBRARY DCMTK::${DCMTK_LIB} IMPORTED_LOCATION_RELEASE)
#    install(FILES ${DCMTK_LIBRARY} DESTINATION lib)
#endforeach()
#message(STATUS ${DCMTK_INCLUDE_DIRS})



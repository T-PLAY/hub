cmake_minimum_required(VERSION 3.8)

project(octave-api)

#set(WIN32_REQUIRED_LIBS)
#set(UNIX_REQUIRED_LIBS)
set(REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE tests *.m)

add_library(${PROJECT_NAME} ${sources}) # ${headers}

#add_custom_target(test DEPENDS ${tests})
#add_dependencies(${PROJECT_NAME} test)

#set(TARGET_TEST "tests")
#set(TARGET_TEST ${PROJECT_NAME})
add_custom_target(octave-tests SOURCES ${tests})
#target_optional_sources(${PROJECT_NAME} PRIVATE ${tests})

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${SRC_DIR})
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
find_package(Octave REQUIRED)
set(REQUIRED_LIBS ${OCTAVE_LIBRARIES})

#if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#elseif(UNIX)

#endif()

#target_link_libraries(${PROJECT_NAME} hub ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} server)
target_link_libraries(${PROJECT_NAME} hub ${REQUIRED_LIBS})

#message(STATUS ${PROJECT_BINARY_DIR})


#message(STATUS ${HUB_INCLUDE_DIR})
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
#    COMMAND mkoctfile ${sources} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhubd
    COMMAND mkoctfile ${sources} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhub${CMAKE_POSTFIX}
)

#message(STATUS ${RESOURCES_DIR})
add_custom_command(
#    TARGET ${TARGET_TEST}
    TARGET ${PROJECT_NAME}
#    TARGET api-octave
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${tests} $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES_DIR}/anechoicTarget.txt $<TARGET_FILE_DIR:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
)


add_custom_command(
#    TARGET ${TARGET_TEST}
#    OUTPUT test_matpow.m
    TARGET ${PROJECT_NAME}
#    TARGET api-octave-test
    POST_BUILD
    COMMAND octave --no-gui --silent ${tests}
#    MAIN_DEPENDENCY test_matpow.m
#    DEPENDS ${PROJECT_NAME}
#    DEPFILE ${tests}
)

#configure_file(test_matpow.m output.m)

#add_executable(test-matpow "test_matpow.m")


#add_custom_command(
##    TARGET ${PROJECT_NAME}
#    api-octave-test
##    POST_BUILD
#    COMMAND octave --no-gui --silent ${tests}
#)


#add_custom_target(
###    TARGET ${PROJECT_NAME}
#    TARGET octave
#    COMMAND octave --no-gui --silent ${tests}
#)

#add_executable(api-octave-test2 IMPORTED api-octave-test)

# install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)

# install(
#     TARGETS ${PROJECT_NAME}
#     EXPORT ${PROJECT_NAME}
#     DESTINATION bin)

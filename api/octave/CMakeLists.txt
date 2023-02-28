cmake_minimum_required(VERSION 3.8)

project(octave-api)

#set(WIN32_REQUIRED_LIBS)
#set(UNIX_REQUIRED_LIBS)
set(REQUIRED_LIBS)

#file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE tests *.m)


#add_custom_target(tests DEPENDS ${tests})
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
#    add_custom_command(
#        TARGET ${PROJECT_NAME}
#        POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)

#elseif(UNIX)

#endif()

#OCTAVE_MEX_CREATE(test)

#set(MKOCTFILE_EXECUTABLE ${OCTAVE_)

#message(STATUS ${HUB_INCLUDE_DIR})

file(GLOB API_SRC *.cpp)

#string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}")
#set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

#foreach(name ${EXAMPLE_SRC_BASENAME})
#    get_filename_component(basename ${name} NAME)
#    set(TARGET_NAME ${basename})
#    add_executable(${TARGET_NAME} ${name}.cpp)
#    # target_link_libraries(${TARGET_NAME} hub libServer)
#    target_link_libraries(${TARGET_NAME} hub)
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
#endforeach()

string(REPLACE ".cpp" "" API_SRC_BASENAME "${API_SRC}")
set(API_SRC_BASENAME ${API_SRC_BASENAME})


foreach(name ${API_SRC_BASENAME})
    message(STATUS ${name})

#    set(SRC_TARGET "${PROJECT_NAME}-${source}")
    set(source ${name}.cpp)
    set(test ${name}-test.m)

    get_filename_component(test-basename ${test} NAME)

    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME octave-${basename})


#    set_source_files_properties(${test} PROPERTIES LANGUAGE NONE)

#    project(${TARGET_NAME})
#    set(TARGET_NAME ${PROJECT_NAME})
    add_library(${TARGET_NAME} ${source})
#set(sources ${source})
#    OCTAVE_MEX_CREATE(${TARGET_NAME} ${test})
#     target_link_libraries(${TARGET_NAME} hub libServer)
    target_link_libraries(${TARGET_NAME} hub)

#    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_14)

#    source_group("Source Files" FILES ${source} ${test})
#    source_group(test FILES ${test} ${source})


#    add_custom_target(${TARGET_NAME}-test SOURCES ${test})
#    add_dependencies(${TARGET_NAME} ${TARGET_NAME}-test)

#    add_library(${TARGET_NAME} ${sources}) # ${headers}
#    #target_link_libraries(${TARGET_NAME} hub ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} server)
#    target_link_libraries(${TARGET_NAME} hub ${REQUIRED_LIBS})
    target_include_directories(${TARGET_NAME} PUBLIC ${OCTAVE_INCLUDE_DIR})
#    #message(STATUS ${PROJECT_BINARY_DIR})

    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${TARGET_NAME}>)

    add_custom_command(
        TARGET ${TARGET_NAME}
        POST_BUILD
    #    COMMAND mkoctfile ${sources} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhubd
    #    COMMAND mkoctfile ${sources} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhub${CMAKE_POSTFIX}
        COMMAND ${MKOCTFILE_EXECUTABLE} ${source} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhub${CMAKE_POSTFIX}
    )

    add_custom_command(
    #    TARGET ${TARGET_TEST}
        TARGET ${TARGET_NAME}
    #    TARGET api-octave
        POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy "${tests}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES_DIR}/anechoicTarget.txt $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/OctaveViewer.m $<TARGET_FILE_DIR:${TARGET_NAME}>
        DEPENDS ${TARGET_NAME}
    )


    add_custom_command(
    #    TARGET ${TARGET_TEST}
    #    OUTPUT test_matpow.m
        TARGET ${TARGET_NAME}
    #    TARGET api-octave-test
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${test} $<TARGET_FILE_DIR:${TARGET_NAME}>
    #    COMMAND octave --no-gui --silent ${tests}
        COMMAND ${OCTAVE_EXECUTABLE} --no-gui --silent ${test-basename}
    #    MAIN_DEPENDENCY test_matpow.m
        DEPENDS ${test}
        DEPFILE ${test}
    )

endforeach()

#message(STATUS ${RESOURCES_DIR})

#foreach(test ${tests})

#endforeach()

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

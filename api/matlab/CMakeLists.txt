cmake_minimum_required(VERSION 3.8)

project(matlab-api)

#set(CMAKE_CXX_STANDARD 11)
#add_compile_definitions($<$<AND:$<BOOL:${MSVC}>,$<COMPILE_LANGUAGE:C,CXX>>:_CRT_SECURE_NO_WARNINGS>)


#set(WIN32_REQUIRED_LIBS)
#set(UNIX_REQUIRED_LIBS)
set(REQUIRED_LIBS)

file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE tests *.m)

#add_library(${PROJECT_NAME} ${sources}) # ${headers}
#target_link_libraries(${PROJECT_NAME} hub ${REQUIRED_LIBS} Matlab::mex)

add_custom_target(matlab-tests SOURCES ${tests})

#set(Matlab_INCLUDE_DIRS "/home/gauthier/usr/MATLAB/R2022b/extern/include")
#set(Matlab_ROOT_DIR "/home/gauthier/usr/MATLAB/R2022b/")
find_package(Matlab COMPONENTS MAIN_PROGRAM REQUIRED)


#matlab_add_unit_test(
#    NAME mexFunTest
#    UNITTEST_FILE test_MexFunction.m
#)

# set(Matlab_INCLUDE_DIRS "${Matlab_ROOT_DIR}/extern/include")
set(MATLAB_EXE "${Matlab_ROOT_DIR}/bin/matlab")


file(GLOB API_SRC *.cpp)

string(REPLACE ".cpp" "" API_SRC_BASENAME "${API_SRC}")
set(API_SRC_BASENAME ${API_SRC_BASENAME})


foreach(name ${API_SRC_BASENAME})
    message(STATUS ${name})

#    set(SRC_TARGET "${PROJECT_NAME}-${source}")
    set(source ${name}.cpp)
    set(test ${name}_test.m)

    get_filename_component(test-basename ${test} NAME)

    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME matlab-${basename})
#    set(TARGET_NAME ${basename})

    matlab_add_mex(
#        NAME ${TARGET_NAME}-mex
        NAME ${TARGET_NAME}
#        NAME ${basename}
        SRC ${source}
        LINK_TO hub
        OUTPUT_NAME ${basename}
    )

    #

    add_custom_command(
    #    TARGET ${TARGET_TEST}
        TARGET ${TARGET_NAME}
    #    TARGET api-octave
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/MatlabViewer.m $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES_DIR}/anechoicTarget.txt $<TARGET_FILE_DIR:${TARGET_NAME}>
        DEPENDS ${PROJECT_NAME}
    )

    add_custom_command(
    #    TARGET ${TARGET_TEST}
    #    OUTPUT test_matpow.m
        TARGET ${TARGET_NAME}
    #    TARGET api-octave-test
        POST_BUILD
    #    COMMAND ${MATLAB_EXE} -batch \'run(\"${tests}\")\'
        COMMAND ${CMAKE_COMMAND} -E copy ${test} $<TARGET_FILE_DIR:${TARGET_NAME}>
        COMMAND ${MATLAB_EXE} -batch 'run(\"${test-basename}\")' -nojvm
    #    COMMAND ${MATLAB_EXE} -batch 'run(\""${tests}"\")'
    #    MAIN_DEPENDENCY test_matpow.m

    #    DEPENDS ${PROJECT_NAME}
    #    DEPFILE ${tests}
    )

endforeach()


#
# #add_custom_target(test DEPENDS ${tests})
# #add_dependencies(${PROJECT_NAME} test)
#
# #set(TARGET_TEST "tests")
# #set(TARGET_TEST ${PROJECT_NAME})
# add_custom_target(tests SOURCES ${tests})
# #target_optional_sources(${PROJECT_NAME} PRIVATE ${tests})
#
# #target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} ${SRC_DIR})
# #target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
# find_package(Octave REQUIRED)
# set(REQUIRED_LIBS ${OCTAVE_LIBRARIES})
#
# #if(WIN32)
#     add_custom_command(
#         TARGET ${PROJECT_NAME}
#         POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
#
# #elseif(UNIX)
#
# #endif()
#
# #target_link_libraries(${PROJECT_NAME} hub ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS} server)
#
# #message(STATUS ${PROJECT_BINARY_DIR})
#
#
# #message(STATUS ${HUB_INCLUDE_DIR})
# add_custom_command(
#     TARGET ${PROJECT_NAME}
#     POST_BUILD
#     COMMAND mkoctfile ${sources} -I${HUB_INCLUDE_DIR} -L${PROJECT_BINARY_DIR} -lhubd
# )
#
# #message(STATUS ${RESOURCES_DIR})
# add_custom_command(
# #    TARGET ${TARGET_TEST}
#     TARGET ${PROJECT_NAME}
# #    TARGET api-octave
#     POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy ${tests} $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     COMMAND ${CMAKE_COMMAND} -E copy ${RESOURCES_DIR}/anechoicTarget.txt $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     DEPENDS ${PROJECT_NAME}
# )
#
#
# add_custom_command(
# #    TARGET ${TARGET_TEST}
# #    OUTPUT test_matpow.m
#     TARGET ${PROJECT_NAME}
# #    TARGET api-octave-test
#     POST_BUILD
#     COMMAND octave --no-gui --silent ${tests}
# #    MAIN_DEPENDENCY test_matpow.m
# #    DEPENDS ${PROJECT_NAME}
# #    DEPFILE ${tests}
# )
#
# #configure_file(test_matpow.m output.m)
#
# #add_executable(test-matpow "test_matpow.m")
#
#
# #add_custom_command(
# ##    TARGET ${PROJECT_NAME}
# #    api-octave-test
# ##    POST_BUILD
# #    COMMAND octave --no-gui --silent ${tests}
# #)
#
#
# #add_custom_target(
# ###    TARGET ${PROJECT_NAME}
# #    TARGET octave
# #    COMMAND octave --no-gui --silent ${tests}
# #)
#
# #add_executable(api-octave-test2 IMPORTED api-octave-test)
#
# # install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION bin)
#
# # install(
# #     TARGETS ${PROJECT_NAME}
# #     EXPORT ${PROJECT_NAME}
# #     DESTINATION bin)

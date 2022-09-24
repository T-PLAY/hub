
cmake_minimum_required( VERSION 3.5 )

project( examples LANGUAGES CXX )


file(GLOB_RECURSE EXAMPLE_SRC *.cpp)
# These examples use the standard separate compilation
#set( EXAMPLE_SRC
#    010-TestCase.cpp
#)
#message(STATUS "examples/CMakeLists.txt --------------------")
#message(STATUS ${SOURCES_IDOMATIC_EXAMPLES})

string( REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}" )
set( EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME} )


foreach( name ${EXAMPLE_SRC_BASENAME} )
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME example-${basename})
    add_executable( ${TARGET_NAME}
        ${name}.cpp )
    target_link_libraries( ${TARGET_NAME} ${SRC_TARGET} server )
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)
endforeach()


#set(ALL_EXAMPLE_TARGETS
#    ${EXAMPLE_SRC_BASENAME}
#)

#foreach( name ${ALL_EXAMPLE_TARGETS} )
#    target_link_libraries( ${name} Catch2 Catch2WithMain )
#endforeach()


# list(APPEND CATCH_WARNING_TARGETS ${ALL_EXAMPLE_TARGETS})
# set(CATCH_WARNING_TARGETS ${CATCH_WARNING_TARGETS} PARENT_SCOPE)

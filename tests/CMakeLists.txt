


cmake_minimum_required( VERSION 3.5 )

project( tests LANGUAGES CXX )


Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)


# These examples use the standard separate compilation
set( SOURCES_IDIOMATIC_EXAMPLES
    010-TestCase.cpp
)

string( REPLACE ".cpp" "" BASENAMES_IDIOMATIC_EXAMPLES "${SOURCES_IDIOMATIC_EXAMPLES}" )
set( TARGETS_IDIOMATIC_EXAMPLES ${BASENAMES_IDIOMATIC_EXAMPLES} )


foreach( name ${TARGETS_IDIOMATIC_EXAMPLES} )
    add_executable( ${name}
        ${name}.cpp )
endforeach()

set(ALL_EXAMPLE_TARGETS
    ${TARGETS_IDIOMATIC_EXAMPLES}
)

foreach( name ${ALL_EXAMPLE_TARGETS} )
    target_link_libraries( ${name} Catch2 Catch2WithMain )
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${name} PROPERTY CXX_EXTENSIONS OFF)
endforeach()


# list(APPEND CATCH_WARNING_TARGETS ${ALL_EXAMPLE_TARGETS})
# set(CATCH_WARNING_TARGETS ${CATCH_WARNING_TARGETS} PARENT_SCOPE)

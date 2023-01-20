# first we can indicate the documentation build as an option and set it to ON by default option(BUILD_DOC "Build
# documentation" ON)

# check if Doxygen is installed find_package(Doxygen) if (DOXYGEN_FOUND) # set input and output files # set(DOXYGEN_IN
# ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in) # set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#
# # request to configure the file configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY) message("Doxygen build started")
#
# # note the option ALL which allows to build the docs together with the application add_custom_target( doc_doxygen ALL
# COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Generating API
# documentation with Doxygen" VERBATIM ) else (DOXYGEN_FOUND) message("Doxygen need to be installed to generate the
# doxygen documentation") endif (DOXYGEN_FOUND)

cmake_minimum_required(VERSION 3.0.0)


# Doxygen

option(HUB_BUILD_DOC "Build documentation" OFF)

if(HUB_BUILD_DOC)
    # look for Doxygen package find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

    set(DOXYGEN_EXECUTABLE "E:/Documents/tmpMedical/doxygen/out/install/x64-Debug/bin/doxygen.exe")

#    find_package(Doxygen REQUIRED)
    find_package(Doxygen QUIET)

#    if(NOT ${DOXYGEN_FOUND})
#        include(FetchContent)

#        FetchContent_Declare(
#            Flex
#            GIT_REPOSITORY https://github.com/lexxmark/winflexbison.git
##            GIT_TAG Release_1_9_6 # or a later release
#            GIT_PROGRESS TRUE)
#        FetchContent_MakeAvailable(Flex)


##        message(STATUS ${CMAKE_BINARY_DIR})

#        set(FLEX_EXECUTABLE "${CMAKE_BINARY_DIR}/_deps/flex-build/flex/win_flex.exe")
#        set(BISON_EXECUTABLE "${CMAKE_BINARY_DIR}/_deps/flex-build/bison/win_bison.exe")
#        set(DOXYGEN_EXECUTABLE "${CMAKE_BINARY_DIR}/_deps/doxygen-build/")

#        FetchContent_Declare(
#            Doxygen
#            GIT_REPOSITORY https://github.com/doxygen/doxygen.git
##            GIT_TAG Release_1_9_6 # or a later release
#            GIT_PROGRESS TRUE)
#        FetchContent_MakeAvailable(Doxygen)
#    endif()

    if(DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # Note: do not put "ALL" - this builds docs together with application EVERY TIME!
        add_custom_target(
            docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)

    else(DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)

endif()

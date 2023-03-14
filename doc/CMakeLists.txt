# check if Doxygen is installed find_package(Doxygen) if (DOXYGEN_FOUND) # set input and output files # set(DOXYGEN_IN
# ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in) # set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile) # request to
# configure the file configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY) message("Doxygen build started") # note the
# option ALL which allows to build the docs together with the application add_custom_target( doc_doxygen ALL

cmake_minimum_required(VERSION 3.0.0)

if(HUB_BUILD_DOC)

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    find_package(Doxygen QUIET)

    # FetchContent_Declare( Flex GIT_REPOSITORY https://github.com/lexxmark/winflexbison.git GIT_TAG Release_1_9_6 # or

    # FetchContent_Declare( Doxygen GIT_REPOSITORY https://github.com/doxygen/doxygen.git GIT_TAG Release_1_9_6 # or a

    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.out)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        add_custom_target(
            doc-doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/doc/docs_doxygen/html/annotated.html
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)

    else(DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)

    add_custom_target(
        doc-sphinx
        COMMAND make -C ${PROJECT_SOURCE_DIR}/doc/docs_sphinx html
        COMMAND ${HUB_BROWSER} ${PROJECT_SOURCE_DIR}/doc/docs_sphinx/_build/html/api/index.html
        COMMENT "Generating API documentation with Sphinx")

endif()

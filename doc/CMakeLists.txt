cmake_minimum_required(VERSION 3.5)

project(doc)

if(HUB_BUILD_DOC)

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    ############################ DOXYGEN #############################

    find_package(Doxygen QUIET)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile.out)

        message(STATUS "${HEADER_MSG} Doxygen ${DOXYGEN_VERSION} found")

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(
            hub-doc-doxygen
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/doc/doxygen/html/annotated.html
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
        custom_target_added(doc-doxygen)

    else(DOXYGEN_FOUND)
        message(WARNING "Doxygen have to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)

    ############################# SPHINX #############################

    find_package(Sphinx QUIET)
    if (Sphinx_FOUND)
        set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/sphinx)
        set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html)

        add_custom_target(hub-doc-sphinx
              COMMAND ${SPHINX_EXECUTABLE} -b html ${SPHINX_SOURCE} ${SPHINX_BUILD}
              COMMAND ${HUB_BROWSER} ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html/api/index.html
              COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/_build/html ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html/classes
              COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/index.html ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html/
              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
              COMMENT "Generating documentation with Sphinx"
              VERBATIM
        )
        custom_target_added(doc-sphinx)
    endif()

endif()

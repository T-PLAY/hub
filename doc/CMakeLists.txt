cmake_minimum_required(VERSION 3.5)

project(doc)

if(HUB_BUILD_DOC)

    # add_subdirectory(cpp)

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    find_package(Doxygen REQUIRED)

    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile.out)
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
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)

    # set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/Doxyfile.in)
    # set(DOXYGEN_OUT ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/Doxyfile.out)
    # set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/sphinx/Doxyfile.out)
    # configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    # file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sphinx DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

    find_package(Sphinx REQUIRED)
    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/sphinx)
    set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html)

    add_custom_target(hub-doc-sphinx
          COMMAND ${SPHINX_EXECUTABLE} -b html ${SPHINX_SOURCE} ${SPHINX_BUILD}
          COMMAND ${HUB_BROWSER} ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html/api/index.html
          COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/sphinx/_build/html ${CMAKE_CURRENT_BINARY_DIR}/sphinx/html/classes
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating documentation with Sphinx"
          VERBATIM
    )

    # add_custom_target(
    #     hub-doc-sphinx
    #     COMMAND export BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}/sphinx"
    #     COMMAND make -C ${HUB_PROJECT_DIR}/doc/sphinx html
    #     COMMAND ${HUB_BROWSER} ${CMAKE_BINARY_DIR}/doc/sphinx/html/api/index.html
    #     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    #     COMMENT "Generating API documentation with Sphinx"
    #     VERBATIM
    # )
    custom_target_added(doc-sphinx)

endif()

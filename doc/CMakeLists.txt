cmake_minimum_required(VERSION 3.5)

project(doc)

if(HUB_BUILD_DOC)


    file(GLOB DOC_SRC *.cpp)

    add_library(${PROJECT_NAME} ${DOC_SRC})
    target_link_libraries(${PROJECT_NAME} hub)
#        set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#        set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

#    string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${DOC_SRC}")
#    set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

#    foreach(name ${EXAMPLE_SRC_BASENAME})
#        get_filename_component(basename ${name} NAME)
#        set(TARGET_NAME ${basename})
#        add_executable(${TARGET_NAME} ${name}.cpp)
#        target_link_libraries(${TARGET_NAME} hub)
#        set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
#        set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

#        if(WIN32)
#            add_custom_command(
#                # TARGET ${PROJECT_NAME}
#                TARGET ${TARGET_NAME}
#                POST_BUILD
#                COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${TARGET_NAME}>)
#        endif()
#    endforeach()

    if(NOT HUB_BROWSER)
        set(HUB_BROWSER
            "ls"
            CACHE STRING "User browser to see html docs and coverage")
    endif()

    find_package(Doxygen REQUIRED)

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
        custom_target_added(doc-doxygen)

    else(DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif(DOXYGEN_FOUND)

    add_custom_target(
        doc-sphinx
        COMMAND make -C ${PROJECT_SOURCE_DIR}/doc/docs_sphinx html
        COMMAND ${HUB_BROWSER} ${PROJECT_SOURCE_DIR}/doc/docs_sphinx/_build/html/api/index.html
        COMMENT "Generating API documentation with Sphinx")
    custom_target_added(doc-sphinx)

endif()

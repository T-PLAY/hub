#
# This cmake module sets the project version and partial version
# variables by analysing the git tag and commit history. It expects git
# tags defined with semantic versioning 2.0.0 (http://semver.org/).
#
# The module expects the PROJECT_NAME variable to be set, and recognizes
# the GIT_FOUND, GIT_EXECUTABLE and VERSION_UPDATE_FROM_GIT variables.
# If Git is found and VERSION_UPDATE_FROM_GIT is set to boolean TRUE,
# the project version will be updated using information fetched from the
# most recent git tag and commit. Otherwise, the module will try to read
# a VERSION file containing the full and partial versions. The module
# will update this file each time the project version is updated.
#
# Once done, this module will define the following variables:
#
# ${PROJECT_NAME}_VERSION_STRING - Version string without metadata
# such as "v2.0.0" or "v1.2.41-beta.1". This should correspond to the
# most recent git tag.
# ${PROJECT_NAME}_VERSION_STRING_FULL - Version string with metadata
# such as "v2.0.0+3.a23fbc" or "v1.3.1-alpha.2+4.9c4fd1"
# ${PROJECT_NAME}_VERSION - Same as ${PROJECT_NAME}_VERSION_STRING,
# without the preceding 'v', e.g. "2.0.0" or "1.2.41-beta.1"
# ${PROJECT_NAME}_VERSION_MAJOR - Major version integer (e.g. 2 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_MINOR - Minor version integer (e.g. 3 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_PATCH - Patch version integer (e.g. 1 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_TWEAK - Tweak version string (e.g. "RC.2" in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_AHEAD - How many commits ahead of last tag (e.g. 21 in v2.3.1-RC.2+21.ef12c8)
# ${PROJECT_NAME}_VERSION_GIT_SHA - The git sha1 of the most recent commit (e.g. the "ef12c8" in v2.3.1-RC.2+21.ef12c8)
#
# This module is public domain, use it as it fits you best.
#
# Author: Nuno Fachada


# Check if git is found...
#if (GIT_FOUND AND VERSION_UPDATE_FROM_GIT)
if (GIT_FOUND)
    execute_process(
#        COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --match "v*"
        COMMAND ${GIT_EXECUTABLE} describe
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_STRING
        RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

set(LATEST_VERSION_MAJOR 1)
set(LATEST_VERSION_MINOR 2)
set(LATEST_VERSION_PATCH 0)
if (NOT GIT_FOUND OR GIT_DESCRIBE_ERROR_CODE)
#        message(STATUS "Version describe ${${PROJECT_NAME}_VERSION_STRING}")
    set(${PROJECT_NAME}_VERSION_MAJOR ${LATEST_VERSION_MAJOR})
    set(${PROJECT_NAME}_VERSION_MINOR ${LATEST_VERSION_MINOR})
    set(${PROJECT_NAME}_VERSION_PATCH ${LATEST_VERSION_PATCH})

    string(TIMESTAMP DATE_EPOCH "%s" UTC)
    set(${PROJECT_NAME}_HASH ${DATE_EPOCH})
else()

#        # Get last tag from git
#        execute_process(COMMAND ${GIT_EXECUTABLE} describe --abbrev=0 --tags
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_STRING
#                OUTPUT_STRIP_TRAILING_WHITESPACE)
#            message(STATUS "Version string ${${PROJECT_NAME}_VERSION_STRING}")

        #How many commits since last tag
        # execute_process(COMMAND ${GIT_EXECUTABLE} rev-list master ${${PROJECT_NAME}_VERSION_STRING}^..HEAD --count
#        execute_process(COMMAND ${GIT_EXECUTABLE} rev-list HEAD ${${PROJECT_NAME}_VERSION_STRING}^..HEAD --count
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_AHEAD
#                OUTPUT_STRIP_TRAILING_WHITESPACE)
#            message(STATUS "version ahead ${${PROJECT_NAME}_VERSION_AHEAD}")

#        # Get current commit SHA from git
#        execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_GIT_SHA
#                OUTPUT_STRIP_TRAILING_WHITESPACE)
#            message(STATUS "version git sha ${${PROJECT_NAME}_VERSION_GIT_SHA}")

#    message(STATUS "Version describe ${${PROJECT_NAME}_VERSION_STRING}")


        # Get partial versions into a list
        string(REGEX MATCHALL "-.*$|[0-9]+" ${PROJECT_NAME}_PARTIAL_VERSION_LIST
                ${${PROJECT_NAME}_VERSION_STRING})

        # Set the version numbers
        list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST
                0 ${PROJECT_NAME}_VERSION_MAJOR)
        list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST
                1 ${PROJECT_NAME}_VERSION_MINOR)
        list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST
                2 ${PROJECT_NAME}_VERSION_PATCH)

        if (NOT ${LATEST_VERSION_MAJOR} EQUAL ${${PROJECT_NAME}_VERSION_MAJOR} OR
        NOT ${LATEST_VERSION_MINOR} EQUAL ${${PROJECT_NAME}_VERSION_MINOR} OR
        NOT ${LATEST_VERSION_PATCH} EQUAL ${${PROJECT_NAME}_VERSION_PATCH}
            )
            message(FATAL_ERROR "You must update the latest version for offline use")
        endif()

#            foreach(MATCH ${${PROJECT_NAME}_PARTIAL_VERSION_LIST})
#                message(STATUS "list: ${MATCH}")
#            endforeach()
#        # The tweak part is optional, so check if the list contains it
#        list(LENGTH ${PROJECT_NAME}_PARTIAL_VERSION_LIST
#                ${PROJECT_NAME}_PARTIAL_VERSION_LIST_LEN)
#        if (${PROJECT_NAME}_PARTIAL_VERSION_LIST_LEN GREATER 3)
#                list(GET ${PROJECT_NAME}_PARTIAL_VERSION_LIST 3 ${PROJECT_NAME}_VERSION_TWEAK)
#                string(SUBSTRING ${${PROJECT_NAME}_VERSION_TWEAK} 1 -1 ${PROJECT_NAME}_VERSION_TWEAK)
#        endif()

        # Unset the list
        unset(${PROJECT_NAME}_PARTIAL_VERSION_LIST)

        ###############################################  COMMIT GIT HASH
        execute_process(
    #        COMMAND ${GIT_EXECUTABLE} describe --tags --dirty --match "v*"
            COMMAND ${GIT_EXECUTABLE} log -1 --format="%H"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE ${PROJECT_NAME}_HASH
            RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

#    message(STATUS "################### ${${PROJECT_NAME}_GIT_HASH}")

        # Set full project version string
#        set(${PROJECT_NAME}_VERSION_STRING_FULL
#                ${${PROJECT_NAME}_VERSION_STRING}+${${PROJECT_NAME}_VERSION_AHEAD}.${${PROJECT_NAME}_VERSION_GIT_SHA})

        # Save version to file (which will be used when Git is not available
        # or VERSION_UPDATE_FROM_GIT is disabled)
        #	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/VERSION ${${PROJECT_NAME}_VERSION_STRING_FULL}
#		"*" ${${PROJECT_NAME}_VERSION_STRING}
#		"*" ${${PROJECT_NAME}_VERSION_MAJOR}
#		"*" ${${PROJECT_NAME}_VERSION_MINOR}
#		"*" ${${PROJECT_NAME}_VERSION_PATCH}
#		"*" ${${PROJECT_NAME}_VERSION_TWEAK}
#		"*" ${${PROJECT_NAME}_VERSION_AHEAD}
#		"*" ${${PROJECT_NAME}_VERSION_GIT_SHA})
    endif()

#else()
#        set(${PROJECT_NAME}_VERSION_MAJOR ${LATEST_VERSION_MAJOR})
#        set(${PROJECT_NAME}_VERSION_MINOR ${LATEST_VERSION_MINOR})
#        set(${PROJECT_NAME}_VERSION_PATCH ${LATEST_VERSION_PATCH})

##        # Git not available, get version from file
##        file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/VERSION ${PROJECT_NAME}_VERSION_LIST)
##        string(REPLACE "*" ";" ${PROJECT_NAME}_VERSION_LIST ${${PROJECT_NAME}_VERSION_LIST})
##        # Set partial versions
##        list(GET ${PROJECT_NAME}_VERSION_LIST 0 ${PROJECT_NAME}_VERSION_STRING_FULL)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 1 ${PROJECT_NAME}_VERSION_STRING)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 2 ${PROJECT_NAME}_VERSION_MAJOR)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 3 ${PROJECT_NAME}_VERSION_MINOR)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 4 ${PROJECT_NAME}_VERSION_PATCH)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 5 ${PROJECT_NAME}_VERSION_TWEAK)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 6 ${PROJECT_NAME}_VERSION_AHEAD)
##        list(GET ${PROJECT_NAME}_VERSION_LIST 7 ${PROJECT_NAME}_VERSION_GIT_SHA)

#endif()


# Set project version (without the preceding 'v')
set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})
#if (${PROJECT_NAME}_VERSION_TWEAK)
#	set(${PROJECT_NAME}_VERSION ${${PROJECT_NAME}_VERSION}-${${PROJECT_NAME}_VERSION_TWEAK})
#endif()

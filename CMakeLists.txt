cmake_minimum_required(VERSION 3.8)

project(testHubAsFetchContent)

# add_subdirectory(hub)

option(BUILD_STATIC "Build static" ON)
option(BUILD_SHARED "Build shared" ON)

set(CMAKE_CXX_STANDARD 17) # using hub required std17

if (NOT DEFINED EXTERNAL_BRANCH_NAME)
    set(EXTERNAL_BRANCH_NAME "main" CACHE STRING "External hub project branch name to install")
endif()

############################### install hub

include(ExternalProject)
set(EXTERNAL_NAME hub)
find_package(${EXTERNAL_NAME} QUIET)
if(NOT ${EXTERNAL_NAME}_FOUND)
    include(FetchContent)
   FetchContent_Declare(
               hub
               GIT_REPOSITORY https://github.com/T-PLAY/hub.git
               GIT_TAG ${EXTERNAL_BRANCH_NAME}
               GIT_PROGRESS TRUE
               GIT_SHALLOW TRUE
       )
   FetchContent_MakeAvailable(hub)

endif()

# if (hub_FOUND)

    file(GLOB sources main.cpp)

    add_executable(${PROJECT_NAME} ${sources})
    target_link_libraries(${PROJECT_NAME} hub)

    if(WIN32)
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub> $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()

# endif()

cmake_minimum_required(VERSION 3.5)

project(examples LANGUAGES CXX)

if(MINGW)
    # set(CMAKE_CXX_STANDARD 14) # filesystem error with c++17
endif()

file(GLOB EXAMPLE_SRC *.cpp)

string(REPLACE ".cpp" "" EXAMPLE_SRC_BASENAME "${EXAMPLE_SRC}")
set(EXAMPLE_SRC_BASENAME ${EXAMPLE_SRC_BASENAME})

foreach(name ${EXAMPLE_SRC_BASENAME})
    get_filename_component(basename ${name} NAME)
    set(TARGET_NAME ${basename})
    add_executable(${TARGET_NAME} ${name}.cpp)
    target_link_libraries(${TARGET_NAME} hub)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD ${CMAKE_CXX_STANDARD})
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_EXTENSIONS OFF)

    if(WIN32)
         add_custom_command(
#	     TARGET ${PROJECT_NAME}
             TARGET ${TARGET_NAME}
             POST_BUILD
             COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:hub>
                     $<TARGET_FILE_DIR:${TARGET_NAME}>)
    endif()
endforeach()



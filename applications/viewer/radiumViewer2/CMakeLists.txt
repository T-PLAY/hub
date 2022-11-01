cmake_minimum_required(VERSION 3.6)
if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} VERSION_GREATER "3.9")
    cmake_policy(SET CMP0071 NEW)
endif()
if(APPLE)
    cmake_policy(SET CMP0042 NEW)
endif(APPLE)

project(radiumViewer2 VERSION 1.0.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(NOT IN_RADIUM_BUILD_TREE)
    find_package(Radium REQUIRED Core Engine Gui IO)
endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(app_sources main.cpp minimalapp.cpp minimalradium.cpp)
set(app_headers minimalapp.hpp minimalradium.hpp)

set(app_uis)
qt_wrap_ui(app_uis_moc ${app_uis})

set(app_resources)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${app_sources} ${app_headers} ${app_uis}
                                             ${app_resources})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME} PRIVATE ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
                            ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${SRC_TARGET} Radium::Core Radium::Engine Radium::Gui
                                             Radium::IO ${Qt_LIBRARIES})

configure_radium_app(NAME ${PROJECT_NAME})

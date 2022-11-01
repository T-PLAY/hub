cmake_minimum_required(VERSION 3.8)

project(radiumViewer)

set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)



find_package(Radium REQUIRED Core Engine Gui)

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


file(GLOB_RECURSE sources *.cpp *.glsl)
file(GLOB_RECURSE headers *.h)

set(app_uis)
qt_wrap_ui(app_uis_moc ${app_uis})

add_executable(${PROJECT_NAME} ${sources} ${headers} ${app_uis})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME} PRIVATE ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
                            ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC ${SRC_TARGET} libApp Radium::Core Radium::Engine
                                             Radium::Gui ${Qt_LIBRARIES})

configure_radium_app(NAME ${PROJECT_NAME})










# elseif(UNIX) #        find_package(Threads REQUIRED) # set(UNIX_REQUIRED_LIBS Threads::Threads)


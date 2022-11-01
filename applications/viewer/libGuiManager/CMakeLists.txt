cmake_minimum_required(VERSION 3.8)

project(libGuiManager)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Radium REQUIRED Core Engine Gui NO_DEFAULT_PATH)

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB_RECURSE sources *.cpp *.glsl)
file(GLOB_RECURSE headers *.h *.hpp *.inl)
file(GLOB_RECURSE app_uis *.ui)

qt_wrap_ui(app_uis_moc ${app_uis})

add_library(${PROJECT_NAME} ${sources} ${headers} ${app_uis})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    # #    ${SRC_DIR} #    PRIVATE # ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
)

set(OTHER_LIBRARIES)

if(HUB_VIEWER_ENABLE_IMAGE_VIEWER)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_IMAGE_VIEWER)
    set(OTHER_LIBRARIES ${OTHER_LIBRARIES} libImageManipulator)
endif()

if(HUB_VIEWER_ENABLE_LOADER)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_LOADER)
    set(OTHER_LIBRARIES ${OTHER_LIBRARIES} libLoader)
endif()

target_link_libraries(
    ${PROJECT_NAME}
    Radium::Core
    Radium::Engine
    Radium::Gui
    ${Qt_LIBRARIES}
    libViewer
    ${OTHER_LIBRARIES})

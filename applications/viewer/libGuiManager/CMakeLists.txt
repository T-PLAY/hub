cmake_minimum_required (VERSION 3.8)

project(libGuiManager)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)


set(CMAKE_AUTOMOC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)

# Disable in-source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Be nice to visual studio
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Be nice and export compile commands by default, this is handy for clang-tidy
# and for other tools.
#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ------------------------------------------------------------------------------
find_package(DCMTK REQUIRED)
#if(NOT IN_RADIUM_BUILD_TREE)
find_package(Radium REQUIRED Core Engine Gui)
#endif()

find_qt_package(COMPONENTS Core Widgets REQUIRED)
set(Qt_LIBRARIES Qt::Core Qt::Widgets)
#set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# ------------------------------------------------------------------------------


if(${QT_VERSION} GREATER_EQUAL 6)
    add_definitions(-DUSE_GOT_CONTRIB)
endif()


#if (WIN32)
#    set(QT_VERSION 5)
#else()
#    set(QT_VERSION 6)
#endif()
#set(QT_LIBS Core Widgets Gui)
#set(QT_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Widgets)
#find_package(Qt${QT_VERSION} COMPONENTS ${QT_LIBS} REQUIRED)


file(GLOB_RECURSE sources *.cpp *.glsl)
file(GLOB_RECURSE headers *.h)

#set(app_uis)
#qt_wrap_ui(app_uis_moc ${app_uis})


add_library(${PROJECT_NAME}
    ${sources}
    ${headers}
    ${app_uis}
    )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${SRC_DIR}
    PRIVATE
    ${RADIUM_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} # Moc
    )


target_link_libraries(
    ${PROJECT_NAME} PUBLIC Radium::Core Radium::Engine Radium::Gui ${Qt_LIBRARIES}
    libViewer
    DicomLoader
    libImageManipulator
#    ${DCMTK_LIBRARIES}
#    DCMTK::dcmdata DCMTK::dcmimage
)

configure_radium_app(NAME ${PROJECT_NAME})


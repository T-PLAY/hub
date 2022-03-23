cmake_minimum_required (VERSION 3.8)

project(radiumViewer)

#set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.1\\5.0.1\\msvc2010\\")
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(Radium REQUIRED Core Engine Gui)

if (WIN32)
    set(QT_VERSION 5)
else()
    set(QT_VERSION 6)
endif()
set(REQUIRED_LIBS Core Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt${QT_VERSION}::Core Qt${QT_VERSION}::Widgets)


find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)


file(GLOB_RECURSE sources *.cpp)
file(GLOB_RECURSE headers *.h)

add_executable(${PROJECT_NAME}
    ${sources}
    ${headers}
    )


target_include_directories(${PROJECT_NAME} PUBLIC
    ${SRC_DIR}
    ${PROJECT_SOURCE_DIR}
    )

if (WIN32)


elseif(UNIX)
    #        find_package(Threads REQUIRED)
    #        set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()


target_link_libraries(${PROJECT_NAME} src Radium::Core Radium::Engine Radium::Gui ${REQUIRED_LIBS_QUALIFIED} ${UNIX_REQUIRED_LIBS} ${WIN32_REQUIRED_LIBS})


cmake_minimum_required (VERSION 3.8)

project ("HubV2")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Network Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets)
set(WIN32_REQUIRED_LIBS)
set(UNIX_REQUIRED_LIBS)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)

if (WIN32)
    set(WIN32_REQUIRED_LIBS wsock32 ws2_32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB ${REQUIRED_LIBS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt${QT_VERSION}${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
elseif(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set(UNIX_REQUIRED_LIBS Threads::Threads)
endif()

add_executable(${PROJECT_NAME} main.cpp
        #UI
        ui/hub/hub.ui ui/hub/hub.h ui/hub/hub.cpp
        ui/diapo/diapowindow.ui ui/diapo/diapowindow.h ui/diapo/diapowindow.cpp
        ui/select/selectwindow.ui ui/select/selectwindow.h ui/select/selectwindow.cpp
        ui/stream/streamwindow.ui ui/stream/streamwindow.h ui/stream/streamwindow.cpp

        #SRC
        #BRIDGE
            #SENDER
            src/bridge/sender/FileSender.h src/bridge/sender/FileSender.cpp
            src/bridge/sender/UDPSender.h src/bridge/sender/UDPSender.cpp
            src/bridge/sender/Sender.h

            #TRACKER
            src/bridge/tracker/BasicTracker.h src/bridge/tracker/BasicTracker.cpp
            src/bridge/tracker/FileTracker.h src/bridge/tracker/FileTracker.cpp
            src/bridge/tracker/USBTracker.h src/bridge/tracker/USBTracker.cpp

        src/bridge/Bridge.h src/bridge/Bridge.cpp
        src/bridge/synchro.h src/bridge/synchro.cpp
        src/bridge/utils.h src/bridge/utils.cpp

        #HUB
        src/hub/modesender.h src/hub/modesender.cpp

        #IMAGE LOAD
        src/imLoad/uploader.h src/imLoad/uploader.cpp

        #IMAGE STREAM
        src/imStream/ImageAcquisition.h
        src/imStream/FileStreamer.h src/imStream/FileStreamer.cpp
        src/imStream/ImageStream.h src/imStream/ImageStream.cpp

        )


target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${WIN32_REQUIRED_LIBS} ${UNIX_REQUIRED_LIBS})

/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#include <functional>

#include "core/Any.hpp"
#include "io/Header.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#    include "sensor/Acquisition.hpp"
#    include "sensor/SensorSpec.hpp"
#endif

namespace hub {
namespace client {

///
/// \brief The ViewerHandler class
/// is a facade, able the viewer client to catch low level (close to sensor) event from sensor
///
class SRC_API ViewerHandler
{
  public:
    ///
    /// \brief Catched when server is not found (sended after connection to the server failed)
    /// \param ipv4 [in] correspond to the ip the viewer has failed to connect to the server
    /// \param port [in] correspond to the port the viewer has failed to connect to the server
    ///
    std::function<void( const char* ipv4, int port )> onServerNotFound;

    ///
    /// \brief Catched when server founded (sended after connection to the server)
    /// \param ipv4 [in] correspond to the ip the viewer has been connecting to the server
    /// \param port [in] correspond to the port the viewer has been connecting to the server
    ///
    std::function<void( const char* ipv4, int port )> onServerConnected;

    ///
    /// \brief Catched when server is disconnected (loose connection from the server)
    /// \param ipv4 [in] correspond to the ip the viewer has lost from the server
    /// \param port [in] correspond to the port the viewer has lost from the server
    ///
    std::function<void( const char* ipv4, int port )> onServerDisconnected;

    ///
    /// \brief Catched when new stream was connected to the server (after connection of OutputStream to the server)
    /// Todo move header to input/output class avoid the use of data stream (change to deprecated)
    /// \param streamName [in] is the new connected stream available from server
    /// \param header [in] is the header of new connected stream available from server
    ///
    std::function<bool( const char* streamName, const io::Header& header )> onNewStream;

#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief Catched when new sensor was connected to the server (after connection of OutputSensor to the server)
    /// \param streamName [in] is the name of new connected sensor to the server
    /// \param header [in] is sensor spec of new connected sensor to the server
    ///
    std::function<bool( const char* streamName, const sensor::SensorSpec& sensorSpec )> onNewSensor;
#endif

    ///
    /// \brief Catched when new data comming from stream
    /// \param streamName [in] is the name of stream data is comming
    /// \param datas [in] represent comming data from active stream
    /// \warning data comming from stream imply the stream is available from server (onNewStream called)
    /// Todo move header to input/output classes avoid the use of data stream (change to deprecated)
    ///
    std::function<void( const char* streamName, const Datas_t& datas )> onNewData;

#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief Catched when new acquisition comming from sensor to server
    /// \param streamName [in] is the name of stream comming from connected sensor to server
    /// \param acq represents [in] latest acquisition sended by streamer application to server
    ///
    std::function<void( const char* streamName, const sensor::Acquisition& acq )> onNewAcq;
#endif

    ///
    /// \brief Catched when stream or sensor are disconnected
    /// \param streamName [in] is the name of stream which has been disconnected (unavailable from server)
    /// Todo change to onDelSensor when header moved to input/output classes
    ///
    std::function<void( const char* streamName )> onDelStream;

    ///
    /// \brief Catched when viewer want to change render properties of data/sensor stream
    /// (dispatched for all other viewer of the same data)
    /// \param streamName [in] is the stream impacted by the change
    /// \param objectName [in] is the object in stream impacted by the change
    /// \param property [in] is the property impacted by the change
    /// \param value [in] is the render option is about to change
    ///
    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        onSetProperty;

    ///
    /// \brief Catched when server send log message (dispatched for all viewer)
    /// Message log from server are sended to the viewer allow to debug from client side.
    /// Helpfull for Unity3D application for example.
    /// \param logMessage [in] is the message comming from server for debugging
    ///
    std::function<void( const char* logMessage )> onLogMessage;
};

} // namespace client
} // namespace hub

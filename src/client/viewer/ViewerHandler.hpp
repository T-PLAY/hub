/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/30

#pragma once

#include <functional>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

#include "core/Any.hpp"
#include "io/input/InputStream.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#    include "sensor/Acquisition.hpp"
#    include "sensor/SensorSpec.hpp"
#endif

namespace hub {
namespace client {

///
/// \brief The ViewerHandler class
///
class SRC_API ViewerHandler
{
  public:
    ///
    /// \brief onServerNotFound
    ///
    std::function<void( const char* ipv4, int port )> onServerNotFound;

    ///
    /// \brief onServerConnected
    ///
    std::function<void( const char* ipv4, int port )> onServerConnected;

    ///
    /// \brief onServerDisconnected
    ///
    std::function<void( const char* ipv4, int port )> onServerDisconnected;

    ///
    /// \brief onNewStream
    ///
    std::function<bool( const char* streamName, const io::Header& header )> onNewStream;
#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief onNewSensor
    ///
    std::function<bool( const char* streamName, const sensor::SensorSpec& sensorSpec )> onNewSensor;
#endif
    ///
    /// \brief onNewData
    ///
    std::function<void( const char* streamName, const Datas_t& datas )> onNewData;
#ifndef HUB_NON_BUILD_SENSOR
    ///
    /// \brief onNewAcq
    ///
    std::function<void( const char* streamName, const sensor::Acquisition& acq )> onNewAcq;
#endif

    ///
    /// \brief onDelStream
    ///
    std::function<void( const char* streamName )> onDelStream;

    ///
    /// \brief onSetProperty
    ///
    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        onSetProperty;

    ///
    /// \brief onLogMessage
    ///
    std::function<void( const char* logMessage )> onLogMessage;
};

} // namespace client
} // namespace hub

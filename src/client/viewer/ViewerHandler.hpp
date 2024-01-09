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
// #include "io/output/OutputStream.hpp"

#ifndef HUB_NON_BUILD_SENSOR
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"
#endif

namespace hub {
namespace client {

class ViewerHandler
{
  public:
    std::function<void( const char* ipv4, int port )> onServerNotFound;
    std::function<void( const char* ipv4, int port )> onServerConnected;
    std::function<void( const char* ipv4, int port )> onServerDisconnected;

    std::function<bool( const char* streamName, const io::Header & header )>
        onNewStream;
#ifndef HUB_NON_BUILD_SENSOR
    std::function<bool( const char* streamName, const sensor::SensorSpec& sensorSpec )>
        onNewSensor;
#endif
    // std::function<void( const char* streamName, input::InputStream & inputStream )>
        // onInitStream;
    // std::function<void( const char* streamName, input::InputStream & inputStream )> onNewData;
    std::function<void( const char* streamName, const Datas_t & datas )> onNewData;
#ifndef HUB_NON_BUILD_SENSOR
    std::function<void( const char* streamName, const sensor::Acquisition& acq )> onNewAcq;
#endif

    std::function<void( const char* streamName )>
        onDelStream;
    // std::function<void( const char* streamName, const sensor::SensorSpec& sensorSpec )>
        // onDelStream;

    std::function<
        void( const char* streamName, const char* objectName, int property, const Any& value )>
        onSetProperty;

    std::function<void( const char* logMessage )> onLogMessage;
};

} // namespace client
} // namespace hub

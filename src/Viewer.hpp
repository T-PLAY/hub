#pragma once

#include <functional>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "Configurations.hpp"
#include "InputSensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

///
/// \brief The Viewer class
/// can be use to be aware of streamer connected to the server,
/// for example you can open input sensor in your application
/// when new sensor is connected to server to have an interactive application.
///
class SRC_API Viewer
{
  public:
    Viewer( std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
            std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
            std::function<void( const char* ipv4, int port )> onServerConnected    = {},
            std::function<void( const char* ipv4, int port )> onServerDisconnected = {},
            std::function<void( const char* streamName, const hub::Acquisition& acq )>
                onNewAcquisition    = {},
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port                = net::s_defaultServicePort );

    ~Viewer();

    void setIpv4( const std::string& ipv4 );
    void setPort( int port );
    void startStream( const std::string& streamName, const SensorSpec& );
    void stopStream( const std::string& streamName, const SensorSpec& );

  private:
    std::thread m_thread;
    bool m_stopThread = false;

    std::function<bool( const char* streamName, const SensorSpec& )> m_onNewStreamer;
    std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void( const char* ipv4, int port )> m_onServerConnected;
    std::function<void( const char* ipv4, int port )> m_onServerDisconnected;
    std::function<void( const char* streamName, const hub::Acquisition& acq )> m_onNewAcquisition;

    std::string m_ipv4;
    int m_port;
    bool m_serverConnected = false;
    std::regex m_ipv4Regex;

    //    std::map<std::string, std::unique_ptr<InputSensor>> m_streamName2inputSensor;
    std::map<std::string, SensorSpec> m_streamName2sensorSpec;
    std::map<std::string, std::thread> m_streamName2thread;
    std::map<std::string, bool> m_streamName2stopThread;
};

} // namespace hub

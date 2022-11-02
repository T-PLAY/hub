#pragma once

#include <functional>
#include <regex>
#include <string>
#include <thread>

#include "Acquisition.hpp"
#include "Configurations.hpp"
#include "SensorSpec.hpp"

namespace hub {

///
/// \brief The Viewer class (event dispatcher)
/// can be use by client application to be aware of the events comming from server
/// client (user of this class) connection/deconnection to the server,
/// streamer connected/disconnected to the server,
/// new acquisition sended from a connected streamer.
/// Example of use : open input stream in your application (client side)
/// when new streamer is connected to server to have an interactive application.
/// Close the input stream when server or streamer are disconnected.
///
class SRC_API Viewer
{
  public:
    Viewer( std::function<bool( const char* streamName, const SensorSpec& )> onNewStreamer,
            std::function<void( const char* streamName, const SensorSpec& )> onDelStreamer,
            std::function<void( const char* ipv4, int port )> onServerConnected    = {},
            std::function<void( const char* ipv4, int port )> onServerDisconnected = {},
            std::function<void( const char* streamName, const hub::Acquisition& )>
                onNewAcquisition    = {},
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port                = net::s_defaultServicePort );

    ~Viewer();

    void setIpv4( const std::string& ipv4 );
    void setPort( int port );

  private:
    std::thread m_thread;
    bool m_stopThread = false;

    std::function<bool( const char* streamName, const SensorSpec& )> m_onNewStreamer;
    std::function<void( const char* streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void( const char* ipv4, int port )> m_onServerConnected;
    std::function<void( const char* ipv4, int port )> m_onServerDisconnected;
    std::function<void( const char* streamName, const hub::Acquisition& )> m_onNewAcquisition;

    std::string m_ipv4;
    int m_port;
    bool m_serverConnected = false;
    std::regex m_ipv4Regex;

    std::map<std::string, SensorSpec> m_streamName2sensorSpec;
    std::map<std::string, std::thread> m_streamName2thread;
    std::map<std::string, bool> m_streamName2stopThread;

  private:
    void startStream( const std::string& streamName, const SensorSpec& );
    void stopStream( const std::string& streamName, const SensorSpec& );
};

} // namespace hub

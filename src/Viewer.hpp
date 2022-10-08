#pragma once

#include <functional>
#include <string>
#include <thread>
#include <regex>

#include "SensorSpec.hpp"
#include "Configurations.hpp"

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
    Viewer(
            std::function<void( const std::string& streamName, const SensorSpec& )> onNewStreamer,
            std::function<void( const std::string& streamName, const SensorSpec& )> onDelStreamer,
            std::function<void(const std::string& ipv4, int port)> onServerConnected    = {},
            std::function<void(const std::string& ipv4, int port)> onServerDisconnected = {},
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port = net::s_defaultServicePort);
    ~Viewer();

    void setIpv4(const std::string & ipv4);
    void setPort(int port);

  private:
    std::thread m_thread;
    bool m_stopThread = false;

    std::function<void( const std::string& streamName, const SensorSpec& )> m_onNewStreamer;
    std::function<void( const std::string& streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void(const std::string& ipv4, int port)> m_onServerConnected;
    std::function<void(const std::string& ipv4, int port)> m_onServerDisconnected;

    std::string m_ipv4;
    int m_port;
    bool m_serverConnected = false;
    std::regex m_ipv4Regex;
};

} // namespace hub

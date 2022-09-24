#pragma once

//#include "Net/ClientSocket.hpp"
#include <functional>
#include <string>
#include <thread>

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
    //    Viewer( hub::net::ClientSocket&& clientSocket,
    //            std::function<void( const std::string&, const SensorSpec& )> onNewStreamer,
    //            std::function<void( const std::string& )> onDelStreamer );
    Viewer(
            std::function<void( const std::string& streamName, const SensorSpec& )> onNewStreamer,
            std::function<void( const std::string& streamName, const SensorSpec& )> onDelStreamer,
            std::function<void()> onServerConnected    = {},
            std::function<void()> onServerDisconnected = {},
            const std::string& ipv4 = net::s_defaultServiceIp,
            int port = net::s_defaultServicePort);
    ~Viewer();

  private:
    std::thread m_thread;
    bool m_stopThread = false;

    std::function<void( const std::string& streamName, const SensorSpec& )> m_onNewStreamer;
    std::function<void( const std::string& streamName, const SensorSpec& )> m_onDelStreamer;
    std::function<void()> m_onServerConnected;
    std::function<void()> m_onServerDisconnected;

    const std::string m_ipv4;
    const int m_port;
    bool m_serverConnected = false;
};

} // namespace hub

#pragma once

#include "Net/ClientSocket.hpp"

namespace hub {

///
/// \brief The Viewer class
/// can be use to be aware of streamer connected to the server,
/// for example you can open input sensor in your application
/// when new sensor is connected to server to have an interactive application.
///
class Viewer : public net::ClientSocket
{
  public:
    Viewer( ClientSocket&& clientSocket,
            std::function<void( const std::string&, const SensorSpec& )> onNewStreamer,
            std::function<void( const std::string& )> onDelStreamer );
    ~Viewer();

  private:
    std::thread m_thread;
    bool m_stopThread = false;
    std::function<void( const std::string& sensorName, const SensorSpec& sensorSpec )>
        m_onNewStreamer;
    std::function<void( const std::string& sensorName )> m_onDelStreamer;
};

} // namespace hub

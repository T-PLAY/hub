#pragma once

#include "Net/ClientSocket.hpp"

namespace hub {

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

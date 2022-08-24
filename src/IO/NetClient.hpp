#pragma once

#include <mutex>

#include "Interface.hpp"
#include "Net/ClientSocket.hpp"

namespace hub {
namespace io {

///////////////////////// STREAMER ///////////////////////////////

class Streamer : public OutputInterface, public net::ClientSocket
{
  public:
    Streamer( const std::string& sensorName,
              net::ClientSocket&& clientSocket = net::ClientSocket() );
    Streamer( net::ClientSocket&& clientSocket ); // server side
};

///////////////////////// STREAM_VIEWER ///////////////////////////////

class StreamViewer : public InputInterface, public net::ClientSocket
{
  public:
    StreamViewer( const std::string& sensorName,
                  const std::string& syncSensorName = "",
                  net::ClientSocket&& clientSocket  = net::ClientSocket() );
    StreamViewer( net::ClientSocket&& clientSocket ); // server side
};

///////////////////////// VIEWER ///////////////////////////////

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

} // namespace io
} // namespace hub

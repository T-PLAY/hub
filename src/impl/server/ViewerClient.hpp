
#pragma once

#include <mutex>
#include <thread>

#include "Client.hpp"
//#include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace server {

class StreamerClient;

///
/// \brief The ViewerClient class
///
class ViewerClient : public Client
{
  private:
    ViewerClient( Server* server, int iClient, net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

    void notifyNewStreamer( const std::string& streamName, const sensor::SensorSpec& sensorSpec );
    void notifyDelStreamer( const std::string& streamName, const sensor::SensorSpec& sensorSpec );

    void end( io::StreamInterface::ServerMessage message ) override;
    void notifyProperty( const std::string& streamName,
                         const std::string& objectName,
                         int property,
                         const Any& value );

  private:
    std::thread m_thread;

//    net::ClientSocket m_socket;
    io::InputOutputSocket m_socket;

    bool m_viewerClosed = false;

    friend class hub::Server;
};

} // namespace server
} // namespace hub

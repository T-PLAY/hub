
#pragma once

#include <mutex>
#include <thread>

#include "Client.hpp"
// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace impl2 {
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

    //    void notifyNewStreamer( const std::string& streamName, const sensor::SensorSpec&
    //    sensorSpec ); void notifyDelStreamer( const std::string& streamName, const
    //    sensor::SensorSpec& sensorSpec );
    void notifyNewStreamer( const std::string& streamName );
    void notifyDelStreamer( const std::string& streamName );

    void end( hub::io::StreamInterface::ServerMessage message ) override;
    //    void notifyProperty( const std::string& streamName,
    //                         const std::string& objectName,
    //                         int property,
    //                         const Any& value );

  private:
    std::thread m_thread;

    //    net::ClientSocket m_socket;
    hub::io::InputOutputSocket m_socket;

    bool m_viewerClosed = false;

    friend class hub::impl2::Server;
};

} // namespace server
} // namespace impl2
} // namespace hub

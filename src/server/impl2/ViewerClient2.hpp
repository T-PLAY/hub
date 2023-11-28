
#pragma once

#include <mutex>
#include <thread>

#include "Client2.hpp"
// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
//#include "sensor/SensorSpec.hpp"

namespace hub {
namespace server {
namespace impl2 {

class StreamerClient2;

///
/// \brief The ViewerClient2 class
///
class ViewerClient2 : public Client2
{
  private:
    ViewerClient2( ServerImpl2* server, int iClient, net::ClientSocket&& sock );
    ~ViewerClient2();

    std::string headerMsg() const override;

    //    void notifyNewStreamer( const std::string& streamName, const sensor::SensorSpec&
    //    sensorSpec ); void notifyDelStreamer( const std::string& streamName, const
    //    sensor::SensorSpec& sensorSpec );
    void notifyNewStreamer( const std::string& streamName );
    void notifyDelStreamer( const std::string& streamName );

    void end( io::StreamBase::ServerMessage message ) override;
    //    void notifyProperty( const std::string& streamName,
    //                         const std::string& objectName,
    //                         int property,
    //                         const Any& value );

  private:
    std::thread m_thread;

    //    net::ClientSocket m_socket;
    hub::io::InputOutputSocket m_socket;

    bool m_viewerClosed = false;

    friend class ServerImpl2;
};

} // namespace impl2
} // namespace server
} // namespace hub

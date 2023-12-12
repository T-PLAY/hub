
#pragma once

#include <mutex>
#include <thread>

#include "Client2.hpp"
// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"
//#include "sensor/SensorSpec.hpp"

namespace hub {
namespace server {

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

    void notifyNewStreamer( const StreamerClient2 * streamer );
    void notifyDelStreamer( const std::string& streamName );

    void end( io::StreamBase::ServerMessage message ) override;
    void notifyInited() override;

  private:
    std::thread m_thread;

    hub::io::InputOutputSocket m_socket;

    bool m_viewerClosed = false;
    std::atomic<bool> m_clientStreamAdded = false;

    friend class ServerImpl2;
};

} // namespace server
} // namespace hub

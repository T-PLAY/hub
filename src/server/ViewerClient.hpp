
#pragma once

#include <mutex>
#include <thread>

#include <net/ClientSocket.hpp>

#include "Client.hpp"

namespace hub {
namespace server {

class StreamerClient;

///
/// \brief The ViewerClient class
///
class ViewerClient : public Client
{
    //  public:
    //  protected:
  private:
    ViewerClient( Server* server, int iClient, hub::net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

    void notifyNewStreamer( const std::string& streamName, const SensorSpec& sensorSpec );
    void notifyDelStreamer( const std::string& streamName, const SensorSpec& sensorSpec );
    //    void notifyDelStreamer( const StreamerClient& streamer );

    void end( net::ClientSocket::Message message ) override;
    void notifyProperty( const std::string& streamName,
                         const std::string& objectName,
                         int property,
                         const Any& value );

  private:
    std::thread m_thread;

    hub::net::ClientSocket m_socket;
    //    std::mutex m_mtxSocket;

    bool m_viewerClosed = false;

    friend class Server;
};

} // namespace server
} // namespace hub

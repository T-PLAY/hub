
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

    void notifyNewStreamer( const StreamerClient& streamer );
    void notifyDelStreamer( const StreamerClient& streamer );

  private:
    std::thread m_thread;

    hub::net::ClientSocket m_socket;
    std::mutex m_mtxSocket;

    friend class Server;
};


} // server
} // hub

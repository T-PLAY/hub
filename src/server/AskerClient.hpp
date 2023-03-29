
#pragma once

#include <mutex>
#include <thread>

#include <net/ClientSocket.hpp>

#include "Client.hpp"

namespace hub {
namespace server {


///
/// \brief The AskerClient class
///
class AskerClient : public Client
{
//  public:
//  protected:
private:
    AskerClient( Server* server, int iClient, hub::net::ClientSocket&& sock );
    ~AskerClient();

    std::string headerMsg() const override;

    void end(net::ClientSocket::Message message) override;

  private:
    std::thread m_thread;
    hub::net::ClientSocket m_sock;

    friend class Server;
};

} // server
} // hub

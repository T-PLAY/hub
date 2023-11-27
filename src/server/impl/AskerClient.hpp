
#pragma once

#include <mutex>
#include <thread>

// #include "net/ClientSocket.hpp"
#include "io/InputOutputSocket.hpp"

#include "Client.hpp"

namespace hub {
namespace server {
namespace impl {

///
/// \brief The AskerClient class
///
class AskerClient : public Client
{
  private:
    AskerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock );
    ~AskerClient();

    std::string headerMsg() const override;

    void end( io::StreamBase::ServerMessage message ) override;

  private:
    std::thread m_thread;
    //    net::ClientSocket m_sock;
    io::InputOutputSocket m_sock;

    friend class ServerImpl;
};

} // namespace impl
} // namespace server
} // namespace hub

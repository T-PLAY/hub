/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#ifdef HUB_USE_SERVER

#    include <mutex>
#    include <thread>

#    include "io/InputOutputSocket.hpp"

#    include "Client.hpp"

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
    io::InputOutputSocket m_sock;

    friend class ServerImpl;
};

} // namespace impl
} // namespace server
} // namespace hub

#endif

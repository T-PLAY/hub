/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/27

#pragma once

#include <atomic>
#include <mutex>
#include <thread>

#include "Client.hpp"
#include "io/InputOutputSocket.hpp"

namespace hub {
namespace server {

class StreamerClient;

///
/// \brief The ViewerClient class
/// represents Viewer peer from server side
///
class SRC_API ViewerClient : public Client
{
  private:
    ViewerClient( ServerImpl* server, int iClient, net::ClientSocket&& sock );
    ~ViewerClient();

    std::string headerMsg() const override;

    void notifyNewStreamer( const StreamerClient* streamer );
    void notifyDelStreamer( const std::string& streamName );

    void end( io::StreamBase::ServerMessage message ) override;
    void notifyInited() override;

  private:
    std::thread m_thread;

    hub::io::InputOutputSocket m_socket;

    bool m_viewerClosed                   = false;
    std::atomic<bool> m_clientStreamAdded = false;

    friend class ServerImpl;
};

} // namespace server
} // namespace hub

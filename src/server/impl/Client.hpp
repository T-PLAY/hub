#pragma once

#include <mutex>
#include <string>

// #include "net/ClientSocket.hpp"
#include "io/StreamBase.hpp"

namespace hub {
namespace server {
namespace impl {

class ServerImpl;

///
/// \brief The Client class
///
class Client
{
  private:
    Client( ServerImpl* server, int iClient );
    virtual ~Client();

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

    void setServer( ServerImpl* newServer );

    virtual void end( io::StreamBase::ServerMessage message ) = 0;

  private:
    ServerImpl* m_server = nullptr;
    int m_iClient;

    friend class ServerImpl;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};

} // namespace impl
} // namespace server
} // namespace hub

#pragma once

#include <mutex>
#include <string>

// #include "net/ClientSocket.hpp"
#include "io/StreamBase.hpp"

namespace hub {
namespace server {
namespace impl2 {

class ServerImpl2;

///
/// \brief The Client2 class
///
class Client2
{
  private:
    Client2( ServerImpl2* server, int iClient );
    virtual ~Client2();

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

    void setServer( ServerImpl2* newServer );

    virtual void end( hub::io::StreamBase::ServerMessage message ) = 0;

  private:
    ServerImpl2* m_server = nullptr;
    int m_iClient;

    friend class ServerImpl2;
    friend class StreamerClient2;
    friend class StreamViewerClient2;
    friend class ViewerClient2;
    friend class AskerClient2;
};

} // namespace impl2
} // namespace server
} // namespace hub

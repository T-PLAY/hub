#pragma once

#include <mutex>
#include <string>

//#include "net/ClientSocket.hpp"
#include "io/StreamInterface.hpp"

namespace hub {

class Server;

namespace server {


///
/// \brief The Client class
///
class Client
{
  private:
    Client( Server* server, int iClient );
    virtual ~Client();

    virtual std::string headerMsg() const;
    void printStatusMessage( const std::string& message ) const;

    void setServer( Server* newServer );

    virtual void end( io::StreamInterface::ServerMessage message ) = 0;

  private:
    Server* m_server = nullptr;
    int m_iClient;

    friend class hub::Server;
    friend class StreamerClient;
    friend class StreamViewerClient;
    friend class ViewerClient;
    friend class AskerClient;
};

} // namespace server
} // namespace hub

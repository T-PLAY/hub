#pragma once

#include "net/ServerSocketI.hpp"

#include "ClientSocketSystem.hpp"
#include "SocketSystem.hpp"

namespace hub {
namespace net {
namespace system {

///
/// \brief The ServerSocketSystem class
/// enable the server application to listen client sockets (streamer, viewer).
///
class SRC_API ServerSocketSystem : public ServerSocketI, public SocketSystem
{
  public:
    ///
    /// \brief ServerSocketSystem
    /// create server socket with the default service port.
    ///
//    ServerSocketSystem();

    ///
    /// \brief ServerSocketSystem
    /// create server socket with specific service port.
    /// \param port
    /// you want to open to listen.
    ///
    explicit ServerSocketSystem( int port );

    ServerSocketSystem( const ServerSocketSystem& sock ) = delete;
    ServerSocketSystem( ServerSocketSystem&& sock ) = delete;
    ServerSocketSystem& operator=( const ServerSocketSystem& sock ) = delete;
    ServerSocketSystem&& operator=( ServerSocketSystem&& sock ) = delete;
    // ~ServerSocketSystem();

    ///
    /// \brief waitNewClient
    /// wait client connection.
    /// \return
    /// return client socket to communication with.
    ///
    ClientSocketSystem waitNewClient();

//    void disconnect();


  private:
    void initServer();

  private:
    int mPort;
    utils::ServerAddr m_addr;

  public:
    ///
    /// \brief getPort
    /// of oppened running server.
    /// \return
    /// service port.
    ///
    int getPort() const override;
};

} // namespace system
} // namespace net
} // namespace hub

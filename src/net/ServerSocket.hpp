#pragma once

#include <net/ClientSocket.hpp>
#include <net/Socket.hpp>

namespace hub {
namespace net {

///
/// \brief The ServerSocket class
/// enable the server application to listen client sockets (streamer, viewer).
///
class SRC_API ServerSocket : public Socket
{
  public:
    ///
    /// \brief ServerSocket
    /// create server socket with the default service port.
    ///
//    ServerSocket();

    ///
    /// \brief ServerSocket
    /// create server socket with specific service port.
    /// \param port
    /// you want to open to listen.
    ///
    explicit ServerSocket( int port );

    ~ServerSocket();

    ///
    /// \brief waitNewClient
    /// wait client connection.
    /// \return
    /// return client socket to communication with.
    ///
    ClientSocket waitNewClient();


  private:
    void initServer();

  private:
    int mPort;
    net::utils::ServerAddr m_addr;

  public:
    ///
    /// \brief getPort
    /// of oppened running server.
    /// \return
    /// service port.
    ///
    int getPort() const;
};

} // namespace net
} // namespace hub

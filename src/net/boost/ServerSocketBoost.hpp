#pragma once

#include "net/ServerSocketI.hpp"

#include "ClientSocketBoost.hpp"
//#include "SocketSystem.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

//#define HUB_DEBUG_SOCKET

namespace hub {
namespace net {
namespace boost {

//#ifdef HUB_DEBUG_SOCKET
//std::string SocketSystem::getHeader(utils::socket_fd iSock ) const {
//    std::string str;
//    if ( m_serverSide ) { str += "\t\033[1m[Server]\033[0m"; }
//    else {
//        str += "[Client]";
//    }
//    str += "\033[" + std::to_string( 31 + iSock % 7 ) + "m[socket:" + std::to_string( iSock ) +
//           "]\033[0m ";
//    return str;
//}
//#endif


///
/// \brief The ServerSocketBoost class
/// enable the server application to listen client sockets (streamer, viewer).
///
//class SRC_API ServerSocketBoost : public ServerSocketI, public SocketSystem
class SRC_API ServerSocketBoost : public ServerSocketI
{
  public:
    ///
    /// \brief ServerSocketBoost
    /// create server socket with the default service port.
    ///
//    ServerSocketBoost();

    ///
    /// \brief ServerSocketBoost
    /// create server socket with specific service port.
    /// \param port
    /// you want to open to listen.
    ///
    explicit ServerSocketBoost( int port );

    ~ServerSocketBoost();

    ///
    /// \brief waitNewClient
    /// wait client connection.
    /// \return
    /// return client socket to communication with.
    ///
    ClientSocketBoost waitNewClient();


  private:
    void initServer();

  private:
    int m_port;
//    utils::ServerAddr m_addr;

    ::boost::asio::io_service m_ioService;
    ::boost::asio::ip::tcp::acceptor m_acceptor;

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

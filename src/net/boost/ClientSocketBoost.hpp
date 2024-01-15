/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/18
	
#pragma once

#ifdef HUB_USE_BOOST

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "net/ClientSocketI.hpp"


namespace hub {
namespace net {
namespace boost {

///
/// \brief The ClientSocketBoost class
/// allows connection to remote server hubs.
/// This class describes the functionality needed to open a remote communication.
///
//class SRC_API ClientSocketBoost : public Socket, public io::InputOutput
//class SRC_API ClientSocketBoost : public ClientSocketI, public SocketSystem
class SRC_API ClientSocketBoost : public ClientSocketI
{
  public:
//    ClientSocketBoost();

    ///
    /// \brief ClientSocketBoost
    /// is used to open a remote communication
    /// \param ipv4
    /// [in] string corresponds to the server ip (ex: "127.0.0.1" for local server)
    /// \param port
    /// [in] opened service of the running server
    /// \param autoConnect
    /// [in] connect to the server
    ///
    ClientSocketBoost( const std::string& ipv4, int port, bool autoConnect = true );

    ///
    /// \brief ClientSocketBoost
    /// \param fdSock
    ///
//    explicit ClientSocketBoost( utils::socket_fd fdSock ); // server side client (bind and listen)
    explicit ClientSocketBoost( ::boost::asio::ip::tcp::socket && fdSock ); // server side client (bind and listen)

    ClientSocketBoost( const ClientSocketBoost& sock ) = delete;

    ///
    /// \brief ClientSocketBoost
    /// \param sock
    ///
    ClientSocketBoost( ClientSocketBoost&& sock );

    ClientSocketBoost& operator=( const ClientSocketBoost& sock ) = delete;
    ClientSocketBoost&& operator=( ClientSocketBoost&& sock ) = delete;

    ~ClientSocketBoost();

  public:
    ///
    /// \brief setIpv4
    /// \param newIpv4
    ///
    void setIpv4( const std::string& newIpv4 ) override;

    ///
    /// \brief getIpv4
    /// \return
    ///
    const std::string & getIpv4() const override;

    ///
    /// \brief setPort
    /// \param newPort
    ///
    void setPort( int newPort ) override;

    ///
    /// \brief getPort
    /// \return
    ///
    int getPort() const override;

    ///
    /// \brief connect
    ///
    void connect() override;

    ///
    /// \brief isOpen
    /// \return
    ///
//    bool isOpen() const override;
//    bool isOpen() const;
    bool isConnected() const override;

    ///
    /// \brief close
    ///
//    void close() override;
//    void close();
    void disconnect() override;


    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
//    void write( const unsigned char* data, size_t len ) override;
    void write(const unsigned char * data, size_t size ) override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
//    void read( unsigned char* data, size_t len ) override;
    void read( unsigned char* data, size_t size ) override;

  public:

    ///
    /// \brief isEnd
    /// \return
    ///
//    bool isEnd() const override;

  private:
    ///
    /// \brief initServerAddress
    ///
    void initServerAddress();
//    bool isOpen() const;
//    void close();

  private:
    std::string m_ipv4;
    int m_port;

    ::boost::asio::io_service m_ioService;

    ::boost::asio::ip::tcp::socket m_sock;

//    utils::ClientAddr m_addr;
//    bool m_connected = false;
    bool m_moved     = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace system
} // namespace net
} // namespace hub


#endif

#pragma once

#include <mutex>

#include "net/ClientSocketI.hpp"

//#include "io/InputOutput.hpp"
#include "SocketSystem.hpp"


namespace hub {
namespace net {
namespace system {

///
/// \brief The ClientSocketSystem class
/// allows connection to remote server hubs.
/// This class describes the functionality needed to open a remote communication.
///
//class SRC_API ClientSocketSystem : public Socket, public io::InputOutput
class SRC_API ClientSocketSystem : public ClientSocketI, public SocketSystem
{
  public:
//    ClientSocketSystem();

    ///
    /// \brief ClientSocketSystem
    /// is used to open a remote communication
    /// \param ipv4
    /// [in] string corresponds to the server ip (ex: "127.0.0.1" for local server)
    /// \param port
    /// [in] opened service of the running server
    /// \param autoConnect
    /// [in] connect to the server
    ///
    ClientSocketSystem( const std::string& ipv4, int port, bool autoConnect = true );

    ///
    /// \brief ClientSocketSystem
    /// \param fdSock
    ///
    explicit ClientSocketSystem( utils::socket_fd fdSock ); // server side client (bind and listen)

    ClientSocketSystem( const ClientSocketSystem& sock ) = delete;

    ///
    /// \brief ClientSocketSystem
    /// \param sock
    ///
    ClientSocketSystem( ClientSocketSystem&& sock );

    ClientSocketSystem& operator=( const ClientSocketSystem& sock ) = delete;
    ClientSocketSystem&& operator=( ClientSocketSystem&& sock ) = delete;

    ~ClientSocketSystem();

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
    const std::string& getIpv4() const override;

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
    void write(const unsigned char * data, size_t len ) override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
//    void read( unsigned char* data, size_t len ) override;
    void read( unsigned char* data, size_t len ) override;

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
    utils::ClientAddr m_addr;
    bool m_connected = false;
    bool m_moved     = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace system
} // namespace net
} // namespace hub

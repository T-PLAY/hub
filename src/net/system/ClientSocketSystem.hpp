/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/18

#pragma once

#include <mutex>

#include "SocketSystem.hpp"
#include "net/ClientSocketI.hpp"

namespace hub {
namespace net {
namespace system {

///
/// \brief The ClientSocketSystem class
/// allows connection to remote server hubs.
/// This class describes the functionality needed to open a remote communication.
///
class SRC_API ClientSocketSystem : public ClientSocketI, public SocketSystem
{
  public:
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
    explicit ClientSocketSystem(
        utils::socket_fd fdSock,
        utils::ClientAddr clientAddr ); // server side client (bind and listen)

    ClientSocketSystem( const ClientSocketSystem& sock ) = delete;
    ClientSocketSystem( ClientSocketSystem&& sock )      = default;
    ClientSocketSystem& operator=( const ClientSocketSystem& sock ) = delete;
    ClientSocketSystem&& operator=( ClientSocketSystem&& sock ) = delete;

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
    bool isConnected() const override;

    ///
    /// \brief close
    ///
    void disconnect() override;

    ///
    /// \brief write
    /// \param data
    /// \param size
    ///
    void write( const Data_t* data, size_t size ) override;

    ///
    /// \brief read
    /// \param data
    /// \param size
    ///
    void read( Data_t* data, size_t size ) override;

    std::string toString() const { return m_ipv4 + "-" + std::to_string( m_port ); }

  public:
    ///
    /// \brief isEnd
    /// \return
    ///

  private:
    ///
    /// \brief initServerAddress
    ///
    void initServerAddress();

  private:
    std::string m_ipv4;
    int m_port;
    utils::ClientAddr m_addr;
    bool m_connected = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace system
} // namespace net
} // namespace hub

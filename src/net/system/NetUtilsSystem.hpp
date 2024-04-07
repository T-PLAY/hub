/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/18

#pragma once

#include <iostream>
#include <memory>

#include "core/Base.hpp"

namespace hub {
namespace net {
namespace system {
namespace utils {

using socket_fd = uint64_t;

socket_fd SRC_API invalidSocket();
bool SRC_API isValid( socket_fd sock );
void SRC_API closeSocket( socket_fd& sock );
bool SRC_API isConnected( socket_fd sock );
bool SRC_API isValid( const std::string& ipv4 );
bool SRC_API isValid( int port );

///////////////////////////////////// SERVER ADDRESS ///////////////////////////////////////

class ServerAddrImpl;

///
/// \brief The ServerAddr class
///
class SRC_API ServerAddr
{
  public:
    explicit ServerAddr();
    ~ServerAddr();

    ///
    /// \brief init
    /// \param port
    ///
    void init( int port );

    ///
    /// \brief m_pimpl
    ///
    std::unique_ptr<ServerAddrImpl> m_pimpl;
};

///
/// \brief serverSocket
/// \return
///
socket_fd SRC_API serverSocket();

///
/// \brief bind
/// \param sock
/// \param addr
/// \return
///
int SRC_API bind( socket_fd sock, ServerAddr& addr );

///
/// \brief listen
/// \param sock
/// \param backlog
/// \return
///
int SRC_API listen( socket_fd sock, int backlog );

/////////////////////////////////////// CLIENT ADDRESS /////////////////////////////////////

class ClientAddrImpl;

///
/// \brief The ClientAddr class
///
class SRC_API ClientAddr
{
  public:
    explicit ClientAddr();
    ~ClientAddr();

    ///
    /// \brief ClientAddr
    /// \param clientAddr
    ///
    ClientAddr( ClientAddr&& clientAddr ) noexcept;

    ///
    /// \brief init
    /// \param ipv4
    /// \param port
    ///
    void init( const std::string& ipv4, int port );

    ///
    /// \brief setPort
    /// \param port
    ///
    void setPort( int port );

    ///
    /// \brief setIpv4
    /// \param ipv4
    ///
    void setIpv4( const std::string& ipv4 );

    ///
    /// \brief getIpv4
    /// \return
    ///
    std::string getIpv4() const;

    ///
    /// \brief getPort
    /// \return
    ///
    int getPort() const;

    ///
    /// \brief m_pimpl
    ///
    std::unique_ptr<ClientAddrImpl> m_pimpl;
};

///
/// \brief accept
/// \param sock
/// \param addr
/// \return
///
socket_fd SRC_API accept( socket_fd sock, ClientAddr& addr );

///
/// \brief clientSocket
/// \return
///
socket_fd SRC_API clientSocket();

///
/// \brief connect
/// \param sock
/// \param addr
/// \return
///
int SRC_API connect( socket_fd sock, ClientAddr& addr );

///
/// \brief send
/// \param sock
/// \param buf
/// \param len
/// \param flags
/// \return
///
int64_t SRC_API send( socket_fd sock, const char* buf, size_t len, int flags );

///
/// \brief recv
/// \param buf
/// \param len
/// \param flags
/// \return
///
int64_t SRC_API recv( socket_fd, char* buf, size_t len, int flags );

} // namespace utils
} // namespace system
} // namespace net
} // namespace hub

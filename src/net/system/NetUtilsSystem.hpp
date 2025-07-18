/*******************************************************************************
* Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
* Please visit https://www.irit.fr/tplay/.
*
* All rights reserved.
* This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*
* Initial Contributors:
*   - Nicolas Mellado (IRIT)
*   - Gauthier Bouyjou (IRIT)
*   - François Gaits (IRIT)
*******************************************************************************/

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
/// represents socket address from server side
///
class SRC_API ServerAddr
{
  public:
    explicit ServerAddr();
    ~ServerAddr();

    ///
    /// \brief Initialize server address with specific port
    /// \param port [in] Service port you want to oppened
    ///
    void init( int port );

    ///
    /// \brief Use hide implement because socket depends of system
    /// avoid to have socket includes in the header file
    ///
    std::unique_ptr<ServerAddrImpl> m_pimpl;
};

socket_fd SRC_API serverSocket();

///
/// \brief bind server address to socket
/// \param sock [in] to bind
/// \param addr [in] to bind
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
/// represents socket address from client side
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

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
*   - Gauthier Bouyjou
*******************************************************************************/

#pragma once

#ifdef HUB_USE_BOOST

#    include "net/ClientSocketI.hpp"
#    include <boost/asio.hpp>
#    include <boost/asio/ip/tcp.hpp>

namespace hub {
namespace net {
namespace boost {

///
/// \brief The ClientSocketBoost class
/// allows connection to remote server hubs.
/// This class describes the functionality needed to open a remote communication.
///
class SRC_API ClientSocketBoost : public ClientSocketI
{
  public:
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
    explicit ClientSocketBoost(
        ::boost::asio::ip::tcp::socket&& fdSock ); // server side client (bind and listen)

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
    /// \param len
    ///
    void write( const unsigned char* data, size_t size ) override;

    ///
    /// \brief read
    /// \param data
    /// \param len
    ///
    void read( unsigned char* data, size_t size ) override;

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

    ::boost::asio::io_service m_ioService;

    ::boost::asio::ip::tcp::socket m_sock;

    bool m_moved = false;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace boost
} // namespace net
} // namespace hub

#endif

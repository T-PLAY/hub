/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Copyright © 2021-2024 Hub. All Rights Reserved.
 * @author Gauthier Bouyjou <gauthierbouyjou@aol.com>
 * @date 2023/10/18
 */

#pragma once

#ifdef HUB_USE_BOOST

#    include "ClientSocketBoost.hpp"
#    include "net/ServerSocketI.hpp"
#    include <boost/asio.hpp>
#    include <boost/asio/ip/tcp.hpp>

namespace hub {
namespace net {
namespace boost {

///
/// \brief The ServerSocketBoost class
/// enable the server application to listen client sockets (streamer, viewer).
///
class SRC_API ServerSocketBoost : public ServerSocketI
{
  public:
    ///
    /// \brief ServerSocketBoost
    /// create server socket with the default service port.
    ///

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

} // namespace boost
} // namespace net
} // namespace hub

#endif

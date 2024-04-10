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

#include "ClientSocketSystem.hpp"
#include "SocketSystem.hpp"
#include "net/ServerSocketI.hpp"

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

    ///
    /// \brief ServerSocketSystem
    /// create server socket with specific service port.
    /// \param port
    /// you want to open to listen.
    ///
    explicit ServerSocketSystem( int port );

    ServerSocketSystem( const ServerSocketSystem& sock ) = delete;
    ServerSocketSystem( ServerSocketSystem&& sock )      = delete;
    ServerSocketSystem& operator=( const ServerSocketSystem& sock ) = delete;
    ServerSocketSystem&& operator=( ServerSocketSystem&& sock ) = delete;

    ///
    /// \brief waitNewClient
    /// wait client connection.
    /// \return
    /// return client socket to communication with.
    ///
    ClientSocketSystem waitNewClient();

    ///
    /// \brief toString
    /// \return
    ///
    std::string toString() const { return std::to_string( m_port ); }

  private:
    void initServer();

  private:
    int m_port;
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

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

#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include "core/Base.hpp"
#include "NetUtilsSystem.hpp"

namespace hub {
namespace net {
namespace system {

///
/// \brief The SocketSystem class
/// is an abstraction of different (Windows, Unix) network sockets
/// allows the TCP communication.
///
class SRC_API SocketSystem
{
  public:
    ///
    /// \brief The exception class
    /// is used to launch exception when broken communication
    /// through the internet.
    ///
    class exception : public std::runtime_error
    {
      public:
        ///
        /// \brief exception
        /// launched when peer connection is lost or invalid.
        /// \param message
        /// shown by the client application when exception has been launched.
        ///
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}

        ///
        /// \brief what
        /// allow the user to catch message exception.
        /// \return
        /// message describe the exception.
        ///
        const char* what() const throw() { return std::runtime_error::what(); }
    };

    ///
    /// \brief isConnected
    /// is used to test if peer connection is valid.
    /// \return
    /// true if connection is alive.\n
    /// false otherwise.
    ///
    bool isConnected() const;

  protected:
#ifdef HUB_DEBUG_SOCKET
    std::string getHeader() const;
#endif
    ///
    /// \brief SocketSystem
    ///
    SocketSystem();
    ~SocketSystem();

    SocketSystem( const SocketSystem& socket ) = delete;

    ///
    /// \brief SocketSystem
    /// \param socket
    ///
    SocketSystem( SocketSystem&& socket );

    SocketSystem& operator=( const SocketSystem& socket ) = delete;
    SocketSystem&& operator=( SocketSystem&& socket ) = delete;

  protected:
    ///
    /// \brief m_fdSock
    ///
    utils::socket_fd m_fdSock = utils::invalidSocket();

    ///
    /// \brief m_serverSide
    ///
    bool m_serverSide = false;

    ///
    /// \brief m_moved
    ///
    bool m_moved      = false;
};

} // namespace system
} // namespace net
} // namespace hub

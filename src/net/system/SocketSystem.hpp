/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/18
	
#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include "NetUtilsSystem.hpp"
#include "core/Macros.hpp"

// #define HUB_DEBUG_SOCKET

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
    SocketSystem();
    ~SocketSystem();

    SocketSystem( const SocketSystem& socket ) = delete;
    SocketSystem( SocketSystem&& socket );
    SocketSystem& operator=( const SocketSystem& socket ) = delete;
    SocketSystem&& operator=( SocketSystem&& socket )     = delete;

  protected:
    ///
    /// \brief m_fdSock
    ///
    utils::socket_fd m_fdSock = utils::invalidSocket();
    // utils::socket_fd m_fdSockInited = utils::invalidSocket();

    bool m_serverSide = false;
    bool m_moved = false;
};

} // namespace system
} // namespace net
} // namespace hub

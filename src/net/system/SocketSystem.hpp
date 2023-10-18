#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include "core/Macros.hpp"
#include "NetUtilsSystem.hpp"

// #define DEBUG_SOCKET

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

#ifdef DEBUG_SOCKET
    std::string getHeader( net::utils::socket_fd iSock ) const;
#endif

    ///
    /// \brief isConnected
    /// is used to test if peer connection is valid.
    /// \return
    /// true if connection is alive.\n
    /// false otherwise.
    ///
    bool isConnected() const;

  protected:
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
};

} // namespace system
} // namespace net
} // namespace hub

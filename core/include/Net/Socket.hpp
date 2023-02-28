#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include "Macros.hpp"
#include "Net/Utils.hpp"

// #define DEBUG_SOCKET

namespace hub {
namespace net {

///
/// \brief The Socket class
/// is an abstraction of different (Windows, Unix) network sockets
/// allows the TCP communication.
///
class SRC_API Socket
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
    std::string getHeader( socket_fd iSock ) const;
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
    Socket();
    ~Socket();

    Socket( const Socket& socket ) = delete;
    ///
    /// \brief Socket
    /// \param socket
    ///
    Socket( Socket&& socket );

    Socket& operator=( const Socket& socket ) = delete;
    Socket&& operator=( Socket&& socket ) = delete;

  protected:
    ///
    /// \brief m_fdSock
    ///
    mutable net::utils::socket_fd m_fdSock = net::utils::invalidSocket();

    ///
    /// \brief m_serverSide
    ///
    bool m_serverSide = false;
};

} // namespace net
} // namespace hub

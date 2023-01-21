#pragma once

// #define NOMINMAX

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
/// is an abstraction of socket use in (Windows, Linux).
///
class SRC_API Socket
{
  public:

    ///
    /// \brief The exception class
    ///
    class exception : public std::runtime_error
    {
      public:
        ///
        /// \brief exception
        /// \param message
        ///
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}

        ///
        /// \brief what
        /// \return
        ///
        const char* what() const throw() { return std::runtime_error::what(); }
    };

#ifdef DEBUG_SOCKET
    std::string getHeader( socket_fd iSock ) const;
#endif

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

    ///
    /// \brief isConnected
    /// \return
    ///
    bool isConnected() const;
    //    void clear() const;

  protected:
    //    mutable socket_fd m_fdSock = INVALID_SOCKET;
    ///
    /// \brief m_fdSock
    ///
    mutable net::utils::socket_fd m_fdSock = net::utils::invalidSocket();

    ///
    /// \brief m_serverSide
    ///
    bool m_serverSide                      = false;
};

} // namespace net
} // namespace hub

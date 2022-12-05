#pragma once

//#define NOMINMAX

#include <exception>
#include <functional>
#include <list>
#include <string>
#include <iostream>

#include "Macros.hpp"
#include "Net/Utils.hpp"

//#define DEBUG_SOCKET

namespace hub {
namespace net {

///
/// \brief The Socket class
/// is an abstraction of socket use in (Windows, Linux).
///
class SRC_API Socket
{
  public:
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

#ifdef DEBUG_SOCKET
    std::string getHeader( socket_fd iSock ) const;
#endif

  protected:
    Socket();
    ~Socket();

    Socket( const Socket& socket ) = delete;
    Socket( Socket&& socket );
    Socket& operator=( const Socket& socket ) = delete;
    Socket&& operator=(Socket&& socket) = delete;

    bool isConnected() const;
//    void clear() const;

  protected:
//    mutable socket_fd m_fdSock = INVALID_SOCKET;
    mutable net::utils::socket_fd m_fdSock = net::utils::invalidSocket();
    bool m_serverSide  = false;
};

} // namespace net
} // namespace hub

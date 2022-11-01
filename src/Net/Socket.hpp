#pragma once

//#define NOMINMAX

#include <exception>
#include <functional>
#include <list>
#include <string>

#include "Macros.hpp"
#include "Utils.hpp"

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
    //    static const int s_defaultServicePort;
    //    static const char* const s_defaultServiceIp;

    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

    bool isConnected() const;
#ifdef DEBUG_SOCKET
    std::string getHeader( socket_fd iSock ) const;
#endif

  protected:
    Socket();
    ~Socket();

    Socket( const Socket& socket ) = delete;
    Socket( Socket&& socket );
    Socket& operator=( const Socket& socket ) = delete;

  protected:
    socket_fd m_fdSock = INVALID_SOCKET;
    bool m_serverSide  = false;
};

} // namespace net
} // namespace hub

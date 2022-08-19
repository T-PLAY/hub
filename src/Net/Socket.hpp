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

class SRC_API Socket
{
  public:
    static const int s_defaultServicePort;
    static const char* const s_defaultServiceIp;

    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };


    bool isConnected() const;
    static std::string getHeader( socket_fd iSock );

  protected:
    Socket();
    ~Socket();

  protected:
    socket_fd mFdSock = INVALID_SOCKET;
};

} // namespace net
} // namespace hub

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

    enum class Message {
        NONE,
        PING,
        SYNC,
        DATA,
        OK,
        CLOSE,
        DEL_STREAMER,
        NEW_STREAMER,
        NOT_FOUND,
        FOUND,
        COUNT
    };
    static constexpr char const* message2string[static_cast<int>( Message::COUNT )] = {
        "NONE",
        "PING",
        "SYNC",
        "DATA",
        "OK",
        "CLOSE",
        "DEL_STREAMER",
        "NEW_STREAMER",
        "NOT_FOUND",
        "FOUND",
    };
    friend std::ostream& operator<<( std::ostream& os, const Message& msg ) {
        os << message2string[(int)msg];
        return os;
    }

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

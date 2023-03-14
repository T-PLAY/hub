#include "net/Socket.hpp"

#include <algorithm>
#include <cassert>
#include <exception>

// #ifdef WIN32
// #    include <WS2tcpip.h>
// #endif

namespace hub {
namespace net {

// constexpr int Socket::s_defaultServicePort       = 4041;
// const char* const Socket::s_defaultServiceIp = "127.0.0.1";

#ifdef DEBUG_SOCKET
std::string Socket::getHeader( socket_fd iSock ) const {
    std::string str;
    if ( m_serverSide ) { str += "\t\033[1m[Server]\033[0m"; }
    else {
        str += "[Client]";
    }
    str += "\033[" + std::to_string( 31 + iSock % 7 ) + "m[socket:" + std::to_string( iSock ) +
           "]\033[0m ";
    return str;
}
#endif

Socket::Socket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "Socket()" );
#endif
}

Socket::Socket( Socket&& socket ) {
    m_fdSock        = socket.m_fdSock;
    socket.m_fdSock = net::utils::invalidSocket();
}

net::utils::socket_fd Socket::getFdSock() const {
    return m_fdSock;
}

// void Socket::clear() const {

// #ifdef DEBUG_SOCKET
// #endif
//}

Socket::~Socket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~Socket()" );
#endif

    if ( net::utils::isValid( m_fdSock ) ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "close socket" );
#endif
        net::utils::closeSocket( m_fdSock );
        m_fdSock = net::utils::invalidSocket();
    }
}

bool Socket::isConnected() const {
    return net::utils::isConnected( m_fdSock );
}

} // namespace net
} // namespace hub

#include "Socket.hpp"

#include <algorithm>
#include <cassert>
#include <exception>

#ifdef WIN32
#    include <WS2tcpip.h>
#endif

namespace hub {
namespace net {

constexpr int Socket::s_defaultServicePort       = 4042;
const char* const Socket::s_defaultServiceIp = "127.0.0.1";

std::string Socket::getHeader( socket_fd iSock ) const {
    std::string str;
    if (m_serverSide) {
        str += "\t\033[1m[Server]\033[0m";
    }
    else {
        str += "[Client]";
    }
    str += "\033[" + std::to_string( 31 + iSock % 7 ) +
                      "m[socket:" + std::to_string( iSock ) + "]\033[0m ";
    return str;
}

Socket::Socket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "Socket()" );
#endif
    net::init();
}

Socket::Socket(Socket &&socket)
{
    m_fdSock = socket.m_fdSock;
    socket.m_fdSock = INVALID_SOCKET;
}

Socket::~Socket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~Socket()" );
#endif
    if ( m_fdSock != INVALID_SOCKET ) {
        DEBUG_MSG( getHeader( m_fdSock ) << "close socket" );
        net::clearSocket( m_fdSock );
    }
}

bool Socket::isConnected() const {
#ifdef WIN32
    // not sure how to check this in windows
    return true;
#else

    // TODO - make this better, because its probably wrong (but seems to work)

    int optval;
    socklen_t optlen = sizeof( optval );

    int res = getsockopt( m_fdSock, SOL_SOCKET, SO_ERROR, &optval, &optlen );

    if ( optval == 0 && res == 0 ) return true;
#endif

    return false;
}

} // namespace net
} // namespace hub

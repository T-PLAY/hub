#include "Socket.hpp"

#include <algorithm>
#include <cassert>
#include <exception>

namespace hub {
namespace net {

#ifdef DEBUG_SOCKET
std::string Socket::getHeader(utils::socket_fd iSock ) const {
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
    std::cout << "[Socket] Socket(Socket&&)" << std::endl;
    m_fdSock        = socket.m_fdSock;
    socket.m_fdSock = net::utils::invalidSocket();
}

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

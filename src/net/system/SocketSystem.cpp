#include "SocketSystem.hpp"

#include <algorithm>
#include <exception>

namespace hub {
namespace net {
namespace system {

#ifdef HUB_DEBUG_SOCKET
std::string SocketSystem::getHeader() const {
    std::string str;
    if ( m_serverSide ) { str += "\t\033[1m[Server]\033[0m"; }
    else { str += "[Client]"; }
    if ( utils::isValid( m_fdSock ) ) {
        str += "\033[" + std::to_string( 31 + m_fdSock % 7 ) +
               "m[socket:" + std::to_string( m_fdSock ) + "]\033[0m ";
    }
    else { str += "\033[31m[socket:invalid]\033[0m"; }
    return str;
}
#endif

SocketSystem::SocketSystem() {
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "SocketSystem()" );
#endif
}

SocketSystem::SocketSystem( SocketSystem&& socket ) :
    m_fdSock( socket.m_fdSock ), m_serverSide( socket.m_serverSide ), m_moved( socket.m_moved ) {
    socket.m_moved = true;
}

SocketSystem::~SocketSystem() {
    if ( !m_moved ) {
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( getHeader() << "~Socket()" );
#endif

        if ( utils::isValid( m_fdSock ) ) {
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( getHeader() << "closing socket ..." );
#endif
            utils::closeSocket( m_fdSock );
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( getHeader() << "socket closed" );
#endif
        }
    }
}

bool SocketSystem::isConnected() const {
    return utils::isConnected( m_fdSock );
}

} // namespace system
} // namespace net
} // namespace hub

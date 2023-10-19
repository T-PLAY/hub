#include "SocketSystem.hpp"

#include <algorithm>
#include <exception>

namespace hub {
namespace net {
namespace system {

#ifdef DEBUG_SOCKET
std::string SocketSystem::getHeader(utils::socket_fd iSock ) const {
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

SocketSystem::SocketSystem() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "SocketSystem()" );
#endif
}

SocketSystem::SocketSystem( SocketSystem&& socket ) {
    std::cout << "[SocketSystem] SocketSystem(SocketSystem&&)" << std::endl;
    m_fdSock        = socket.m_fdSock;
    socket.m_fdSock = utils::invalidSocket();
}

SocketSystem::~SocketSystem() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~SocketSystem()" );
#endif

    if ( utils::isValid( m_fdSock ) ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "close socket" );
#endif
        utils::closeSocket( m_fdSock );
        m_fdSock = utils::invalidSocket();
    }
}

bool SocketSystem::isConnected() const {
    return utils::isConnected( m_fdSock );
}

} // namespace system
} // namespace net
} // namespace hub

#include "ServerSocketSystem.hpp"

#include <cstring>

#include "NetUtilsSystem.hpp"

namespace hub {
namespace net {
namespace system {

ServerSocketSystem::ServerSocketSystem( int port ) : m_port( port ) {
    m_serverSide = true;
    initServer();
}

ClientSocketSystem ServerSocketSystem::waitNewClient() {

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "wait client on port " << m_port );
#endif

    assert( utils::isValid( m_fdSock ) );

    utils::ClientAddr clientAddr;
    utils::socket_fd new_socket = utils::accept( m_fdSock, clientAddr );
    if ( !utils::isValid( new_socket ) ) {
        perror( "not accept new socket" );
        utils::closeSocket( new_socket );
        exit( 1 );
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "new client on socket " << new_socket );
#endif

    return ClientSocketSystem( new_socket, std::move( clientAddr ) );
}

void ServerSocketSystem::initServer() {

    // Socket creation
    m_fdSock = utils::serverSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "socket creation failed.\n" );
        exit( 1 );
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "server started " );
#endif

    m_addr.init( m_port );

    // Create server
    if ( utils::bind( m_fdSock, m_addr ) < 0 ) {
        throw SocketSystem::exception( ( std::string( "Failed to bind because port " ) +
                                         std::to_string( m_port ) + " already in use" )
                                           .c_str() );
    }

    if ( utils::listen( m_fdSock, 10 ) < 0 ) {
        perror( "listen" );
        exit( 1 );
    }

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "server inited " );
#endif
}

int ServerSocketSystem::getPort() const {
    return m_port;
}

} // namespace system
} // namespace net
} // namespace hub

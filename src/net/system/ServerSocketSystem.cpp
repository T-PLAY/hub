#include "ServerSocketSystem.hpp"

#include <cstring>

#include "NetUtilsSystem.hpp"

namespace hub {
namespace net {
namespace system {

//ServerSocketSystem::ServerSocketSystem() : mPort( s_defaultServicePort ) {
//    m_serverSide = true;
//    initServer();
//}

ServerSocketSystem::ServerSocketSystem( int port )
//    : ServerSocketInterface(port),
    : mPort( port )
{
    m_serverSide = true;
    initServer();
}

ServerSocketSystem::~ServerSocketSystem()
{
    assert(utils::isValid( m_fdSock ));
    utils::closeSocket( m_fdSock );
}

//#define DEBUG_SOCKET

ClientSocketSystem ServerSocketSystem::waitNewClient() {

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "wait client on port " << mPort );
#endif
    utils::socket_fd new_socket = utils::accept( m_fdSock, m_addr );
    if ( !utils::isValid( new_socket ) ) {
        perror( "not accept new socket" );
        utils::closeSocket( new_socket );
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
#endif

    return ClientSocketSystem( new_socket );
}

void ServerSocketSystem::initServer() {

    // Socket creation
    m_fdSock = utils::serverSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "socket creation failed.\n" );
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
#endif

    m_addr.init( mPort );

    // Create server
    if ( utils::bind( m_fdSock, m_addr ) < 0 ) {
        throw SocketSystem::exception( ( std::string( "Failed to bind because port " ) +
                                   std::to_string( mPort ) + " already in use" )
                                     .c_str() );
    }

    if ( utils::listen( m_fdSock, 3 ) < 0 ) {
        perror( "listen" );
        exit( 1 );
    }

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server inited " );
#endif
}

int ServerSocketSystem::getPort() const {
    return mPort;
}

} // namespace system
} // namespace net
} // namespace hub

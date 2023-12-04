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
//    : ServerSocketI(port),
    : mPort( port )
{
    m_serverSide = true;
    initServer();
}

// ServerSocketSystem::~ServerSocketSystem()
// {
//     assert(utils::isValid( m_fdSock ));
//     utils::closeSocket( m_fdSock );
// }

//#define HUB_DEBUG_SOCKET

ClientSocketSystem ServerSocketSystem::waitNewClient() {

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader(  ) << "wait client on port " << mPort );
#endif


    assert(utils::isValid(m_fdSock));

    utils::ClientAddr clientAddr;
    utils::socket_fd new_socket = utils::accept( m_fdSock, clientAddr );
    if ( !utils::isValid( new_socket ) ) {
        perror( "not accept new socket" );
        utils::closeSocket( new_socket );
        exit( 1 );
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader(  ) << "new client on socket " << new_socket );
#endif

    return ClientSocketSystem( new_socket, std::move(clientAddr) );
}

//void ServerSocketSystem::disconnect()
//{
//    if ( isConnected() ) {
//        utils::closeSocket( m_fdSock );
//    }
//    assert( !isConnected() );
//}

void ServerSocketSystem::initServer() {

    // Socket creation
    m_fdSock = utils::serverSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "socket creation failed.\n" );
        exit( 1 );
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader(  ) << "server started " );
#endif

    m_addr.init( mPort );

    // Create server
    if ( utils::bind( m_fdSock, m_addr ) < 0 ) {
        throw SocketSystem::exception( ( std::string( "Failed to bind because port " ) +
                                   std::to_string( mPort ) + " already in use" )
                                     .c_str() );
    }

    if ( utils::listen( m_fdSock, 10 ) < 0 ) {
        perror( "listen" );
        exit( 1 );
    }

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader(  ) << "server inited " );
#endif

//    DEBUG_MSG( "starting server at port " << mPort );
//    DEBUG_MSG( "[OutputStream] listening port " << mPort );
}

int ServerSocketSystem::getPort() const {
    return mPort;
}

} // namespace system
} // namespace net
} // namespace hub

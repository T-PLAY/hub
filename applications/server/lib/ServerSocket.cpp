#include "ServerSocket.hpp"

#include <Configurations.hpp>

#include <cstring>

namespace hub {
namespace net {

ServerSocket::ServerSocket() : mPort( s_defaultServicePort ) {
    m_serverSide = true;
    initServer();
}

ServerSocket::ServerSocket( int port ) : mPort( port ) {
    m_serverSide = true;
    initServer();
}

ClientSocket ServerSocket::waitNewClient() {

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "wait client on port " << mPort );
#endif
    socklen_t addrlen    = sizeof( mAddress );
    socket_fd new_socket = accept( m_fdSock, (struct sockaddr*)&mAddress, &addrlen );
    if ( new_socket == INVALID_SOCKET ) {
        perror( "not accept new socket" );
        net::clearSocket( new_socket );
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
#endif
    net::registerSocket( new_socket );

    return ClientSocket( new_socket );
}

void ServerSocket::initServer() {

    // Socket creation
    m_fdSock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( m_fdSock < 0 ) {
        perror( "socket creation failed.\n" );
        return;
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
#endif
    net::registerSocket( m_fdSock );

    // Server address construction
    memset( &mAddress, 0, sizeof( mAddress ) );
    mAddress.sin_family      = AF_INET;
    mAddress.sin_addr.s_addr = INADDR_ANY;
    mAddress.sin_port        = htons( mPort ); // Server port

    // Create server
    if ( bind( m_fdSock, (struct sockaddr*)&mAddress, sizeof( mAddress ) ) < 0 ) {
        perror( "Failed to bind.\n" );
        return;
    }

    if ( listen( m_fdSock, 3 ) < 0 ) {
        perror( "listen" );
        return;
    }

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server inited " );
#endif
}

int ServerSocket::getPort() const {
    return mPort;
}

} // namespace net
} // namespace hub

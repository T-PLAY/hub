#include "ServerSocket.hpp"

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

    DEBUG_MSG( getHeader( m_fdSock ) << "wait client on port " << mPort );
    socklen_t addrlen    = sizeof( mAddress );
    socket_fd new_socket = accept( m_fdSock, (struct sockaddr*)&mAddress, &addrlen );
    if ( new_socket == INVALID_SOCKET ) {
        perror( "not accept new socket" );
        net::clearSocket( new_socket );
        exit( 1 );
    }
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
//    net::sSockets.push_back( new_socket );
    net::registerSocket(new_socket);

    return ClientSocket( new_socket );
}

void ServerSocket::initServer() {

    // Socket creation
    m_fdSock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( m_fdSock < 0 ) {
        perror( "socket creation failed.\n" );
        return;
    }
    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
//    net::sSockets.push_back( mFdSock );
    net::registerSocket(m_fdSock);

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

    DEBUG_MSG( getHeader( m_fdSock ) << "server inited " );
}

} // namespace net
} // namespace hub

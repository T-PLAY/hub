#include "net/ServerSocket.hpp"

#include <cstring>

#include <net/Utils.hpp>

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
    //    socklen_t addrlen    = sizeof( mAddress );
    //    socket_fd new_socket = accept( m_fdSock, (struct sockaddr*)&mAddress, &addrlen );
    //    socket_fd new_socket = net::accept( m_fdSock, (struct sockaddr*)&mAddress, &addrlen );
    net::utils::socket_fd new_socket = net::utils::accept( m_fdSock, m_addr );
    //    if ( new_socket == INVALID_SOCKET ) {
    if ( !net::utils::isValid( new_socket ) ) {
        perror( "not accept new socket" );
        net::utils::closeSocket( new_socket );
        //        ::close(m_fdSock);
        //        m_fdSock = INVALID_SOCKET;
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
#endif
    //    net::registerSocket( new_socket );

    return ClientSocket( new_socket );
}

void ServerSocket::initServer() {

    // Socket creation
    //    m_fdSock = net::createSocket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
    m_fdSock = net::utils::serverSocket();
    //    if ( m_fdSock < 0 ) {
    if ( !net::utils::isValid( m_fdSock ) ) {
        perror( "socket creation failed.\n" );
        exit( 1 );
        //        return;
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
#endif
    //    net::registerSocket( m_fdSock );

    //    // Server address construction
    //    memset( &mAddress, 0, sizeof( mAddress ) );
    //    mAddress.sin_family      = AF_INET;
    //    mAddress.sin_addr.s_addr = INADDR_ANY;
    //    mAddress.sin_port        = htons( mPort ); // Server port

    m_addr.init( mPort );

    // Create server
    //    if ( net::bind( m_fdSock, (struct sockaddr*)&mAddress, sizeof( mAddress ) ) < 0 ) {
    if ( net::utils::bind( m_fdSock, m_addr ) < 0 ) {
        perror( "Failed to bind.\n" );
        exit( 1 );
        //        return;
    }

    if ( net::utils::listen( m_fdSock, 3 ) < 0 ) {
        perror( "listen" );
        exit( 1 );
        //        return;
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

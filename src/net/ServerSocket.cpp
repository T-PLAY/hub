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
    net::utils::socket_fd new_socket = net::utils::accept( m_fdSock, m_addr );
    if ( !net::utils::isValid( new_socket ) ) {
        perror( "not accept new socket" );
        net::utils::closeSocket( new_socket );
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
#endif

    return ClientSocket( new_socket );
}

void ServerSocket::initServer() {

    // Socket creation
    m_fdSock = net::utils::serverSocket();
    if ( !net::utils::isValid( m_fdSock ) ) {
        perror( "socket creation failed.\n" );
        exit( 1 );
    }
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
#endif

    m_addr.init( mPort );

    // Create server
    if ( net::utils::bind( m_fdSock, m_addr ) < 0 ) {
        throw Socket::exception( ( std::string( "Failed to bind because port " ) +
                                   std::to_string( mPort ) + " already in use" )
                                     .c_str() );
    }

    if ( net::utils::listen( m_fdSock, 3 ) < 0 ) {
        perror( "listen" );
        exit( 1 );
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

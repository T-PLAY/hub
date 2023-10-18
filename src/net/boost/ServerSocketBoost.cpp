#include "ServerSocketBoost.hpp"

#include <cassert>
#include <cstring>

// #include "NetUtilsSystem.hpp"

namespace hub {
namespace net {
namespace boost {

// ServerSocketBoost::ServerSocketBoost() : mPort( s_defaultServicePort ) {
//     m_serverSide = true;
//     initServer();
// }

ServerSocketBoost::ServerSocketBoost( int port )
    //    : ServerSocketInterface(port),
    :
    m_port( port ),
    m_acceptor( m_ioService,
                ::boost::asio::ip::tcp::endpoint( ::boost::asio::ip::tcp::v4(), m_port ) ) {
    //    m_serverSide = true;
    //    initServer();
}

ServerSocketBoost::~ServerSocketBoost() {
    //    assert(utils::isValid( m_fdSock ));
    //    utils::closeSocket( m_fdSock );
}

ClientSocketBoost ServerSocketBoost::waitNewClient() {

    // #ifdef DEBUG_SOCKET
    //     DEBUG_MSG( getHeader( m_fdSock ) << "wait client on port " << mPort );
    // #endif
    //     utils::socket_fd new_socket = utils::accept( m_fdSock, m_addr );
    //     if ( !utils::isValid( new_socket ) ) {
    //         perror( "not accept new socket" );
    //         utils::closeSocket( new_socket );
    //         exit( 1 );
    //     }
    // #ifdef DEBUG_SOCKET
    //     DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << new_socket );
    // #endif

    ::boost::asio::ip::tcp::socket socket( m_ioService );
    m_acceptor.accept( socket );

    return ClientSocketBoost(std::move(socket));

    //    return ClientSocketSystem( new_socket );
    //    return ClientSocketBoost();

        // waiting for connection
        // read operation
}

void ServerSocketBoost::initServer() {

    // Socket creation
    //    m_fdSock = utils::serverSocket();
    //    if ( !utils::isValid( m_fdSock ) ) {
    //        perror( "socket creation failed.\n" );
    //        exit( 1 );
    //    }
    // #ifdef DEBUG_SOCKET
    //    DEBUG_MSG( getHeader( m_fdSock ) << "server started " );
    // #endif

    //    m_addr.init( mPort );

    //    // Create server
    //    if ( utils::bind( m_fdSock, m_addr ) < 0 ) {
    //        throw SocketSystem::exception( ( std::string( "Failed to bind because port " ) +
    //                                   std::to_string( mPort ) + " already in use" )
    //                                     .c_str() );
    //    }

    //    if ( utils::listen( m_fdSock, 3 ) < 0 ) {
    //        perror( "listen" );
    //        exit( 1 );
    //    }

    // #ifdef DEBUG_SOCKET
    //     DEBUG_MSG( getHeader( m_fdSock ) << "server inited " );
    // #endif
}

int ServerSocketBoost::getPort() const {
    //    return mPort;
    //    return mPort;
    return m_port;
}

} // namespace boost
} // namespace net
} // namespace hub

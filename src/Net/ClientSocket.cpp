#include "ClientSocket.hpp"

#include "Configurations.hpp"

#include <cstring>

namespace hub {
namespace net {

static constexpr std::string_view type2string[static_cast<int>( ClientSocket::Type::COUNT )] = {
    "NONE",
    "STREAMER",
    "VIEWER",
    "STREAM_VIEWER",
};
std::ostream& operator<<( std::ostream& os, const ClientSocket::Type& type ) {
    os << type2string[(int)type];
    return os;
}

static constexpr std::string_view message2string[static_cast<int>( ClientSocket::Message::COUNT )] =
    {
        "NONE",
        "PING",
        "SYNC",
        "DATA",
        "OK",
        "CLOSE",
        "DEL_STREAMER",
        "NEW_STREAMER",
        "NOT_FOUND",
        "FOUND",
};
std::ostream& operator<<( std::ostream& os, const ClientSocket::Message& msg ) {
    os << message2string[(int)msg];
    return os;
}

void ClientSocket::connectToServer() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( "[ClientSocket] connectToServer" );
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocket] ClientSocket('" << m_ipv4 << ", " << m_port << ")" );
#endif

    /*   struct addrinfo *result = NULL, *ptr = NULL, hints;

       ZeroMemory( &hints, sizeof( hints ) );
       hints.ai_family   = AF_UNSPEC;
       hints.ai_socktype = SOCK_STREAM;
       hints.ai_protocol = IPPROTO_TCP;*/

    // Resolve the server address and port
    /*const int iResult = getaddrinfo(m_ipv4.c_str(), m_port, &hints, &result );
    if ( iResult != 0 ) {
        printf( "getaddrinfo failed: %d\n", iResult );
        WSACleanup();
        return;
    }*/

    // Socket creation
    m_fdSock = socket( PF_INET, SOCK_STREAM, 0 );
    if ( m_fdSock < 0 ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }
    net::registerSocket( m_fdSock );

    // Server address construction
    struct sockaddr_in serv_addr;
    memset( &serv_addr, 0, sizeof( serv_addr ) );
    serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr(m_ipv4.c_str()); // winsock 1.0
    inet_pton( AF_INET, m_ipv4.c_str(), &serv_addr.sin_addr.s_addr ); // winsock 2.0
    serv_addr.sin_port = htons( m_port ); // Server port

    // Connect to server
    while ( connect( m_fdSock, (struct sockaddr*)&serv_addr, sizeof( serv_addr ) ) < 0 ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
#endif
        throw Socket::exception(
            ( ( std::string(
                    "[ClientSocket] connectToServer() Failed to connect to server at address " ) +
                m_ipv4 + " and port " + std::to_string( m_port ) ) )
                .c_str() );
    }

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocket] connected to the server, starting communication" );
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << m_fdSock );
#endif
}

ClientSocket::ClientSocket() : m_ipv4( s_defaultServiceIp ), m_port( s_defaultServicePort ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket()" );
#endif

    connectToServer();
}

ClientSocket::ClientSocket( const std::string& ipv4, int port ) : m_ipv4( ipv4 ), m_port( port ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(std::string ipv4, int port)" );
#endif

    connectToServer();
}

ClientSocket::ClientSocket( socket_fd fdSock ) {
    m_port       = -1;
    m_fdSock     = fdSock;
    m_serverSide = true;
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(socket_fd fdSock)" );
#endif
}

// ClientSocket::ClientSocket( ClientSocket&& sock ) noexcept :
//{

//#ifdef DEBUG_SOCKET
//#endif
//}

ClientSocket::~ClientSocket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~ClientSocket()" );
#endif
    clear();
}

// void ClientSocket::close() {
// }

void ClientSocket::write( const unsigned char* data, size_t len ) const {
#ifdef DEBUG_SOCKET
    // DEBUG_MSG(getHeader(m_fdSock) << "write message ");
#endif
    assert( len > 0 );
    size_t uploadSize = 0;
    do {
        if ( !isConnected() ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG(
                getHeader( m_fdSock )
                << "write(const unsigned char* data, size_t len) : isConnected() client lost" );
#endif
            throw Socket::exception( "[ClientSocket] write(data, len) Server lost" );
        }
        // winsock const char * data
        // winsock int len
        int byteSent;
        try {
            byteSent = send(
                m_fdSock, (const char*)data + uploadSize, static_cast<int>( len - uploadSize ), 0 );
        }
        catch ( std::exception& e ) {
            assert( false );
            throw e;
        }

        if ( byteSent == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "can't send packet " << byteSent << "/" << len );
#endif
            throw Socket::exception(
                "[ClientSocket] write(data, len) Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byteSent == 0, sleep" );
#endif
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
        uploadSize += byteSent;
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock )
                   << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" );
#endif
    } while ( len != uploadSize );
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocket::read( unsigned char* data, size_t len ) const {
    size_t downloadSize = 0;
    do {
        int byteRead =
            recv( m_fdSock, (char*)data + downloadSize, static_cast<int>( len - downloadSize ), 0 );
        if ( byteRead == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
#endif
            throw Socket::exception(
                "[ClientSocket] read(data, len) Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            throw Socket::exception(
                "[ClientSocket] read(data, len) 0 byte received, peer connection lost" );
        }

        downloadSize += byteRead;
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock )
                   << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")" );
#endif
    } while ( len != downloadSize );

#ifdef DEBUG_SOCKET
//
#endif
}

void ClientSocket::close() {
    clear();
}

// Acquisition ClientSocket::getAcquisition(int acquistionSize) const
//{
// }

// void ClientSocket::waitClose() const {



//}

void ClientSocket::clear() {
    if ( m_fdSock != INVALID_SOCKET ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "close socket" );
#endif
        net::clearSocket( m_fdSock );
        m_fdSock = INVALID_SOCKET;
    }
}

} // namespace net
} // namespace hub

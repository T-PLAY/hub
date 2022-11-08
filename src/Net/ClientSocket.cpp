#include "ClientSocket.hpp"

#include <cstring>

#include "Configurations.hpp"

namespace hub {
namespace net {

static const std::regex m_ipv4Regex { "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" };

ClientSocket::ClientSocket() : m_ipv4( s_defaultServiceIp ), m_port( s_defaultServicePort ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket()" );
#endif

    //    initSocket();
    initServerAddress();
    //    connectToServer();
    connect();
}

ClientSocket::ClientSocket( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(std::string ipv4, int port)" );
#endif

    //    initSocket();
    initServerAddress();

    if ( autoConnect ) connect();
    //    connectToServer();
}

ClientSocket::ClientSocket( socket_fd fdSock ) {
    m_port       = -1;
    m_fdSock     = fdSock;
    m_serverSide = true;
    m_connected  = true;
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(socket_fd fdSock)" );
#endif
}

ClientSocket::~ClientSocket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~ClientSocket()" );
#endif
    //    clear();
}

void ClientSocket::initServerAddress() {
    assert( std::regex_match( m_ipv4, m_ipv4Regex ) );
    assert( 0 <= m_port && m_port <= 65535 );

    // Server address construction
    //    struct sockaddr_in serv_addr;
    memset( &m_serverAddress, 0, sizeof( m_serverAddress ) );
    m_serverAddress.sin_family = AF_INET;
    // m_serverAddress.sin_addr.s_addr = inet_addr(m_ipv4.c_str()); // winsock 1.0
    inet_pton( AF_INET, m_ipv4.c_str(), &m_serverAddress.sin_addr.s_addr ); // winsock 2.0
    m_serverAddress.sin_port = htons( m_port );                             // Server port
}

// void ClientSocket::connectToServer() {
void ClientSocket::connect() {
    assert( !m_connected );
    assert( !isOpen() );

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

    assert( m_fdSock == INVALID_SOCKET );
    //            assert(! isConnected());

    //    const auto tmp = m_fdSock;

    // Socket creation
    m_fdSock = socket( PF_INET, SOCK_STREAM, 0 );
    if ( m_fdSock < 0 ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }
//    net::registerSocket( m_fdSock );

    //    assert(tmp == INVALID_SOCKET || m_fdSock == tmp);

    assert( !isOpen() );
    assert( isConnected() );

    //    assert(isConnected());

    // Connect to server
    if ( ::connect( m_fdSock, (struct sockaddr*)&m_serverAddress, sizeof( m_serverAddress ) ) <
         0 ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
#endif
//        perror( "[ClientSocket] socket connect failed.\n" );
                clearSocket(m_fdSock);
//        ::close( m_fdSock );
//        m_fdSock = INVALID_SOCKET;
        throw Socket::exception(
            ( ( std::string( "[ClientSocket] connect() Failed to connect to server at address " ) +
                m_ipv4 + " and port " + std::to_string( m_port ) ) )
                .c_str() );
    }

    m_connected = true;
    assert( isOpen() );
    assert( isConnected() );
    //    assert(isConnected());

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocket] connected to the server, starting communication" );
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << m_fdSock );
#endif
}

void ClientSocket::write( const unsigned char* data, size_t len ) const {
    //        assert( m_connected );
    assert( isOpen() );
    //    assert(isConnected());

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
            close();
            throw Socket::exception( "[ClientSocket] write(data, len) Can't write packet, not connected" );
        }
        // winsock const char * data
        // winsock int len
        int byteSent;
        try {
            byteSent = send(
                m_fdSock, (const char*)data + uploadSize, static_cast<int>( len - uploadSize ), 0 );
            //            assert(isConnected());
        }
        catch ( std::exception& e ) {
            assert( false );
            throw e;
        }

        if ( byteSent == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "can't send packet " << byteSent << "/" << len );
#endif
            perror( "[socket] send failed.\n" );
            assert( isConnected() );
            close();
            throw Socket::exception(
                "[ClientSocket] write(data, len) Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
            assert( false );
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

    //    assert( isOpen() );
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocket::read( unsigned char* data, size_t len ) const {
    assert( isOpen() );
    assert( isConnected() );

    size_t downloadSize = 0;
    do {
        int byteRead =
            recv( m_fdSock, (char*)data + downloadSize, static_cast<int>( len - downloadSize ), 0 );
        if ( byteRead == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
#endif
            perror( "[socket] receive failed.\n" );
            assert( !isConnected() );
            close();
            throw Socket::exception(
                "[ClientSocket] read(data, len) Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            assert( isConnected() );
            close();
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

void ClientSocket::close() const {
    assert( isOpen() );
    assert( isConnected() );
    //    clear();
    //            assert(m_fdSock != INVALID_SOCKET);
        clearSocket(m_fdSock);
//    ::close( m_fdSock );
//    m_fdSock    = INVALID_SOCKET;
    m_connected = false;
    assert( !isOpen() );
}

bool ClientSocket::isOpen() const {
    //    assert( isConnected() );
    //    return m_fdSock != INVALID_SOCKET;
    return m_connected;
}

bool ClientSocket::isEnd() const {
    assert( isOpen() );
    assert( isConnected() );
    return false;
}

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

// void ClientSocket::clear() const {
//     if ( m_fdSock != INVALID_SOCKET ) {
//#ifdef DEBUG_SOCKET
//         DEBUG_MSG( getHeader( m_fdSock ) << "close socket" );
//#endif
//         net::clearSocket( m_fdSock );
//         m_fdSock = INVALID_SOCKET;
//     }
// }

const int& ClientSocket::getPort() const {
    return m_port;
}

const std::string& ClientSocket::getIpv4() const {
    return m_ipv4;
}

void ClientSocket::setPort( int newPort ) {
    assert( !isOpen() );
    assert( 0 <= newPort && newPort <= 65535 );
    m_port                   = newPort;
    m_serverAddress.sin_port = htons( m_port ); // Server port

    //    initSocket();
    //    memset( &m_serverAddress, 0, sizeof( m_serverAddress ) );
    //    m_serverAddress.sin_family = AF_INET;
    //    // m_serverAddress.sin_addr.s_addr = inet_addr(m_ipv4.c_str()); // winsock 1.0
    //    inet_pton( AF_INET, m_ipv4.c_str(), &m_serverAddress.sin_addr.s_addr ); // winsock 2.0
    //    m_serverAddress.sin_port = htons( m_port );                             // Server port
}

void ClientSocket::setIpv4( const std::string& newIpv4 ) {
    assert( !isOpen() );
    assert( std::regex_match( newIpv4, m_ipv4Regex ) );
    m_ipv4 = newIpv4;
    inet_pton( AF_INET, m_ipv4.c_str(), &m_serverAddress.sin_addr.s_addr ); // winsock 2.0
}

} // namespace net
} // namespace hub

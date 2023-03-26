#include "net/ClientSocket.hpp"

#include <cassert>
#include <cstring>
#include <regex>
#include <thread>

namespace hub {
namespace net {

// static const std::regex m_ipv4Regex { "^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$" };

ClientSocket::ClientSocket() : m_ipv4( s_defaultServiceIp ), m_port( s_defaultServicePort ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket()" );
#endif

    initServerAddress();
    connect();
}

ClientSocket::ClientSocket( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(std::string ipv4, int port)" );
#endif

    initServerAddress();

    if ( autoConnect ) connect();
}

ClientSocket::ClientSocket( net::utils::socket_fd fdSock ) {
    m_port       = -1;
    m_fdSock     = fdSock;
    m_serverSide = true;
    m_connected  = true;
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocket(socket_fd fdSock)" );
#endif
}

ClientSocket::ClientSocket( ClientSocket&& sock ) :
    m_ipv4( sock.m_ipv4 ),
    m_port( sock.m_port ),
    m_addr( std::move( sock.m_addr ) ),
    m_connected( sock.m_connected ) {

    m_fdSock      = sock.m_fdSock;
    m_serverSide  = sock.m_serverSide;
    sock.m_fdSock = net::utils::invalidSocket();
    sock.m_moved  = true;
}

// ClientSocket::ClientSocket(ClientSocket &&sock) :
//{
//}

ClientSocket::~ClientSocket() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~ClientSocket()" );
#endif
    if ( !m_moved ) {
//        std::cout << "[ClientSocket] ~ClientSocket() started" << std::endl;
        assert(! ClientSocket::isOpen());
//        if ( ClientSocket::isOpen() ) ClientSocket::close();
//        assert( !ClientSocket::isOpen() );
//        std::cout << "[ClientSocket] ~ClientSocket() ended" << std::endl;
    }
}

void ClientSocket::initServerAddress() {
    assert( utils::isValid( m_ipv4 ) );
    assert( utils::isValid( m_port ) );

    // Server address construction

    m_addr.init( m_ipv4, m_port );
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

    assert( !net::utils::isValid( m_fdSock ) );

    // Socket creation
    m_fdSock = net::utils::clientSocket();
    if ( !net::utils::isValid( m_fdSock ) ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }

    assert( !isOpen() );
    assert( isConnected() );

    // Connect to server
    if ( net::utils::connect( m_fdSock, m_addr ) < 0 ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
#endif
        net::utils::closeSocket( m_fdSock );
        throw Socket::exception(
            ( ( std::string( "[ClientSocket] connect() Failed to connect to server at address " ) +
                m_ipv4 + " and port " + std::to_string( m_port ) ) )
                .c_str() );
    }

    m_connected = true;
    assert( isOpen() );
    assert( isConnected() );

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocket] connected to the server, starting communication" );
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << m_fdSock );
#endif
}

void ClientSocket::write( const unsigned char* data, size_t len ) {
    if ( !isOpen() ) {
        throw Socket::exception(
            "[ClientSocket] write(data, len) Unable to write because output is closed" );
    }
    assert( isOpen() );

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
            throw Socket::exception(
                "[ClientSocket] write(data, len) Can't write packet, not connected" );
        }
        // winsock const char * data
        // winsock int len
        int byteSent;
        try {
            byteSent = net::utils::send( m_fdSock,
                                         reinterpret_cast<const char*>( data ) + uploadSize,
                                         static_cast<int>( len - uploadSize ),
                                         0 );
        }
        catch ( std::exception& e ) {
            std::cout << "[ClientSocket] catch exception : " << e.what() << std::endl;
            assert( false );
            throw;
        }

        if ( byteSent == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "can't send packet " << byteSent << "/" << len );
            perror( "[socket] send failed.\n" );
#endif
            //            assert( isConnected() );
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
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocket::read( unsigned char* data, size_t len ) {
    assert( isOpen() );
    assert( isConnected() );

    size_t downloadSize = 0;
    do {
        int byteRead = net::utils::recv( m_fdSock,
                                         reinterpret_cast<char*>( data ) + downloadSize,
                                         static_cast<int>( len - downloadSize ),
                                         0 );
        if ( byteRead == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
            perror( "[socket] receive failed.\n" );
#endif
#ifndef WIN32
#endif
            if ( isOpen() ) close();
            throw Socket::exception(
                "[ClientSocket] read(data, len) Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            //            assert( isConnected() );
            if ( isConnected() ) {
                if ( isOpen() ) close();
                throw Socket::exception(
                    "[ClientSocket] read(data, len) 0 byte received, peer connection lost" );
            }
            else {
                throw Socket::exception( "[ClientSocket] read(data, len) 0 byte received, "
                                         "connection already broken by write operation" );
            }
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
    //    if ( !isOpen() ) { throw Socket::exception( "unable to close non openned socket" ); }
    //    else {
    assert( isOpen() );
    //    if (isOpen()) {
    net::utils::closeSocket( m_fdSock );
    m_connected = false;
    //    }
    assert( !isOpen() );
    //    }
}

bool ClientSocket::isOpen() const {
    return m_connected;
}

bool ClientSocket::isEnd() const {
    assert( isOpen() );
    assert( isConnected() );
    return false;
}

#if ( __cplusplus >= 201703L )
#    define ConstString constexpr std::string_view
// using ConstString = constexpr std::string_view;
#else
using ConstString = const std::string;
#endif

static ConstString type2string[static_cast<int>( ClientSocket::Type::COUNT )] = {
    "NONE",
    "STREAMER",
    "VIEWER",
    "STREAM_VIEWER",
    "ASKER",
};
std::ostream& operator<<( std::ostream& os, const ClientSocket::Type& type ) {
    os << type2string[(int)type];
    return os;
}

static ConstString message2string[static_cast<int>( ClientSocket::Message::COUNT )] = {
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
    "NEW_ACQ",
    "LIST_STREAMS",
    "GET_SENSOR_SPEC",
    "GET_ACQUISITION",
};
std::ostream& operator<<( std::ostream& os, const ClientSocket::Message& msg ) {
    os << message2string[(int)msg];
    return os;
}

// void ClientSocket::clear() const {
// #ifdef DEBUG_SOCKET
// #endif
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
    m_port = newPort;
    m_addr.setPort( m_port );
}

void ClientSocket::setIpv4( const std::string& newIpv4 ) {
    assert( !isOpen() );
    assert( utils::isValid( newIpv4 ) );
    m_ipv4 = newIpv4;
    m_addr.setIpv4( m_ipv4 );
}

} // namespace net
} // namespace hub

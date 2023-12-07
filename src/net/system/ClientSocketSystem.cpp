#include "ClientSocketSystem.hpp"

#include <cmath>
#include <cstring>
#include <regex>
#include <thread>

namespace hub {
namespace net {
namespace system {

// ClientSocketSystem::ClientSocketSystem() : m_ipv4( s_defaultServiceIp ), m_port(
// s_defaultServicePort ) { #ifdef HUB_DEBUG_SOCKET
//     DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocketSystem()" );
// #endif

//    initServerAddress();
//    connect();
//}

ClientSocketSystem::ClientSocketSystem( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ) {
    // #ifdef HUB_DEBUG_SOCKET
    //     DEBUG_MSG( getHeader(  ) << "ClientSocket(std::string ipv4, int port)" );
    // #endif

    initServerAddress();

    if ( autoConnect ) connect();
}

//    std::string m_ipv4;
//    int m_port;
//    utils::ClientAddr m_addr;
//    bool m_connected = false;
//    bool m_moved     = false;
ClientSocketSystem::ClientSocketSystem( utils::socket_fd fdSock, utils::ClientAddr clientAddr ) :
    m_ipv4( clientAddr.getIpv4() ),
    m_port( clientAddr.getPort() ),
    m_addr( std::move( clientAddr ) ),
    m_connected( true ) {
    //    m_port       = -1;
    m_fdSock = fdSock;
    //    m_ipv4 = m_addr.getIpv4();
    //    m_addr(std::move(clientAddr)),
    m_serverSide = true;
//    m_connected  = true;
#ifdef HUB_DEBUG_SOCKET
    //    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocketSystem(socket_fd fdSock)" );
    DEBUG_MSG( getHeader() << "accepting new socket:" << toString() );
#endif
    //    DEBUG_MSG( "socket " << m_fdSock << ": accepting new socket:" << toString() );
}

// ClientSocketSystem::ClientSocketSystem( ClientSocketSystem&& sock ) :
//     m_ipv4( sock.m_ipv4 ),
//     m_port( sock.m_port ),
//     m_addr( std::move( sock.m_addr ) ),
//     m_connected( sock.m_connected ) {

//     m_fdSock         = sock.m_fdSock;
//     m_serverSide     = sock.m_serverSide;
//     sock.m_fdSock    = utils::invalidSocket();
//     sock.m_moved     = true;
//     sock.m_connected = false;
// }

// ClientSocketSysteClientSocketSystem::ClientSocketSystem(ClientSocketSystem &&sock) :

// ClientSocketSystem::~ClientSocketSystem() {
// #ifdef HUB_DEBUG_SOCKET
//     DEBUG_MSG( getHeader(  ) << "~ClientSocket()" );
// #endif
//     if ( !m_moved ) {
//         assert( !ClientSocketSystem::isConnected() );

// // //        if ( ClientSocketSystem::isOpen() ) {
// //         if ( ClientSocketSystem::isConnected() ) {
// // //            ClientSocketSystem::close();
// //             disconnect();
// //         }
// // //        assert( !ClientSocketSystem::isOpen() );
// //         assert( !ClientSocketSystem::isConnected() );
//     }
// }

/////////////////////////////////////////////////////////////////////////////////////////

void ClientSocketSystem::setIpv4( const std::string& newIpv4 ) {
    //    assert( !isOpen() );
    assert( !isConnected() );
    assert( utils::isValid( newIpv4 ) );
    m_ipv4 = newIpv4;
    m_addr.setIpv4( m_ipv4 );
}

const std::string& ClientSocketSystem::getIpv4() const {
    return m_ipv4;
}

void ClientSocketSystem::setPort( int newPort ) {
    //    assert( !isOpen() );
    assert( !isConnected() );
    assert( 0 <= newPort && newPort <= 65535 );
    m_port = newPort;
    m_addr.setPort( m_port );
}

int ClientSocketSystem::getPort() const {
    return m_port;
}

void ClientSocketSystem::connect() {
    assert( !m_connected );
    //    assert( !isOpen() );
    assert( !isConnected() );

#ifdef HUB_DEBUG_SOCKET
    // DEBUG_MSG( "[ClientSocket] connectToServer" );
    DEBUG_MSG( getHeader() << "[ClientSocket] ClientSocket('" << m_ipv4 << ", " << m_port << ")" );
#endif

    assert( !utils::isValid( m_fdSock ) );

    // SocketSystem creation
    m_fdSock = utils::clientSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "[ClientSocket] client socket inited" );
    // std::cout << "fdSock = " << m_fdSock << std::endl;
#endif

    //    assert( !isOpen() );
    // assert( !isConnected() );
    assert( isConnected() );
    // assert( SocketSystem::isConnected() );

    // Connect to server
    if ( utils::connect( m_fdSock, m_addr ) < 0 ) {
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
#endif
        utils::closeSocket( m_fdSock );
        throw SocketSystem::exception(
            ( ( std::string( "[ClientSocket] connect() Failed to connect to server at address " ) +
                m_ipv4 + " and port " + std::to_string( m_port ) ) )
                .c_str() );
    }

    m_connected = true;
    //    std::cout << "[ClientSocket] connect() " << std::endl;
    //    assert( isOpen() );
    assert( isConnected() );
    // assert( SocketSystem::isConnected() );

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "[ClientSocket] connected to the server, starting communication" );
    DEBUG_MSG( getHeader() << "new client on socket " << m_fdSock );
#endif
}

bool ClientSocketSystem::isConnected() const {
    return SocketSystem::isConnected();
    // return m_connected;
}

void ClientSocketSystem::disconnect() {
    // assert( isOpen() ); // todo fix
    //    if ( isOpen() ) {
    if ( isConnected() ) {
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( getHeader() << "disconnecting socket ..." );
#endif
        utils::closeSocket( m_fdSock );
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( getHeader() << "socket disconnected" );
#endif
        m_connected = false;
    }
    assert( !isConnected() );
    //    assert( !isOpen() );
    //    close();
}

void ClientSocketSystem::write( const unsigned char* data, const size_t size ) {
    // assert( isConnected() );
    //    assert( isOpen() );
    //    assert( 0 < size && size <= MAX_NET_BUFFER_SIZE );
    assert( 0 < size );

    //    unsigned char * tmp = new unsigned char[size];
    //    memcpy(tmp, data, size);

#ifdef HUB_DEBUG_SOCKET_OUTPUT
    DEBUG_MSG( getHeader() << "write(unsigned char*, size = " << size << ") " );
#endif

    //    const int nPart = std::ceil( (float)size / MAX_NET_BUFFER_SIZE );

    //    for ( int iPart = 0; iPart < nPart; ++iPart ) {

    //        const size_t offsetPart = iPart * MAX_NET_BUFFER_SIZE;
    //    const size_t offsetPart = 0;
    //        const size_t sizePart = (iPart == nPart - 1) ?(size - offsetPart)
    //        :(MAX_NET_BUFFER_SIZE);
    //    const size_t sizePart = size;

    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG( getHeader( m_fdSock )
    //                   << "iPart = " << iPart << "/" << nPart << ", offsetPart: " << offsetPart <<
    //                   ", sizePart:" << sizePart );
    // #endif

    size_t uploadSize = 0;
    do {
        if ( !isConnected() ) {
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG(
                getHeader()
                << "write(const unsigned char* data, size_t size) : isConnected() client lost" );
#endif
            //            close();
            // disconnect();
            throw SocketSystem::exception(
                "[ClientSocket] write(data, size) Can't write packet, peer not connected" );
        }
        // winsock const char * data
        // winsock int size
        int64_t byteSent;
        try {
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG( getHeader() << "sending bytes = " << size - uploadSize );
#endif
            //                assert(static_cast<size_t>(size - uploadSize) > 0);
            //                assert(static_cast<size_t>(size - uploadSize) <= size);
            assert( 0 < size - uploadSize && size - uploadSize <= size );
            byteSent = utils::send( m_fdSock,
                                    reinterpret_cast<const char*>( data + uploadSize ),
                                    size - uploadSize,
                                    0 );
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG( getHeader() << "sended bytes = " << byteSent );
#endif
            assert( -1 <= byteSent && byteSent <= size );
        }
        catch ( std::exception& e ) {
            std::cout << "[ClientSocket] catch exception : " << e.what() << std::endl;
            assert( false );
            throw e;
        }

        if ( byteSent == -1 ) {
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( getHeader() << "can't send packet " << byteSent << "/" << size );
            perror( "[socket] send failed.\n" );
#endif
            //            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocket] write(data, size) Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
            assert( false );
            exit( 1 );
//            close();
//            throw SocketSystem::exception(
//                "[ClientSocket] write(data, size) Can't write packet, buffer overflow" );
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( "byteSent == 0, sleep" );
#endif
        }
        uploadSize += byteSent;
#ifdef HUB_DEBUG_SOCKET_OUTPUT
        DEBUG_MSG( getHeader() << "byteSent = " << byteSent << " (" << uploadSize << "/" << size
                               << ")" );
#endif
    } while ( size != uploadSize );

    //    } // end for ( int iPart = 0; iPart < nPart; ++iPart )

    //        assert(memcmp(tmp, data, size) == 0);
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocketSystem::read( unsigned char* data, const size_t size ) {
    //    assert( isOpen() );
    assert( isConnected() );
    // assert( SocketSystem::isConnected() );

    size_t downloadSize = 0;
    do {
        assert( 0 <= size - downloadSize && size - downloadSize <= size );
        int64_t byteRead = utils::recv(
            m_fdSock, reinterpret_cast<char*>( data ) + downloadSize, size - downloadSize, 0 );
        // assert( -1 <= byteRead && byteRead <= (int64_t)size );
        assert( (int64_t)size > 0 );
        assert( -1 <= byteRead && byteRead <= (int64_t)size );

        if ( byteRead == -1 ) {
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
            perror( "[socket] receive failed.\n" );
#endif
#ifndef WIN32
#endif
            //            close();
            disconnect();
            throw SocketSystem::exception( "[ClientSocketSystem] read(data, size) "
                                           "Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            //            std::cout << "no bytes read, sleep" << std::endl;
            //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            //            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocket] read(data, size) 0 byte received, peer connection lost" );
        }

        downloadSize += byteRead;
#ifdef HUB_DEBUG_SOCKET_INPUT
        DEBUG_MSG( getHeader() << "byteRead = " << byteRead << " (" << downloadSize << "/" << size
                               << ")" );
#endif
    } while ( size != downloadSize );

    // #ifdef HUB_DEBUG_SOCKET
    //
    // #endif
}

////////////////////////////////////////////////////////////////

void ClientSocketSystem::initServerAddress() {
    assert( utils::isValid( m_ipv4 ) );
    assert( utils::isValid( m_port ) );

    m_addr.init( m_ipv4, m_port );
}

// bool ClientSocketSystem::isOpen() const {
// bool ClientSocketSystem::isOpen() const {
//     return m_connected;
// }

// void ClientSocketSystem::close() {
//     // assert( isOpen() ); // todo fix
//     if ( isOpen() ) {
//         utils::closeSocket( m_fdSock );
//         m_connected = false;
//     }
//     assert( !isOpen() );
// }

// bool ClientSocketSystem::isEnd() const {
//     assert( isOpen() );
//     assert( isConnected() );
//     return false;
// }

// #if ( __cplusplus >= 201703L )
// #    define ConstString constexpr std::string_view
//// using ConstString = constexpr std::string_view;
// #else
// using ConstString = const std::string;
// #endif

// static ConstString type2string[static_cast<int>( ClientSocketSystem::Type::COUNT )] = {
//     "NONE",
//     "STREAMER",
//     "VIEWER",
//     "STREAM_VIEWER",
//     "ASKER",
// };
// std::ostream& operator<<( std::ostream& os, const ClientSocketSystem::Type& type ) {
//     os << type2string[(int)type];
//     return os;
// }

// static ConstString message2string[static_cast<int>(
// ClientSocketSysteClientSocketSystem::Message::COUNT )] = {
//     "NONE",
//     "PING",
//     "SYNC",
//     "DATA",
//     "OK",
//     "CLOSE",
//     "DEL_STREAMER",
//     "NEW_STREAMER",
//     "NOT_FOUND",
//     "FOUND",
//     "NEW_ACQ",
//     "LIST_STREAMS",
//     "GET_SENSOR_SPEC",
//     "GET_ACQUISITION",
// };
// std::ostream& operator<<( std::ostream& os, const ClientSocketSystem::Message& msg ) {
//     os << message2string[(int)msg];
//     return os;
// }

// void ClientSocketSystem::clear() const {
// #ifdef HUB_DEBUG_SOCKET
// #endif
// }

} // namespace system
} // namespace net
} // namespace hub

#include "ClientSocketSystem.hpp"

#include <cmath>
#include <cstring>
#include <regex>
#include <thread>

namespace hub {
namespace net {
namespace system {

// ClientSocketSystem::ClientSocketSystem() : m_ipv4( s_defaultServiceIp ), m_port( s_defaultServicePort ) {
// #ifdef DEBUG_SOCKET
//     DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocketSystem()" );
// #endif

//    initServerAddress();
//    connect();
//}

ClientSocketSystem::ClientSocketSystem( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ) {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocketSystem(std::string ipv4, int port)" );
#endif

    initServerAddress();

    if ( autoConnect ) connect();
}

ClientSocketSystem::ClientSocketSystem( utils::socket_fd fdSock ) {
    m_port       = -1;
    m_fdSock     = fdSock;
    m_serverSide = true;
    m_connected  = true;
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "ClientSocketSystem(socket_fd fdSock)" );
#endif
}

ClientSocketSystem::ClientSocketSystem( ClientSocketSystem&& sock ) :
    m_ipv4( sock.m_ipv4 ),
    m_port( sock.m_port ),
    m_addr( std::move( sock.m_addr ) ),
    m_connected( sock.m_connected ) {

    m_fdSock         = sock.m_fdSock;
    m_serverSide     = sock.m_serverSide;
    sock.m_fdSock    = utils::invalidSocket();
    sock.m_moved     = true;
    sock.m_connected = false;
}

// ClientSocketSysteClientSocketSystem::ClientSocketSystem(ClientSocketSystem &&sock) :

ClientSocketSystem::~ClientSocketSystem() {
#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "~ClientSocketSystem()" );
#endif
    if ( !m_moved ) {
//        if ( ClientSocketSystem::isOpen() ) {
        if ( ClientSocketSystem::isConnected() ) {
//            ClientSocketSystem::close();
            disconnect();
        }
//        assert( !ClientSocketSystem::isOpen() );
        assert( !ClientSocketSystem::isConnected() );
    }
}

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

#ifdef DEBUG_SOCKET
    DEBUG_MSG( "[ClientSocketSystem] connectToServer" );
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocketSystem] ClientSocketSystem('" << m_ipv4 << ", " << m_port << ")" );
#endif

    assert( !utils::isValid( m_fdSock ) );

    // SocketSystem creation
    m_fdSock = utils::clientSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }

//    assert( !isOpen() );
    assert( !isConnected() );
    assert( SocketSystem::isConnected() );

    // Connect to server
    if ( utils::connect( m_fdSock, m_addr ) < 0 ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
#endif
        utils::closeSocket( m_fdSock );
        throw SocketSystem::exception(
            ( ( std::string( "[ClientSocketSystem] connect() Failed to connect to server at address " ) +
                m_ipv4 + " and port " + std::to_string( m_port ) ) )
                .c_str() );
    }

    m_connected = true;
//    std::cout << "[ClientSocketSystem] connect() " << std::endl;
//    assert( isOpen() );
    assert( isConnected() );
    assert( SocketSystem::isConnected() );

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock )
               << "[ClientSocketSysteClientSocketSystem] connected to the server, starting communication" );
    DEBUG_MSG( getHeader( m_fdSock ) << "new client on socket " << m_fdSock );
#endif
}

bool ClientSocketSystem::isConnected() const
{
    return m_connected;
}


void ClientSocketSystem::disconnect()
{
    // assert( isOpen() ); // todo fix
//    if ( isOpen() ) {
    if ( isConnected() ) {
        utils::closeSocket( m_fdSock );
        m_connected = false;
    }
    assert( !isConnected() );
//    assert( !isOpen() );
//    close();
}

void ClientSocketSystem::write( const unsigned char* data, const size_t len ) {
    assert( isConnected() );
//    assert( isOpen() );
    //    assert( 0 < len && len <= MAX_NET_BUFFER_SIZE );
    assert( 0 < len );

    //    unsigned char * tmp = new unsigned char[len];
    //    memcpy(tmp, data, len);

#ifdef DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_fdSock ) << "write(unsigned char*, len = " << len << ") " );
#endif

    //    const int nPart = std::ceil( (float)len / MAX_NET_BUFFER_SIZE );

    //    for ( int iPart = 0; iPart < nPart; ++iPart ) {

    //        const size_t offsetPart = iPart * MAX_NET_BUFFER_SIZE;
    //    const size_t offsetPart = 0;
    //        const size_t sizePart = (iPart == nPart - 1) ?(len - offsetPart)
    //        :(MAX_NET_BUFFER_SIZE);
    //    const size_t sizePart = len;

#ifdef DEBUG_SOCKET
//            DEBUG_MSG( getHeader( m_fdSock )
//                   << "iPart = " << iPart << "/" << nPart << ", offsetPart: " << offsetPart << ",
//                   sizePart:" << sizePart );
#endif

    size_t uploadSize = 0;
    do {
        if ( !isConnected() ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG(
                getHeader( m_fdSock )
                << "write(const unsigned char* data, size_t len) : isConnected() client lost" );
#endif
//            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocketSystem] write(data, len) Can't write packet, not connected" );
        }
        // winsock const char * data
        // winsock int len
        int64_t byteSent;
        try {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "sending bytes = " << len - uploadSize );
#endif
            //                assert(static_cast<size_t>(len - uploadSize) > 0);
            //                assert(static_cast<size_t>(len - uploadSize) <= len);
            assert( 0 < len - uploadSize && len - uploadSize <= len );
            byteSent = utils::send(
                m_fdSock, reinterpret_cast<const char*>( data + uploadSize ), len - uploadSize, 0 );
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "sended bytes = " << byteSent );
#endif
            assert( -1 <= byteSent && byteSent <= len );
        }
        catch ( std::exception& e ) {
            std::cout << "[ClientSocketSystem] catch exception : " << e.what() << std::endl;
            assert( false );
            throw e;
        }

        if ( byteSent == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( getHeader( m_fdSock ) << "can't send packet " << byteSent << "/" << len );
            perror( "[socket] send failed.\n" );
#endif
//            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocketSystem] write(data, len) Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
            assert( false );
            exit( 1 );
//            close();
//            throw SocketSystem::exception(
//                "[ClientSocketSystem] write(data, len) Can't write packet, buffer overflow" );
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byteSent == 0, sleep" );
#endif
        }
        uploadSize += byteSent;
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock )
                   << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" );
#endif
    } while ( len != uploadSize );

    //    } // end for ( int iPart = 0; iPart < nPart; ++iPart )

    //        assert(memcmp(tmp, data, len) == 0);
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocketSystem::read( unsigned char* data, const size_t len ) {
//    assert( isOpen() );
    assert( isConnected() );
    assert( SocketSystem::isConnected() );

    size_t downloadSize = 0;
    do {
        assert( 0 <= len - downloadSize && len - downloadSize <= len );
        int64_t byteRead = utils::recv(
            m_fdSock, reinterpret_cast<char*>( data ) + downloadSize, len - downloadSize, 0 );
        //assert( -1 <= byteRead && byteRead <= (int64_t)len );
        assert( (int64_t)len > 0 );
        assert( -1 <= byteRead && byteRead <= (int64_t)len );

        if ( byteRead == -1 ) {
#ifdef DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
            perror( "[socket] receive failed.\n" );
#endif
#ifndef WIN32
#endif
//            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocketSysteClientSocketSystem] read(data, len) Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
            //            std::cout << "no bytes read, sleep" << std::endl;
            //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            close();
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocketSystem] read(data, len) 0 byte received, peer connection lost" );
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


////////////////////////////////////////////////////////////////

void ClientSocketSystem::initServerAddress() {
    assert( utils::isValid( m_ipv4 ) );
    assert( utils::isValid( m_port ) );

    m_addr.init( m_ipv4, m_port );
}

//bool ClientSocketSystem::isOpen() const {
//bool ClientSocketSystem::isOpen() const {
//    return m_connected;
//}

//void ClientSocketSystem::close() {
//    // assert( isOpen() ); // todo fix
//    if ( isOpen() ) {
//        utils::closeSocket( m_fdSock );
//        m_connected = false;
//    }
//    assert( !isOpen() );
//}



//bool ClientSocketSystem::isEnd() const {
//    assert( isOpen() );
//    assert( isConnected() );
//    return false;
//}

//#if ( __cplusplus >= 201703L )
//#    define ConstString constexpr std::string_view
//// using ConstString = constexpr std::string_view;
//#else
//using ConstString = const std::string;
//#endif

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

// static ConstString message2string[static_cast<int>( ClientSocketSysteClientSocketSystem::Message::COUNT )] = {
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
// #ifdef DEBUG_SOCKET
// #endif
// }


} // namespace system
} // namespace net
} // namespace hub

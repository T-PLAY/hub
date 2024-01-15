#include "ClientSocketBoost.hpp"

#ifdef	HUB_USE_BOOST

#include <cmath>
#include <cstring>
#include <regex>
#include <thread>

//#define HUB_DEBUG_SOCKET

namespace hub {
namespace net {
namespace boost {

#ifdef HUB_DEBUG_SOCKET
std::string getHeader( ::boost::asio::ip::tcp::socket& iSock ) {
    std::string str;
    //    if ( m_serverSide ) { str += "\t\033[1m[Server]\033[0m"; }
    //    else {
    //        str += "[Client]";
    //    }
    str += "\033[" + std::to_string( 31 + (int)iSock.native_handle() % 7 ) +
           "m[socket:" + std::to_string( (int)iSock.native_handle() ) + "]\033[0m ";
    return str;
}
#endif

// ClientSocketBoost::ClientSocketBoost() : m_ipv4( s_defaultServiceIp ), m_port(
// s_defaultServicePort ) { #ifdef HUB_DEBUG_SOCKET
//     DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost()" );
// #endif

//    initServerAddress();
//    connect();
//}

ClientSocketBoost::ClientSocketBoost( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ), m_ioService(), m_sock( m_ioService ) {
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost(std::string ipv4, int port)" );
#endif
    //    initServerAddress();
    if ( autoConnect ) connect();
}

ClientSocketBoost::ClientSocketBoost( ::boost::asio::ip::tcp::socket&& fdSock ) :
    m_sock( std::move( fdSock ) ) {
    m_port = -1;
//    m_sock     = fdSock;
//    m_serverSide = true;
//    m_connected  = true;
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost(socket_fd fdSock)" );
#endif
}

// ClientSocketBoost::ClientSocketBoost( utils::socket_fd fdSock ) {
//     m_port       = -1;
//     m_sock     = fdSock;
//     m_serverSide = true;
//     m_connected  = true;
// #ifdef HUB_DEBUG_SOCKET
//     DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost(socket_fd fdSock)" );
// #endif
// }

ClientSocketBoost::ClientSocketBoost( ClientSocketBoost&& sock ) :
    m_ipv4( sock.m_ipv4 ),
    m_port( sock.m_port ),
    m_ioService(),
    m_sock( std::move( sock.m_sock ) )
//    m_addr( std::move( sock.m_addr ) ),
//    m_connected( sock.m_connected )
{
    //    m_sock         = sock.m_sock;
    //    m_serverSide     = sock.m_serverSide;
    //    sock.m_sock    = utils::invalidSocket();
    sock.m_moved = true;
    //    sock.m_connected = false;
}

// ClientSocketSysteClientSocketSystem::ClientSocketBoost(ClientSocketBoost &&sock) :

ClientSocketBoost::~ClientSocketBoost() {
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "~ClientSocketBoost()" );
#endif
    if ( !m_moved ) {
        //        if ( ClientSocketBoost::isOpen() ) {
        if ( ClientSocketBoost::isConnected() ) {
            //            ClientSocketBoost::close();
            disconnect();
        }
        //        assert( !ClientSocketBoosClientSocketBoost::isOpen() );
        assert( !ClientSocketBoost::isConnected() );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void ClientSocketBoost::setIpv4( const std::string& newIpv4 ) {
    //    assert( !isOpen() );
    assert( !isConnected() );
    //    assert( utils::isValid( newIpv4 ) );
    m_ipv4 = newIpv4;
    //    m_addr.setIpv4( m_ipv4 );
}

const std::string & ClientSocketBoost::getIpv4() const {
    return m_ipv4;
}

void ClientSocketBoost::setPort( int newPort ) {
    //    assert( !isOpen() );
    assert( !isConnected() );
    assert( 0 <= newPort && newPort <= 65535 );
    m_port = newPort;
    //    m_addr.setPort( m_port );
}

int ClientSocketBoost::getPort() const {
    return m_port;
}

void ClientSocketBoost::connect() {
    //    assert( !m_connected );
    ////    assert( !isOpen() );
    //    assert( !isConnected() );

    // #ifdef HUB_DEBUG_SOCKET
    //     DEBUG_MSG( "[ClientSocketBoost] connectToServer" );
    //     DEBUG_MSG( getHeader( m_sock )
    //                << "[ClientSocketBoost] ClientSocketBoost('" << m_ipv4 << ", " << m_port <<
    //                ")" );
    // #endif

    m_sock.connect( ::boost::asio::ip::tcp::endpoint(
        ::boost::asio::ip::address::from_string( m_ipv4 ), m_port ) );

    assert( m_sock.is_open() );

    //    assert( !utils::isValid( m_sock ) );

    //    // SocketSystem creation
    //    m_sock = utils::clientSocket();
    //    if ( !utils::isValid( m_sock ) ) {
    //        perror( "[socket] socket creation failed.\n" );
    //        return;
    //    }

    ////    assert( !isOpen() );
    //    assert( !isConnected() );
    //    assert( SocketSystem::isConnected() );

    //    // Connect to server
    //    if ( utils::connect( m_sock, m_addr ) < 0 ) {
    // #ifdef HUB_DEBUG_SOCKET
    //        DEBUG_MSG( "[ClienSocket] failed to connect to server ########################" );
    // #endif
    //        utils::closeSocket( m_sock );
    //        throw SocketSystem::exception(
    //            ( ( std::string( "[ClientSocketBoost] connect() Failed to connect to server at
    //            address " ) +
    //                m_ipv4 + " and port " + std::to_string( m_port ) ) )
    //                .c_str() );
    //    }

    //    m_connected = true;
    //    std::cout << "[ClientSocketBoosClientSocketBoost] connect() " << std::endl;
    ////    assert( isOpen() );
    //    assert( isConnected() );
    //    assert( SocketSystem::isConnected() );

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "[ClientSocketSysteClientSocketSystem] connected to the "
                                      "server, starting communication" );
    DEBUG_MSG( getHeader( m_sock ) << "new client on socket " << m_sock.native_handle() );
#endif
}

bool ClientSocketBoost::isConnected() const {
    return !m_moved && m_sock.is_open();
    //    return m_sock.is_open();
    //    return m_connected;
}

void ClientSocketBoost::disconnect() {
    assert( m_sock.is_open() );

    ::boost::system::error_code ec;
    //    m_sock.shutdown(::boost::asio::ip::tcp::socket::shutdown_send, ec);
    //    if (ec) {
    //        assert(false);
    //    }

    m_sock.close( ec );
    if ( ec ) { assert( false ); }
    assert( !m_sock.is_open() );

    //    m_sock.shutdown()
    // assert( isOpen() ); // todo fix
    //    if ( isOpen() ) {
    //    if ( isConnected() ) {
    //        utils::closeSocket( m_sock );
    //        m_connected = false;
    //    }
    //    assert( !isConnected() );
    //    assert( !isOpen() );
    //    close();
}

void ClientSocketBoost::write( const unsigned char* data, const size_t size ) {
    //    assert( isConnected() );
    ////    assert( isOpen() );
    //    //    assert( 0 < len && len <= MAX_NET_BUFFER_SIZE );
    //    assert( 0 < len );

    const auto& buffer = ::boost::asio::buffer( data, size );

    auto uploadBytes = ::boost::asio::write( m_sock, buffer );
    assert( uploadBytes == size );

//    std::cout << "[ClientSocketBoost] write() write data : [";
//    for ( int j = 0; j < size; ++j ) {
//        std::cout << data[j] << " ";
//    }
//    std::cout << "]" << std::endl;

    //    //    unsigned char * tmp = new unsigned char[len];
    //    //    memcpy(tmp, data, len);

    // #ifdef HUB_DEBUG_SOCKET
    //     DEBUG_MSG( getHeader( m_sock ) << "write(unsigned char*, len = " << len << ") " );
    // #endif

    //    //    const int nPart = std::ceil( (float)len / MAX_NET_BUFFER_SIZE );

    //    //    for ( int iPart = 0; iPart < nPart; ++iPart ) {

    //    //        const size_t offsetPart = iPart * MAX_NET_BUFFER_SIZE;
    //    //    const size_t offsetPart = 0;
    //    //        const size_t sizePart = (iPart == nPart - 1) ?(len - offsetPart)
    //    //        :(MAX_NET_BUFFER_SIZE);
    //    //    const size_t sizePart = len;

    // #ifdef HUB_DEBUG_SOCKET
    ////            DEBUG_MSG( getHeader( m_sock )
    ////                   << "iPart = " << iPart << "/" << nPart << ", offsetPart: " << offsetPart
    ///<< ", /                   sizePart:" << sizePart );
    // #endif

    //    size_t uploadSize = 0;
    //    do {
    //        if ( !isConnected() ) {
    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG(
    //                getHeader( m_sock )
    //                << "write(const unsigned char* data, size_t len) : isConnected() client lost"
    //                );
    // #endif
    ////            close();
    //            disconnect();
    //            throw SocketSystem::exception(
    //                "[ClientSocketBoost] write(data, len) Can't write packet, not connected" );
    //        }
    //        // winsock const char * data
    //        // winsock int len
    //        int64_t byteSent;
    //        try {
    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG( getHeader( m_sock ) << "sending bytes = " << len - uploadSize );
    // #endif
    //            //                assert(static_cast<size_t>(len - uploadSize) > 0);
    //            //                assert(static_cast<size_t>(len - uploadSize) <= len);
    //            assert( 0 < len - uploadSize && len - uploadSize <= len );
    //            byteSent = utils::send(
    //                m_sock, reinterpret_cast<const char*>( data + uploadSize ), len - uploadSize,
    //                0 );
    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG( getHeader( m_sock ) << "sended bytes = " << byteSent );
    // #endif
    //            assert( -1 <= byteSent && byteSent <= len );
    //        }
    //        catch ( std::exception& e ) {
    //            std::cout << "[ClientSocketBoost] catch exception : " << e.what() << std::endl;
    //            assert( false );
    //            throw e;
    //        }

    //        if ( byteSent == -1 ) {
    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG( getHeader( m_sock ) << "can't send packet " << byteSent << "/" << len
    //            ); perror( "[socket] send failed.\n" );
    // #endif
    ////            close();
    //            disconnect();
    //            throw SocketSystem::exception(
    //                "[ClientSocketBoost] write(data, len) Can't write packet, peer connection
    //                lost" );
    //        }
    //        else if ( byteSent == 0 ) {
    //            assert( false );
    //            exit( 1 );
    ////            close();
    ////            throw SocketSystem::exception(
    ////                "[ClientSocketBoost] write(data, len) Can't write packet, buffer overflow"
    ///);
    // #ifdef HUB_DEBUG_SOCKET
    //             DEBUG_MSG( "byteSent == 0, sleep" );
    // #endif
    //         }
    //         uploadSize += byteSent;
    // #ifdef HUB_DEBUG_SOCKET
    //         DEBUG_MSG( getHeader( m_sock )
    //                    << "byteSent = " << byteSent << " (" << uploadSize << "/" << len << ")" );
    // #endif
    //     } while ( len != uploadSize );

    //    //    } // end for ( int iPart = 0; iPart < nPart; ++iPart )

    //        assert(memcmp(tmp, data, len) == 0);
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocketBoost::read( unsigned char* data, const size_t size ) {
    //    assert( isOpen() );
    //    assert( isConnected() );
    //    assert( SocketSystem::isConnected() );

    const auto& buffer = ::boost::asio::buffer( data, size );

    //    ::boost::asio::bind_allocator()
//    ::boost::asio::streambuf buff( size );
//    auto downloadBytes = ::boost::asio::read( m_sock, buff );
    auto downloadBytes = ::boost::asio::read( m_sock, buffer );
    assert( downloadBytes == size );

//    std::cout << "[ClientSocketBoost] read() read data : [";
//    for ( int j = 0; j < size; ++j ) {
//        std::cout << ( (unsigned char*)buff.data().data() )[j] << " ";
//    }
//    std::cout << "]" << std::endl;

//    memcpy( data, (unsigned char*)buffer.data().data(), size );


    //    size_t downloadSize = 0;
    //    do {
    //        assert( 0 <= len - downloadSize && len - downloadSize <= len );
    //        int64_t byteRead = utils::recv(
    //            m_sock, reinterpret_cast<char*>( data ) + downloadSize, len - downloadSize, 0 );
    //        //assert( -1 <= byteRead && byteRead <= (int64_t)len );
    //        assert( (int64_t)len > 0 );
    //        assert( -1 <= byteRead && byteRead <= (int64_t)len );

    //        if ( byteRead == -1 ) {
    // #ifdef HUB_DEBUG_SOCKET
    //            DEBUG_MSG( "byte read == -1 error" );
    //            perror( "[socket] receive failed.\n" );
    // #endif
    // #ifndef WIN32
    // #endif
    ////            close();
    //            disconnect();
    //            throw SocketSystem::exception(
    //                "[ClientSocketSysteClientSocketSystem] read(data, len) Can't read packet, peer
    //                connection lost" );
    //        }
    //        else if ( byteRead == 0 ) {
    //            //            std::cout << "no bytes read, sleep" << std::endl;
    //            //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    ////            close();
    //            disconnect();
    //            throw SocketSystem::exception(
    //                "[ClientSocketBoost] read(data, len) 0 byte received, peer connection lost" );
    //        }

    //        downloadSize += byteRead;
    // #ifdef HUB_DEBUG_SOCKET
    //        DEBUG_MSG( getHeader( m_sock )
    //                   << "byteRead = " << byteRead << " (" << downloadSize << "/" << len << ")"
    //                   );
    // #endif
    //    } while ( len != downloadSize );

    // #ifdef HUB_DEBUG_SOCKET
    ////
    // #endif
}

////////////////////////////////////////////////////////////////

// void ClientSocketBoost::initServerAddress() {
//     assert( utils::isValid( m_ipv4 ) );
//     assert( utils::isValid( m_port ) );

//    m_addr.init( m_ipv4, m_port );
//}

} // namespace boost
} // namespace net
} // namespace hub

#endif

#include "ClientSocketBoost.hpp"

#ifdef HUB_USE_BOOST

#    include <cmath>
#    include <cstring>
#    include <regex>
#    include <thread>

namespace hub {
namespace net {
namespace boost {

#    ifdef HUB_DEBUG_SOCKET
std::string getHeader( ::boost::asio::ip::tcp::socket& iSock ) {
    std::string str;
    str += "\033[" + std::to_string( 31 + (int)iSock.native_handle() % 7 ) +
           "m[socket:" + std::to_string( (int)iSock.native_handle() ) + "]\033[0m ";
    return str;
}
#    endif

ClientSocketBoost::ClientSocketBoost( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ), m_ioService(), m_sock( m_ioService ) {
#    ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost(std::string ipv4, int port)" );
#    endif
    /// if ( autoConnect )
    /// todo connect
    /// connect();
}

ClientSocketBoost::ClientSocketBoost( ::boost::asio::ip::tcp::socket&& fdSock ) :
    m_sock( std::move( fdSock ) ) {
    m_port = -1;
#    ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "ClientSocketBoost(socket_fd fdSock)" );
#    endif
}

ClientSocketBoost::ClientSocketBoost( ClientSocketBoost&& sock ) :
    m_ipv4( sock.m_ipv4 ),
    m_port( sock.m_port ),
    m_ioService(),
    m_sock( std::move( sock.m_sock ) ) {
    sock.m_moved = true;
}

ClientSocketBoost::~ClientSocketBoost() {
#    ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "~ClientSocketBoost()" );
#    endif
    if ( !m_moved ) {
        if ( ClientSocketBoost::isConnected() ) {
            /// todo disconnect
            /// disconnect();
        }
        assert( !ClientSocketBoost::isConnected() );
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void ClientSocketBoost::setIpv4( const std::string& newIpv4 ) {
    assert( !isConnected() );
    m_ipv4 = newIpv4;
}

const std::string& ClientSocketBoost::getIpv4() const {
    return m_ipv4;
}

void ClientSocketBoost::setPort( int newPort ) {
    assert( !isConnected() );
    assert( 0 <= newPort && newPort <= 65535 );
    m_port = newPort;
}

int ClientSocketBoost::getPort() const {
    return m_port;
}

void ClientSocketBoost::connect() {
    ////    assert( !isOpen() );

    m_sock.connect( ::boost::asio::ip::tcp::endpoint(
        ::boost::asio::ip::address::from_string( m_ipv4 ), m_port ) );

    assert( m_sock.is_open() );

#    ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader( m_sock ) << "[ClientSocketSysteClientSocketSystem] connected to the "
                                      "server, starting communication" );
    DEBUG_MSG( getHeader( m_sock ) << "new client on socket " << m_sock.native_handle() );
#    endif
}

bool ClientSocketBoost::isConnected() const {
    return !m_moved && m_sock.is_open();
}

void ClientSocketBoost::disconnect() {
    assert( m_sock.is_open() );

    ::boost::system::error_code ec;

    m_sock.close( ec );
    if ( ec ) { assert( false ); }
    assert( !m_sock.is_open() );
}

void ClientSocketBoost::write( const unsigned char* data, const size_t size ) {
    ////    assert( isOpen() );

    const auto& buffer = ::boost::asio::buffer( data, size );

    auto uploadBytes = ::boost::asio::write( m_sock, buffer );
    assert( uploadBytes == size );
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocketBoost::read( unsigned char* data, const size_t size ) {

    const auto& buffer = ::boost::asio::buffer( data, size );

    auto downloadBytes = ::boost::asio::read( m_sock, buffer );
    assert( downloadBytes == size );
}

////////////////////////////////////////////////////////////////

} // namespace boost
} // namespace net
} // namespace hub

#endif

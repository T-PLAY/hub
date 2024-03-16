#include "ClientSocketSystem.hpp"

#include <cmath>
#include <cstring>
#include <regex>
#include <thread>

namespace hub {
namespace net {
namespace system {

ClientSocketSystem::ClientSocketSystem( const std::string& ipv4, int port, bool autoConnect ) :
    m_ipv4( ipv4 ), m_port( port ) {

    initServerAddress();

    /// todo fix
    if ( autoConnect ) connect();
}

ClientSocketSystem::ClientSocketSystem( utils::socket_fd fdSock, utils::ClientAddr clientAddr ) :
    m_ipv4( clientAddr.getIpv4() ),
    m_port( clientAddr.getPort() ),
    m_addr( std::move( clientAddr ) ),
    m_connected( true ) {
    assert( utils::isConnected( fdSock ) );
    m_fdSock     = fdSock;
    m_serverSide = true;
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "accepting new socket:" << toString() );
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////

void ClientSocketSystem::setIpv4( const std::string& newIpv4 ) {
    assert( !isConnected() );
    assert( utils::isValid( newIpv4 ) );
    m_ipv4 = newIpv4;
    m_addr.setIpv4( m_ipv4 );
}

const std::string& ClientSocketSystem::getIpv4() const {
    return m_ipv4;
}

void ClientSocketSystem::setPort( int newPort ) {
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

#ifdef HUB_DEBUG_SOCKET
    // DEBUG_MSG( "[ClientSocket] connectToServer" );
    DEBUG_MSG( getHeader() << "[ClientSocket] ClientSocket('" << m_ipv4 << ", " << m_port << ")" );
#endif

    assert( !utils::isValid( m_fdSock ) );

    m_fdSock = utils::clientSocket();
    if ( !utils::isValid( m_fdSock ) ) {
        perror( "[socket] socket creation failed.\n" );
        return;
    }
#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "[ClientSocket] client socket inited" );
#endif

    assert( utils::isValid( m_fdSock ) );
    assert( !isConnected() );

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

    assert( !isConnected() );

    m_connected = true;
    assert( isConnected() );

#ifdef HUB_DEBUG_SOCKET
    DEBUG_MSG( getHeader() << "[ClientSocket] connected to the server, starting communication" );
    DEBUG_MSG( getHeader() << "new client on socket " << m_fdSock );
#endif
}

bool ClientSocketSystem::isConnected() const {
    return m_connected && SocketSystem::isConnected();
}

void ClientSocketSystem::disconnect() {
    assert( utils::isValid( m_fdSock ) );
    if ( isConnected() ) {
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( getHeader() << "disconnecting socket ..." );
#endif
        utils::closeSocket( m_fdSock );
#ifdef HUB_DEBUG_SOCKET
        DEBUG_MSG( getHeader() << "socket disconnected" );
#endif
        assert( m_connected );
        m_connected = false;
    }
    assert( !isConnected() );
}

void ClientSocketSystem::write( const unsigned char* data, const size_t size ) {
    assert( m_connected );
    assert( 0 < size );

#ifdef HUB_DEBUG_SOCKET_OUTPUT
    DEBUG_MSG( getHeader() << "write(unsigned char*, size = " << size << ") " );
#endif

    size_t uploadSize = 0;
    do {
        if ( !isConnected() ) {
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG(
                getHeader()
                << "write(const unsigned char* data, size_t size) : isConnected() client lost" );
#endif
            assert( m_connected );
            m_connected = false;
            throw SocketSystem::exception(
                "[ClientSocket] write(data, size) Can't write packet, peer not connected" );
        }
        int64_t byteSent;
        try {
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG( getHeader() << "sending bytes = " << size - uploadSize );
#endif
            assert( 0 < size - uploadSize && size - uploadSize <= size );
            byteSent = utils::send( m_fdSock,
                                    reinterpret_cast<const char*>( data + uploadSize ),
                                    size - uploadSize,
                                    0 );
#ifdef HUB_DEBUG_SOCKET_OUTPUT
            DEBUG_MSG( getHeader() << "sended bytes = " << byteSent );
#endif
            assert( -1 <= byteSent && byteSent <= (int64_t)size );
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
            disconnect();
            throw SocketSystem::exception(
                "[ClientSocket] write(data, size) Can't write packet, peer connection lost" );
        }
        else if ( byteSent == 0 ) {
            assert( false );
            exit( 1 );
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
}

///////////////////////////////////////////////////////////////////////////////

void ClientSocketSystem::read( unsigned char* data, const size_t size ) {
    assert( isConnected() );

    size_t downloadSize = 0;
    do {
        assert( size > downloadSize );
        int64_t byteRead = utils::recv(
            m_fdSock, reinterpret_cast<char*>( data ) + downloadSize, size - downloadSize, 0 );
        assert( (int64_t)size > 0 );
        assert( -1 <= byteRead && byteRead <= (int64_t)size );

        if ( byteRead == -1 ) {
#ifdef HUB_DEBUG_SOCKET
            DEBUG_MSG( "byte read == -1 error" );
            perror( "[socket] receive failed.\n" );
#endif
#ifndef WIN32
#endif
            disconnect();
            throw SocketSystem::exception( "[ClientSocketSystem] read(data, size) "
                                           "Can't read packet, peer connection lost" );
        }
        else if ( byteRead == 0 ) {
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
}

////////////////////////////////////////////////////////////////

void ClientSocketSystem::initServerAddress() {
    assert( utils::isValid( m_ipv4 ) );
    assert( utils::isValid( m_port ) );

    m_addr.init( m_ipv4, m_port );
}

} // namespace system
} // namespace net
} // namespace hub


#include "io/InputStream.hpp"

namespace hub {
namespace io {

InputStream::InputStream( const std::string& streamName, net::ClientSocket&& clientSocket ) :

    m_clientSocket( std::move( clientSocket ) ) {

    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );

    assert( streamName != "" );
    m_clientSocket.write( streamName );

    net::ClientSocket::Message mess;
    Input::read( mess );
    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
                                         << "' is not attached to server" );
#endif
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( mess == net::ClientSocket::Message::OK );
}

InputStream::InputStream( InputStream&& inputStream ) :
    m_clientSocket( std::move( inputStream.m_clientSocket ) ),
    m_streamViewerClientClosed( inputStream.m_streamViewerClientClosed ),
    m_streamerClosed( inputStream.m_streamerClosed ),
    m_readAcqWaiting( inputStream.m_readAcqWaiting ) {
    inputStream.m_moved = true;
}

InputStream::~InputStream() {
    if ( !m_moved ) { assert( !InputStream::isOpen() ); }
}

void InputStream::read( Acquisition& acq ) {
    assert( !m_readAcqWaiting );
    m_readAcqWaiting = true;

    assert( !m_streamerClosed );

    // Acquisition InputStream::getAcq() {
    net::ClientSocket::Message message;
    try {
        m_clientSocket.read( message );
    }
    catch ( const std::exception& ex ) {
        m_streamerClosed = true;
        m_readAcqWaiting = false;
        throw new std::exception( ex );
    }

    if ( message == net::ClientSocket::Message::STREAMER_CLOSED ) {
        std::cout << "[InputStream] streamer closed" << std::endl;

        m_streamerClosed = true;
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sreamer closed from server" );
    }
    else if ( message == net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED ) {
        std::cout << "[InputStream] stream viewer client closed" << std::endl;

        m_streamViewerClientClosed = true;
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sream viewer client closed from server" );
    }
    else if ( message == net::ClientSocket::Message::NEW_ACQ ) {
        assert( m_clientSocket.isOpen() );

        m_clientSocket.read( acq );
    }
    else {
        assert( false );
    }

    m_readAcqWaiting = false;
}

void InputStream::read( SensorSpec& sensorSpec ) {
    m_clientSocket.read( sensorSpec );
}

////    if ( mess != net::ClientSocket::Message::CLOSED ) {
////#ifdef DEBUG_SOCKET
////        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStream] exception sensor '" << streamName
////                                         << "' is not attached to server" );
////#endif
////        throw net::ClientSocket::exception(
////            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str()
///); /    } /    assert( mess == net::ClientSocket::Message::OK );

// #ifdef WIN32
// void InputStream::write( const unsigned char* data, size_t len ) {
// }

// void InputStream::read( unsigned char* data, size_t len ) {
// }

// void InputStream::close() {
// }
// bool InputStream::isOpen() const {
// }
// bool InputStream::isEnd() const {
// }
// #endif

} // namespace io
} // namespace hub

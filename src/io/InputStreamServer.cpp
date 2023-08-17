
#include "io/InputStreamServer.hpp"

namespace hub {
namespace io {

//InputStreamServer::InputStreamServer( const std::string& streamName, net::ClientSocket&& clientSocket ) :
InputStreamServer::InputStreamServer(const std::string &streamName, const std::string &ipv4, int port) :
    InputStreamInterface(streamName, ipv4, port),
//    m_clientSocket( std::move( clientSocket ) ) {
    m_clientSocket(ipv4, port)
    {

    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );

    assert( streamName != "" );
    m_clientSocket.write( streamName );

    net::ClientSocket::Message mess;
    Input::read( mess );
    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputStreamServer] exception sensor '" << streamName
                                         << "' is not attached to server" );
#endif
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( mess == net::ClientSocket::Message::OK );
}


InputStreamServer::InputStreamServer( InputStreamServer&& inputStream ) :
    InputStreamInterface(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port),
    m_clientSocket( std::move( inputStream.m_clientSocket ) ),
    m_streamViewerClientClosed( inputStream.m_streamViewerClientClosed ),
    m_streamerClosed( inputStream.m_streamerClosed ),
    m_readAcqWaiting( inputStream.m_readAcqWaiting ) {
    inputStream.m_moved = true;
}

InputStreamServer::~InputStreamServer() {
    if ( !m_moved ) { assert( !InputStreamServer::isOpen() ); }
}

void InputStreamServer::read( Acquisition& acq ) {
    assert( !m_readAcqWaiting );
    m_readAcqWaiting = true;

    assert( !m_streamerClosed );

    // Acquisition InputStreamServer::getAcq() {
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
        std::cout << "[InputStreamServer] streamer closed" << std::endl;

        m_streamerClosed = true;
        m_clientSocket.write( net::ClientSocket::Message::INPUT_STREAM_CLOSED );
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sreamer closed from server" );
    }
    else if ( message == net::ClientSocket::Message::STREAM_VIEWER_CLIENT_CLOSED ) {
        std::cout << "[InputStreamServer] stream viewer client closed" << std::endl;

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

void InputStreamServer::read( SensorSpec& sensorSpec ) {
    m_clientSocket.read( sensorSpec );
}

} // namespace io
} // namespace hub

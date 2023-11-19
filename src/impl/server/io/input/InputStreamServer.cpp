
#include "InputStreamServer.hpp"
#include "sensor/Acquisition.hpp"
#include "sensor/SensorSpec.hpp"

namespace hub {
namespace input {

//InputStreamServer::InputStreamServer( const std::string& streamName, net::ClientSocket&& clientSocket ) :
InputStreamServer::InputStreamServer(const std::string &streamName, const std::string &ipv4, int port) :
    io::StreamServer(streamName, ipv4, port),
//    m_clientSocket( std::move( clientSocket ) ) {
//    m_clientSocket(ipv4, port)
    m_clientSocket(net::ClientSocket(ipv4, port))
    {

    assert(m_clientSocket.isConnected());
//    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );
    m_clientSocket.write( ClientType::STREAM_VIEWER);

    assert( streamName != "" );
    m_clientSocket.write( streamName );

    io::StreamInterface::ServerMessage serverMsg;
    Input::read( serverMsg );
    if ( serverMsg == io::StreamInterface::ServerMessage::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( serverMsg == io::StreamInterface::ServerMessage::OK );

    std::cout << "[InputStreamServer:" << this << "] InputStreamServer() ended" << std::endl;
}


InputStreamServer::InputStreamServer( InputStreamServer&& inputStream ) :
    io::StreamServer(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port),
    m_clientSocket( std::move( inputStream.m_clientSocket ) ),
    m_streamViewerClientClosed( inputStream.m_streamViewerClientClosed ),
    m_streamerClosed( inputStream.m_streamerClosed ),
    m_readAcqWaiting( inputStream.m_readAcqWaiting ) {
    inputStream.m_moved = true;

    std::cout << "[InputStreamServer:" << this << "] InputStreamServer(&&)" << std::endl;
}

//InputStreamServer::~InputStreamServer() {
//    if ( !m_moved ) {
//        assert( !InputStreamServer::isOpen() );
//    }
//}

void InputStreamServer::read( sensor::Acquisition& acq ) {
    assert( !m_readAcqWaiting );
    m_readAcqWaiting = true;

    assert( !m_streamerClosed );

    // Acquisition InputStreamServer::getAcq() {
    io::StreamInterface::ServerMessage serverMsg;
    try {
        m_clientSocket.read( serverMsg );
    }
    catch ( const std::exception& ex ) {
        m_streamerClosed = true;
        m_readAcqWaiting = false;
        throw new std::exception( ex );
    }

    if ( serverMsg == io::StreamInterface::ServerMessage::STREAMER_CLOSED ) {
        std::cout << "[InputStreamServer] streamer closed" << std::endl;

        m_streamerClosed = true;
        m_clientSocket.write( io::StreamInterface::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sreamer closed from server" );
    }
    else if ( serverMsg == io::StreamInterface::ServerMessage::STREAM_VIEWER_CLOSED ) {
        std::cout << "[InputStreamServer] stream viewer client closed" << std::endl;

        m_streamViewerClientClosed = true;
        m_clientSocket.write( io::StreamInterface::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
        m_readAcqWaiting = false;
        throw net::ClientSocket::exception( "sream viewer client closed from server" );
    }
    else if ( serverMsg == io::StreamInterface::ServerMessage::STREAM_VIEWER_NEW_ACQ ) {
        assert( m_clientSocket.isOpen() );

        m_clientSocket.read( acq );
    }
    else {
        assert( false );
    }

    m_readAcqWaiting = false;
}

void InputStreamServer::read( sensor::SensorSpec& sensorSpec ) {
    m_clientSocket.read( sensorSpec );
}

void InputStreamServer::clear()
{
}

} // namespace input
} // namespace hub


#include "InputStreamServer2.hpp"

namespace hub {
namespace input {

InputStreamServer2::InputStreamServer2( int streamPort, const std::string& ipv4 ) :
    io::StreamServer2( "", "", 0 ),
    m_streamIpv4{ipv4},
    m_streamPort{streamPort}
{


    startStream();
}

InputStreamServer2::InputStreamServer2( const std::string& streamName,
                                        int serverPort,
                                        const std::string& serverIpv4 ) :
    io::StreamServer2( streamName, serverIpv4, serverPort ),
    m_serverSocket( std::make_unique<hub::io::InputOutputSocket>(
        net::ClientSocket( serverIpv4, serverPort ) ) ) {

    assert( m_serverSocket->isConnected() );
    m_serverSocket->write( ClientType::STREAM_VIEWER );

    assert( streamName != "" );
    m_serverSocket->write( streamName );

    hub::io::StreamBase::ServerMessage serverMsg;
    m_serverSocket->read( serverMsg );
    if ( serverMsg == hub::io::StreamBase::ServerMessage::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( serverMsg == hub::io::StreamBase::ServerMessage::OK );

    m_serverSocket->read( m_streamIpv4 );
    m_serverSocket->read( m_streamPort );

    startStream();
}

void InputStreamServer2::startStream() {
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStream] startStream()" << std::endl;
    std::cout << "[InputStream] stream socket initing ..." << std::endl;
#endif
    m_streamSocket = std::make_unique<hub::io::InputOutputSocket>(
        net::ClientSocket( m_streamIpv4, m_streamPort ) );
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStream] stream socket inited : " << *m_streamSocket << std::endl;
#endif
    m_streamSocket->write( ClientType::STREAM_VIEWER );

#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStream] reading header ..." << std::endl;
#endif
    assert( m_header.getUserDefined().empty() );
    m_streamSocket->read( m_header );
#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStream] read header" << std::endl;
#endif

    hub::io::StreamBase::ClientMessage clientMessage;
    m_streamSocket->read( clientMessage );
    assert( clientMessage ==
            hub::io::StreamBase::ClientMessage::STREAMER_CLIENT_STREAM_VIEWER_INITED );

#ifdef HUB_DEBUG_INPUT_STREAM
    std::cout << "[InputStream] startStream() done" << std::endl;
#endif
}

void InputStreamServer2::clear() {}

} // namespace input
} // namespace hub

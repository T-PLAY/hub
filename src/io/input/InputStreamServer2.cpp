
#include "InputStreamServer2.hpp"
// #include "sensor/Acquisition.hpp"
// #include "sensor/SensorSpec.hpp"

namespace hub {
namespace input {

// InputStreamServer2::InputStreamServer2( const std::string& streamName, net::ClientSocket&&
// clientSocket ) :
InputStreamServer2::InputStreamServer2( int streamPort, const std::string& ipv4 ) :
    io::StreamServer2(  "", "", 0 ) {

    m_streamPort = streamPort;
    m_streamIpv4 = ipv4;

    startStream();
}

InputStreamServer2::InputStreamServer2( const std::string& streamName,
                                        int serverPort,
                                        const std::string& serverIpv4 ) :
    io::StreamServer2(  streamName, serverIpv4, serverPort ),
    //    m_serverSocket( std::move( clientSocket ) ) {
    //    m_serverSocket(ipv4, serverPort)
    m_serverSocket( std::make_unique<hub::io::InputOutputSocket>(
        net::ClientSocket( serverIpv4, serverPort ) ) ) {

    assert( m_serverSocket->isConnected() );
    //    m_serverSocket.write( net::ClientSocket::Type::STREAM_VIEWER );
    m_serverSocket->write( ClientType::STREAM_VIEWER );

    assert( streamName != "" );
    m_serverSocket->write( streamName );

    //    if ( streamName != io::StreamServer::s_exitSignal ) {

    hub::io::StreamBase::ServerMessage serverMsg;
    //        Input::read( serverMsg );
    m_serverSocket->read( serverMsg );
    if ( serverMsg == hub::io::StreamBase::ServerMessage::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( serverMsg == hub::io::StreamBase::ServerMessage::OK );

    m_serverSocket->read( m_streamIpv4 );
    m_serverSocket->read( m_streamPort );
    //    m_serverSocket.close();

    //        std::cout << "[InputStreamServer2:" << this << "] InputStreamServer2() ended" <<
    //        std::endl;
    //    }
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

// InputStreamServer2::InputStreamServer2( InputStreamServer2&& inputStream ) :
//     io::StreamServer(inputStream.m_name, inputStream.m_ipv4, inputStream.m_port),
//     m_serverSockem_serverSocket( std::move( inputStream.m_serverSocket ) ),
//     m_streamViewerClientClosed( inputStream.m_streamViewerClientClosed ),
//     m_streamerClosed( inputStream.m_streamerClosed ),
//     m_readAcqWaiting( inputStream.m_readAcqWaiting ) {
//     inputStream.m_moved = true;

//    std::cout << "[InputStreamServer2:" << this << "] InputStreamServer2(&&)" << std::endl;
//}

// InputStreamServer2::~InputStreamServer2() {
//     if ( !m_moved ) {
//         assert( !InputStreamServer2::isOpen() );
//     }
// }

// void InputStreamServer2::read( sensor::Acquisition& acq ) {
//     assert( !m_readAcqWaiting );
//     m_readAcqWaiting = true;

//    assert( !m_streamerClosed );

//    // Acquisition InputStreamServer2::getAcq() {
//    io::StreamBase::ServerMessage serverMsg;
//    try {
//        m_serverSocket.read( serverMsg );
//    }
//    catch ( const std::exception& ex ) {
//        m_streamerClosed = true;
//        m_readAcqWaiting = false;
//        throw new std::exception( ex );
//    }

//    if ( serverMsg == io::StreamBase::ServerMessage::STREAMER_CLOSED ) {
//        std::cout << "[InputStreamServer2] streamer closed" << std::endl;

//        m_streamerClosed = true;
//        m_serverSocket.write( io::StreamBase::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
//        m_readAcqWaiting = false;
//        throw net::ClientSocket::exception( "sreamer closed from server" );
//    }
//    else if ( serverMsg == io::StreamBase::ServerMessage::STREAM_VIEWER_CLOSED ) {
//        std::cout << "[InputStreamServer2] stream viewer client closed" << std::endl;

//        m_streamViewerClientClosed = true;
//        m_serverSocket.write( io::StreamBase::ClientMessage::STREAM_VIEWER_CLIENT_CLOSED );
//        m_readAcqWaiting = false;
//        throw net::ClientSocket::exception( "sream viewer client closed from server" );
//    }
//    else if ( serverMsg == io::StreamBase::ServerMessage::STREAM_VIEWER_NEW_ACQ ) {
//        assert( m_serverSocket.isOpen() );

//        m_serverSocket.read( acq );
//    }
//    else {
//        assert( false );
//    }

//    m_readAcqWaiting = false;
//}

// void InputStreamServer2::read( sensor::SensorSpec& sensorSpec ) {
//     m_serverSocket.read( sensorSpec );
// }

void InputStreamServer2::clear() {}

} // namespace input
} // namespace hub

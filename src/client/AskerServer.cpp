#include "AskerServer.hpp"


namespace hub {
namespace client {


AskerServer::AskerServer( const std::string& ipv4, int port ) :

    m_sock( ipv4, port ) {

    assert( m_sock.isOpen() );

//    m_sock.write( net::ClientSocket::Type::ASKER );
    m_sock.write( io::StreamInterface::ClientType::ASKER );
}

AskerServer::~AskerServer() {
//    m_sock.write( io::StreamInterface::ClientMessage::CLOSE );
    m_sock.write( io::StreamInterface::ClientMessage::ASKER_CLIENT_CLOSED );
    assert( m_sock.isOpen() );
    m_sock.close();
    assert( !m_sock.isOpen() );
}

std::list<std::pair<std::string, SensorSpec>> AskerServer::listStreams() {
//    m_sock.write( io::StreamInterface::ClientMessage::LIST_STREAMS );
    m_sock.write( io::StreamInterface::ClientMessage::ASKER_CLIENT_GET_LIST_STREAMS );


    std::list<std::pair<std::string, hub::SensorSpec>> ret;
    m_sock.read( ret );

    return ret;
}

Acquisition AskerServer::getAcquisition( const std::string& streamName ) {
    m_sock.write( io::StreamInterface::ClientMessage::ASKER_CLIENT_GET_ACQ );
    m_sock.write( streamName );

    io::StreamInterface::ServerMessage serverMsg;
    m_sock.read( serverMsg );

    if ( serverMsg == io::StreamInterface::ServerMessage::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( serverMsg == io::StreamInterface::ServerMessage::FOUND );

    hub::SensorSpec sensorSpec;
    m_sock.read( sensorSpec );

    hub::Acquisition acq;
    m_sock.read( acq );
    return acq;
}

} // namespace client
} // namespace hub

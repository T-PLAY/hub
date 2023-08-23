#include "AskerServer.hpp"


namespace hub {
namespace client {


AskerServer::AskerServer( const std::string& ipv4, int port ) :

    m_sock( ipv4, port ) {

    assert( m_sock.isOpen() );

    m_sock.write( net::ClientSocket::Type::ASKER );
}

AskerServer::~AskerServer() {
    m_sock.write( net::ClientSocket::Message::CLOSE );
    assert( m_sock.isOpen() );
    m_sock.close();
    assert( !m_sock.isOpen() );
}

std::list<std::pair<std::string, SensorSpec>> AskerServer::listStreams() {
    m_sock.write( net::ClientSocket::Message::LIST_STREAMS );


    std::list<std::pair<std::string, hub::SensorSpec>> ret;
    m_sock.read( ret );

    return ret;
}

Acquisition AskerServer::getAcquisition( const std::string& streamName ) {
    m_sock.write( net::ClientSocket::Message::GET_ACQUISITION );
    m_sock.write( streamName );

    net::ClientSocket::Message message;
    m_sock.read( message );

    if ( message == net::ClientSocket::Message::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( message == net::ClientSocket::Message::FOUND );

    hub::SensorSpec sensorSpec;
    m_sock.read( sensorSpec );

    hub::Acquisition acq;
    m_sock.read( acq );
    return acq;
}

} // namespace client
} // namespace hub

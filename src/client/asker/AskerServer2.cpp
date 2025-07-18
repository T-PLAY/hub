#include "AskerServer2.hpp"

#include "io/StreamBase.hpp"

namespace hub {
namespace client {

AskerServer2::AskerServer2( const std::string& ipv4, int port ) :
    m_sock( net::ClientSocket( ipv4, port ) ) {

    assert( m_sock.isOpen() );

    m_sock.write( hub::io::StreamBase::ClientType::ASKER );
}

AskerServer2::~AskerServer2() {
    m_sock.write( hub::io::StreamBase::ClientMessage::ASKER_CLIENT_CLOSED );
    assert( m_sock.isOpen() );
    m_sock.close();
    assert( !m_sock.isOpen() );
}

std::list<std::pair<std::string, sensor::SensorSpec>> AskerServer2::listStreams() {
    m_sock.write( hub::io::StreamBase::ClientMessage::ASKER_CLIENT_GET_LIST_STREAMS );

    std::list<std::pair<std::string, sensor::SensorSpec>> ret;

    return ret;
}

sensor::Acquisition AskerServer2::getAcquisition( const std::string& streamName ) {
    m_sock.write( hub::io::StreamBase::ClientMessage::ASKER_CLIENT_GET_ACQ );
    m_sock.write( streamName );

    hub::io::StreamBase::ServerMessage serverMsg;
    m_sock.read( serverMsg );

    if ( serverMsg == hub::io::StreamBase::ServerMessage::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( serverMsg == hub::io::StreamBase::ServerMessage::FOUND );

    sensor::SensorSpec sensorSpec;
    m_sock.read( sensorSpec );

    sensor::Acquisition acq;
    m_sock.read( acq );
    return acq;
}

} // namespace client
} // namespace hub

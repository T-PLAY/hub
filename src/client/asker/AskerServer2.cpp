#include "AskerServer2.hpp"

namespace hub {
// namespace impl2 {
namespace client {

AskerServer2::AskerServer2( const std::string& ipv4, int port ) :
    //    m_sock( ipv4, port )
    m_sock( net::ClientSocket( ipv4, port ) ) {

    assert( m_sock.isOpen() );

    //    m_sock.write( net::ClientSocket::Type::ASKER );
    m_sock.write( hub::io::StreamBase::ClientType::ASKER );
}

AskerServer2::~AskerServer2() {
    //    m_sock.write( io::StreamBase::ClientMessage::CLOSE );
    m_sock.write( hub::io::StreamBase::ClientMessage::ASKER_CLIENT_CLOSED );
    assert( m_sock.isOpen() );
    m_sock.close();
    assert( !m_sock.isOpen() );
}

std::list<std::pair<std::string, sensor::SensorSpec>> AskerServer2::listStreams() {
    //    m_sock.write( io::StreamBase::ClientMessage::LIST_STREAMS );
    m_sock.write( hub::io::StreamBase::ClientMessage::ASKER_CLIENT_GET_LIST_STREAMS );

    // todo server
    std::list<std::pair<std::string, sensor::SensorSpec>> ret;
    //    m_sock.read( ret );

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
// } // namespace impl2
} // namespace hub

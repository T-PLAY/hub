
#include "io/InputSyncStream.hpp"

namespace hub {
namespace io {

InputSyncStream::InputSyncStream( const std::string& streamName,
                          const std::string& syncStreamName,
                          net::ClientSocket&& clientSocket) :
//                          bool mergeSyncAcqs ) :

    m_clientSocket( std::move( clientSocket ) ),
    m_clientSocket2(m_clientSocket.getIpv4(), m_clientSocket.getPort())
{
    assert(streamName != "");
    assert(syncStreamName != "");

    m_clientSocket.write( net::ClientSocket::Type::STREAM_VIEWER );

    m_clientSocket.write( streamName );

    net::ClientSocket::Message mess;
    m_clientSocket.read( mess );
    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
#ifdef DEBUG_SOCKET
        DEBUG_MSG( getHeader( m_fdSock ) << "[InputSyncStream] exception sensor '" << streamName
                                         << "' is not attached to server" );
#endif
        throw net::ClientSocket::exception(
            ( std::string( "stream '" ) + streamName + "' is not attached to server" ).c_str() );
    }
    assert( mess == net::ClientSocket::Message::OK );


    m_clientSocket2.write( net::ClientSocket::Type::STREAM_VIEWER );
    m_clientSocket2.write( syncStreamName );

    m_clientSocket2.read( mess );
    if ( mess == net::ClientSocket::Message::NOT_FOUND ) {
        throw net::ClientSocket::exception(
            ( std::string( "sync stream '" ) + syncStreamName + "' is not attached to server" )
                .c_str() );
    }
    assert( mess == net::ClientSocket::Message::OK );

//    m_clientSocket.write( mergeSyncAcqs );
}


Acquisition InputSyncStream::getAcquisition(  ) {

    Input & masterInput = m_clientSocket;
    Input & input = m_clientSocket2;

    hub::Acquisition acq;
    masterInput >> input >> acq;

    return acq;

}

void InputSyncStream::read(SensorSpec &sensorSpec)
{
    m_clientSocket.read(m_sensorSpec);
    m_clientSocket2.read(m_sensorSpec2);

    sensorSpec = m_sensorSpec + m_sensorSpec2;
}

} // namespace io
} // namespace hub

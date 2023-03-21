
#include "io/InputSyncStream.hpp"

namespace hub {
namespace io {

InputSyncStream::InputSyncStream( const std::string& streamName,
                                  const std::string& syncStreamName,
                                  const std::string& ipv4,
                                  int port ) :
    //                          net::ClientSocket&& clientSocket) :
    m_inputStream( streamName, hub::net::ClientSocket( ipv4, port ) ),
    m_inputStream2( syncStreamName, hub::net::ClientSocket( ipv4, port ) )
//                          bool mergeSyncAcqs ) :

//    m_clientSocket( std::move( clientSocket ) ),
//    m_clientSocket2(m_clientSocket.getIpv4(), m_clientSocket.getPort())
{
}

Acquisition InputSyncStream::getAcq( const SensorSpec& sensorSpec ) {

    Input& masterInput = m_inputStream;
    Input& input       = m_inputStream2;

    hub::Acquisition acq = masterInput.getSyncAcq( m_inputStream.m_sensorSpec, input, m_inputStream2.m_sensorSpec, m_lastAcqs );
    //    masterInput >> input >> acq;

    return acq;
}

void InputSyncStream::read( SensorSpec& sensorSpec ) {
//    m_clientSocket.read( m_sensorSpec );
    m_inputStream.read(sensorSpec);
//    m_clientSocket2.read( m_sensorSpec2 );
    m_inputStream2.read(sensorSpec);

//    sensorSpec = m_sensorSpec + m_sensorSpec2;
    sensorSpec = m_inputStream.m_sensorSpec + m_inputStream2.m_sensorSpec;
}

} // namespace io
} // namespace hub

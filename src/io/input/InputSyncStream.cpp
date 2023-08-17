
#include "InputSyncStream.hpp"

namespace hub {
using namespace io;
namespace input {

InputSyncStream::InputSyncStream( const std::string& streamName,
                                  const std::string& syncStreamName,
                                  const std::string& ipv4,
                                  int port ) :
//    m_inputStream( streamName, hub::net::ClientSocket( ipv4, port ) ),
//    m_inputStream2( syncStreamName, hub::net::ClientSocket( ipv4, port ) )
    m_inputStream( streamName, ipv4, port ),
    m_inputStream2( syncStreamName, ipv4, port )
{}

void InputSyncStream::read( Acquisition& acq ) {

    Input& masterInput = m_inputStream;
    Input& input       = m_inputStream2;

    masterInput >> input >> acq;
}

void InputSyncStream::read( SensorSpec& sensorSpec ) {
    m_inputStream.read( sensorSpec );
    SensorSpec sensorSpec2;
    m_inputStream2.read( sensorSpec2 );

    sensorSpec = sensorSpec + sensorSpec2;
}

} // namespace input
} // namespace hub

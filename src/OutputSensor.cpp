#include "OutputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {
//#define DEBUG_STREAM

// OutputSensor::OutputSensor( const SensorSpec& sensorSpec, ClientSocket&& outputInterface ) :
//     Sensor( sensorSpec, *std::move( new ClientSocket( std::move( outputInterface ) ) ) ) {

//    m_interface.setupOutput( spec.getSensorName() );

//    spec.write( m_interface );
//}

//OutputSensor::OutputSensor( const SensorSpec& sensorSpec, io::Output&& output ) :
//    Sensor( sensorSpec, *(new io::Output(std::move(output))) ) {

//    m_interface.write( spec );
//}

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
    assert( acquisition.mData != nullptr );
    // assert( acquisition.mSize == mAcquisitionSize );

    assert( acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival );

    m_interface.write( acquisition.mBackendTimestamp );
    m_interface.write( acquisition.mBackendTimeOfArrival );
    m_interface.write( acquisition.mData, spec.acquisitonSize );
}

} // namespace hub

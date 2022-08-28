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

//OutputSensor &OutputSensor::operator <<( const Acquisition& acquisition ) {
void OutputSensor::operator <<( const Acquisition& acquisition ) const {
//    assert( acquisition.m_data != nullptr );
     assert( acquisition.getSize() == m_spec.m_acquisitionSize );

    assert( acquisition.m_start <= acquisition.m_end );

    const auto & resolutions = m_spec.m_resolutions;
    const auto & measures = acquisition.getMeasures();
    assert(resolutions.size() == measures.size());
    for (int i = 0; i <resolutions.size(); ++i) {
        assert(SensorSpec::computeAcquisitionSize(resolutions.at(i)) == measures.at(i).m_size);
    }

//    m_interface.write( acquisition.m_start );
//    m_interface.write( acquisition.m_end );
//    m_interface.write( acquisition.m_data, m_spec.m_acquisitionSize );
    m_interface.write(acquisition);
//    return *this;
}

//OutputSensor & OutputSensor::operator<<( const Measure& measure ) {
//    m_interface.write(measure);
//    return *this;
//}

} // namespace hub

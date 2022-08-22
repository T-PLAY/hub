#include "InputSensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

//#define DEBUG_STREAM

namespace hub {

// InputSensor::InputSensor( const std::string& sensorName, const std::string& syncSensorName ) :
//     Sensor( {}, *std::move( new ClientSocket( sensorName, syncSensorName ) ) ) {

//    spec.read( m_interface );
//}

// InputSensor::~InputSensor() {
//#ifdef DEBUG_MSG
//     std::cout << "[InputSensor] ~InputSensor()" << std::endl;
//#endif
// }

Acquisition InputSensor::getAcquisition() const {
    long long start, end;
    unsigned char* data = new unsigned char[spec.acquisitonSize];

    m_interface.read( start );
    m_interface.read( end );
    m_interface.read( data, spec.acquisitonSize );

#ifdef DEBUG_STREAM
    Acquisition acquisition( start, end, data, spec.acquisitonSize );
    std::cout << "[InputSensor] read acq :  " << acquisition << std::endl;
#endif

    return Acquisition( start, end, data, spec.acquisitonSize );
    delete[] data;
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        while ( true ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( Sensor::exception& e ) {
        std::cout << "[InputSensor] catch sensor exception : " << e.what() << std::endl;
        throw;
    }
    catch ( std::exception& e ) {
        std::cout << "[InputSensor] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}

// std::ostream& operator<<( std::ostream& os, const InputSensor& inputSensor ) {
//     os << "metadata:" << SensorSpec::metaData2string( inputSensor.spec.metaData );
//     return os;
// }

} // namespace hub

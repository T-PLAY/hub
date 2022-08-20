#include "Sensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

namespace hub {
//#define DEBUG_STREAM

Sensor::Sensor(const SensorSpec &&sensorSpec, io::Interface& interface ) :
    spec( std::move( sensorSpec ) ),
    // mSensorName( sensorName ),
    // mFormat( format ),
    // mDims( dims ),
    //    m_interface(std::move(interface))
    m_interface( interface )
  // mAcquisitionSize( computeAcquisitionSize( spec->getFormat(), spec->getDims() ) ) {}
{}

//Sensor::Sensor(io::Interface &interface) :
//    spec(interface),
//    m_interface(interface)
//{

//}

// Sensor::Sensor( io::Interface& interface ) : m_interface( interface ) {};

Sensor::~Sensor() {
//#ifdef DEBUG_MSG
    std::cout << "[Sensor] ~Sensor()" << std::endl;
//#endif
    m_interface.close();
    delete &m_interface;
}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    //        os << sensor.mSensorName << sensor.mFormat << sensor.mDims << sensor.mAcquisitionSize;
    os << sensor.spec.sensorName;
    return os;
}

///////////////////////////////////////////////////////////////////////////////

// size_t Sensor::getAcquisitionSize() const {
//     return spec->getAcquisitionSize();
// }

//const SensorSpec& Sensor::getHeader() const {
//    return spec;
//}

io::Interface& Sensor::getIO() const {
    return m_interface;
}

} // namespace hub

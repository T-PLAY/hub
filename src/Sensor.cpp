#include "Sensor.hpp"

namespace hub {

Sensor::Sensor( const SensorSpec& sensorSpec ) :
    m_spec( std::move( sensorSpec ) ) {}

Sensor::~Sensor() {
//    if ( m_io.isOpen() ) m_io.close();
//    delete &m_io;
}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    os << sensor.m_spec.getSensorName();
    return os;
}

//io::IO& Sensor::getInterface() const {
//    return m_io;
//}

} // namespace hub

#include "Sensor.hpp"

namespace hub {

Sensor::Sensor( const SensorSpec&& sensorSpec, io::Interface& interface_ ) :
    m_spec( std::move( sensorSpec ) ), m_interface( interface_ ) {}

Sensor::~Sensor() {
    if ( m_interface.isOpen() ) m_interface.close();
    delete &m_interface;
}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    os << sensor.m_spec.getSensorName();
    return os;
}

io::Interface& Sensor::getInterface() const {
    return m_interface;
}

} // namespace hub

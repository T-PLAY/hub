#include "Sensor.hpp"

namespace hub {

Sensor::Sensor( const SensorSpec&& sensorSpec, io::Interface& interface_ ) :
    m_spec( std::move( sensorSpec ) ), m_interface( interface_ ) {}

Sensor::~Sensor() {
    //#ifdef DEBUG_MSG
    std::cout << "[Sensor] ~Sensor()" << std::endl;
    //#endif
    m_interface.close();
    delete &m_interface;
}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    os << sensor.m_spec.m_sensorName;
    return os;
}

io::Interface& Sensor::getInterface() const {
    return m_interface;
}

} // namespace hub

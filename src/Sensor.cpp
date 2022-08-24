#include "Sensor.hpp"

namespace hub {

Sensor::Sensor( const SensorSpec&& sensorSpec, io::Interface& interface ) :
    spec( std::move( sensorSpec ) ), m_interface( interface ) {}

Sensor::~Sensor() {
    //#ifdef DEBUG_MSG
    std::cout << "[Sensor] ~Sensor()" << std::endl;
    //#endif
    m_interface.close();
    delete &m_interface;
}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    os << sensor.spec.sensorName;
    return os;
}

io::Interface& Sensor::getInterface() const {
    return m_interface;
}

} // namespace hub

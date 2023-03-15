#include "Sensor.hpp"

#include <iostream>

namespace hub {

Sensor::Sensor( const SensorSpec& sensorSpec ) : m_spec( std::move( sensorSpec ) ) {}

Sensor::~Sensor() {}

std::ostream& operator<<( std::ostream& os, const Sensor& sensor ) {
    os << sensor.m_spec.getSensorName();
    return os;
}

// io::IO& Sensor::getInterface() const {
//}

} // namespace hub

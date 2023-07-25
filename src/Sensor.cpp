#include "Sensor.hpp"

#include <iostream>

namespace hub {

Sensor::Sensor( const SensorSpec& sensorSpec ) : m_spec( std::move( sensorSpec ) ) {}

Sensor::~Sensor() {}

// io::IO& Sensor::getInterface() const {

} // namespace hub

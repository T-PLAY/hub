#include "Sensor.hpp"

#include <iostream>

namespace hub {

Sensor::Sensor( const SensorSpec& sensorSpec ) : m_spec( std::move( sensorSpec ) ) {}

Sensor::~Sensor() {}

} // namespace hub

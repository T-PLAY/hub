#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

class SRC_API OutputSensor : public Sensor
{
  public:
    //    OutputSensor( const SensorSpec& sensorSpec, ClientSocket&& interface = ClientSocket() );
    template <class InterfaceT>
    OutputSensor( const SensorSpec& sensorSpec, InterfaceT&& interface );
    template <class InterfaceT>
    OutputSensor( const SensorSpec& sensorSpec, InterfaceT& interface ) = delete;

    OutputSensor( const OutputSensor& outputSensor )           = delete;
    OutputSensor operator=( const OutputSensor& outputSensor ) = delete;

  public:
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

template <class InterfaceT>
OutputSensor::OutputSensor( const SensorSpec& sensorSpec, InterfaceT&& interface ) :
    Sensor( sensorSpec, *std::move( new InterfaceT( std::move( interface ) ) ) ) {
    m_interface.setupOutput( spec.sensorName );

    //    spec.write( m_interface );
    m_interface.write( spec );
    //    m_interface.write( spec.sensorName );
    //    m_interface.write( spec.format );
    //    m_interface.write( spec.dims );
    //    m_interface.write( spec.metaData );
}

} // namespace hub

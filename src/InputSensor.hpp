#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

class SRC_API InputSensor : public Sensor
{
  public:
    //    InputSensor( const std::string& sensorName, const std::string& syncSensorName = "" );
    template <class InterfaceT>
    InputSensor( InterfaceT&& interface );
    template <class InterfaceT>
    InputSensor( InterfaceT& interface ) = delete;

    InputSensor( const InputSensor& inputSensor )           = delete;
    InputSensor operator=( const InputSensor& inputSensor ) = delete;

    //    ~InputSensor();

  public:
    Acquisition getAcquisition() const;
    std::vector<Acquisition> getAllAcquisitions();

    //  public:
    //    SRC_API friend std::ostream& operator<<( std::ostream& os, const InputSensor& inputSensor
    //    );
  private:
};

template <class InterfaceT>
InputSensor::InputSensor( InterfaceT&& interface ) :
    Sensor( {}, *std::move( new InterfaceT( std::move( interface ) ) ) ) {

    //    spec.read( m_interface );
    m_interface.read( spec );
    //    m_interface.read( spec.sensorName );
    //    m_interface.read( spec.format );
    //    m_interface.read( spec.dims );
    //    m_interface.read( spec.metaData );
}

} // namespace hub

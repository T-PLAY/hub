#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

class SRC_API InputSensor : public Sensor
{
  public:
    template <class InputInterface,
              typename = typename std::enable_if<
                  std::is_base_of<io::InputInterface, InputInterface>::value>::type>
    InputSensor( InputInterface&& inputInterface ) :

        Sensor( inputInterface.getSensorSpec(), *std::move( new InputInterface( std::move( inputInterface ) ) ) ) {

        static_assert( std::is_base_of<io::InputInterface, InputInterface>::value,
                       "not a base class" );
//        m_interface.read( spec );
    }

    template <class InputInterface>
    InputSensor( InputInterface& inputInterface ) = delete;

    InputSensor( const InputSensor& inputSensor )           = delete;
    InputSensor operator=( const InputSensor& inputSensor ) = delete;

  public:
    Acquisition getAcquisition() const;
    std::vector<Acquisition> getAllAcquisitions();

  private:
};

} // namespace hub

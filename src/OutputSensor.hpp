#pragma once

#include <type_traits>

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

namespace hub {

///
/// \brief The OutputSensor class
///
class SRC_API OutputSensor : public Sensor
{
  public:
    ///
    /// \brief OutputSensor
    /// \param sensorSpec
    /// \param outputInterface
    ///
    template <class SensorSpec = hub::SensorSpec,
              class OutputInterface,
              typename = typename std::enable_if<
                  std::is_base_of<io::OutputInterface, OutputInterface>::value>::type>
    OutputSensor( SensorSpec&& sensorSpec, OutputInterface&& outputInterface ) :

        Sensor( std::move( sensorSpec ),
                *std::move( new OutputInterface( std::move( outputInterface ) ) ) ) {

        std::cout << "[OutputSensor] OutputSensor(const SensorSpec&&, OutputInterface&&)"
                  << std::endl;

        static_assert( std::is_base_of<io::OutputInterface, OutputInterface>::value,
                       "not a base class" );

        m_interface.write( m_spec );
    }

    template <class OutputInterface>
    OutputSensor( const SensorSpec&& sensorSpec, OutputInterface& outputInterface ) = delete;

    OutputSensor( const OutputSensor& outputSensor )           = delete;
    OutputSensor operator=( const OutputSensor& outputSensor ) = delete;

  public:
    ///
    /// \brief operator <<
    /// \param acquisition
    /// to send through the sensor.
    ///
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

} // namespace hub

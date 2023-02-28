#pragma once

#include <type_traits>

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

// user friendly useless includes
#include "IO/File.hpp"
#include "IO/OutputStream.hpp"
#include "IO/Ram.hpp"

namespace hub {

///
/// \brief The OutputSensor class
/// describes what a sensor physically is.
/// A sensor measures physical information from the environment around us.
/// We declare a sensor with its intrinsic data and the transportable
/// data format that the sensor driver proposes to export.
/// The sensor transmits volatile and potentially unrecordable data.
/// This class allows the data to be transmitted without being recorded
/// so that it can be used in real time on various network machines.
///
class SRC_API OutputSensor : public Sensor
{
  public:
    ///
    /// \brief OutputSensor
    /// is called when you want to share data sensor
    /// \param sensorSpec
    /// able client application to understand the data
    /// \param outputInterface
    /// is the communication bus you want to use
    ///
    template <class SensorSpec = hub::SensorSpec,
              class OutputInterface,
              typename = typename std::enable_if<
                  std::is_base_of<io::OutputInterface, OutputInterface>::value>::type>
    OutputSensor( SensorSpec&& sensorSpec, OutputInterface&& outputInterface ) :

        Sensor( std::move( sensorSpec ),
                *std::move( new OutputInterface( std::move( outputInterface ) ) ) ) {

        //        std::cout << "[OutputSensor] OutputSensor(const SensorSpec&&, OutputInterface&&)"
        //                  << std::endl;

        static_assert( std::is_base_of<io::OutputInterface, OutputInterface>::value,
                       "not a base class" );

        m_interface.write( m_spec );
    }

  protected:
    template <class OutputInterface>
    OutputSensor( const SensorSpec&& sensorSpec, OutputInterface& outputInterface ) = delete;

    OutputSensor( const OutputSensor& outputSensor ) = delete;
    OutputSensor operator=( const OutputSensor& outputSensor ) = delete;

  public:
    ///
    /// \brief operator <<
    /// is used when you want to share sensor data.
    /// \param acquisition
    /// to send through the communication bus.
    /// \warning The acquisition needs to be compatible with
    /// the internal sensor specification resolution and format.\n
    /// sensorSpec.resolutions == { acq.measures[0].resolution, acq.measures[1].resolution, ... }
    /// \exception "used interface exception"
    /// when communication is broken.
    ///
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

} // namespace hub

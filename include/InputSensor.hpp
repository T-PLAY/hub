#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"

namespace hub {

///
/// \brief The InputSensor class
/// represents the remote sensor.
/// From any communication bus (file, RAM, socket) you have access to all the information specific
/// to the sensors as well as the acquisition flow coming from the physical sensor.
/// This class allows client applications to completely abstract from the notion
/// of sensor and to be interested only in the carried data.
/// This enables several applications to work simultaneously on the same data in real time.
/// This also avoids the need to manage the drivers of each of the manufacturers of the sensors you wish to use.
///
class SRC_API InputSensor : public Sensor
{
  public:
    ///
    /// \brief InputSensor
    /// is called when you want to get data sensor
    /// \param inputInterface
    /// is the communication bus you want to use
    ///
    template <class InputInterface,
              typename = typename std::enable_if<
                  std::is_base_of<io::InputInterface, InputInterface>::value>::type>
    InputSensor( InputInterface&& inputInterface ) :

        Sensor( {}, *std::move( new InputInterface( std::move( inputInterface ) ) ) ) {

        static_assert( std::is_base_of<io::InputInterface, InputInterface>::value,
                       "not a base class" );

        //        m_interface.read( m_spec );
        m_spec = m_interface.getSensorSpec();
    }

protected:
    template <class InputInterface>
    InputSensor( InputInterface& inputInterface ) = delete;

    InputSensor( const InputSensor& inputSensor ) = delete;
    InputSensor operator=( const InputSensor& inputSensor ) = delete;

  public:
    ///
    /// \brief getAcquisition
    /// can be an asynchronous function depending of the used interface.
    /// \return
    /// the oldest acquisition shared by the physical sensor.
    /// \exception "used interface exception"
    /// when communication bus is broken.
    ///
    Acquisition getAcquisition() const;

    ///
    /// \brief getAllAcquisitions
    /// can be an asynchronous function depending of the used interface.
    /// \return
    /// all acquisitions coming from the interface.
    ///
    std::vector<Acquisition> getAllAcquisitions();

  private:
};

} // namespace hub

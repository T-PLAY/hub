#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "Sensor.hpp"

namespace hub {

///
/// \brief The InputSensor class
/// is created directly by the input interface.
///
class SRC_API InputSensor : public Sensor
{
  public:
    ///
    /// \brief InputSensor
    /// \param inputInterface
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

    template <class InputInterface>
    InputSensor( InputInterface& inputInterface ) = delete;

    InputSensor( const InputSensor& inputSensor ) = delete;
    InputSensor operator=( const InputSensor& inputSensor ) = delete;

  public:
    ///
    /// \brief getAcquisition
    /// \return
    /// the oldest acqusition
    ///
    Acquisition getAcquisition() const;

    ///
    /// \brief getAllAcquisitions
    /// \return
    ///
    std::vector<Acquisition> getAllAcquisitions();

  private:
};

} // namespace hub

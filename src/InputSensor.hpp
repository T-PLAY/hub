#pragma once

#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "io/Input.hpp"

// user friendly useless includes
#include "io/File.hpp"
#include "io/InputStream.hpp"
#include "io/Ram.hpp"

namespace hub {

///
/// \brief The InputSensor class
/// represents the remote sensor.
/// From any communication bus (file, RAM, socket) you have access to all the information specific
/// to the sensors as well as the acquisition flow coming from the physical sensor.
/// This class allows client applications to completely abstract from the notion
/// of sensor and to be interested only in the carried data.
/// This enables several applications to work simultaneously on the same data in real time.
/// This also avoids the need to manage the drivers of each of the manufacturers of the sensors you
/// wish to use.
///
class SRC_API InputSensor : public Sensor
{
  public:
    ///
    /// \brief InputSensor
    /// is called when you want to get data sensor
    /// \param input
    /// is the communication bus you want to use
    ///
    template <class Input,
              typename = typename std::enable_if<std::is_base_of<io::Input, Input>::value>::type>
    explicit InputSensor( Input&& input ) :

        Sensor( hub::SensorSpec {} ), m_input( new Input( std::move( input ) ) ) {
        static_assert( std::is_base_of<io::Input, Input>::value, "not a base class" );

        m_input->read( m_spec );
    }

    template <class Input>
    InputSensor( Input& input ) = delete;

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

    ///
    /// \brief getInput
    /// \return
    ///
    io::Input& getInput() const;

  private:
    std::unique_ptr<io::Input> m_input;
};

} // namespace hub

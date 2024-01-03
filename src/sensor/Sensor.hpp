#pragma once

#include "SensorSpec.hpp"
#include "Acquisition.hpp"

// #include "Resolution.hpp"
// #include "Format.hpp"
//#include "core/Input.hpp"

namespace hub {
namespace sensor {

///
/// \brief The Sensor class
/// describes what a sensor can be, it's defined from the constant specifities of a sensor
/// and uses a communication bus to transmit the data.
/// \note Not user instantiable class.
///
class SRC_API Sensor
{
  protected:
    ///
    /// \brief Sensor
    /// \param sensorSpec
    /// [in] user defined sensor spec
    ///
    Sensor() = default;
    explicit Sensor( const SensorSpec& sensorSpec ) : m_spec{sensorSpec} {

    }
    Sensor( const Sensor& sensor ) = delete;
    Sensor& operator=( const Sensor& sensor ) = delete;

//    Sensor(Input & input) :
//    ~Sensor();

    Acquisition acqMsg() const {
//        return Acquisition{m_spec.getResolution()};
        return make_acquisition(m_spec.getResolution());
//        return dynamic_cast<const Acquisition&>(m_spec.getResolution());
    }

  public:
    ///
    /// \brief getSpec
    /// \return
    ///
    const SensorSpec& getSpec() const {
        return m_spec;
    }

  protected:
    ///
    /// \brief m_spec
    ///
    SensorSpec m_spec;
};

/////////////////////////////////////////////////// INLINE
///////////////////////////////////////////////////////////////////////////

//inline const SensorSpec& Sensor::getSpec() const {
//    return m_spec;
//}

} // namespace sensor
} // namespace hub

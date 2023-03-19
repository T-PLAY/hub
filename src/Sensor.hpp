#pragma once

//#include "io/IO.hpp"
#include "SensorSpec.hpp"

namespace hub {

///
/// \brief The Sensor class
/// describes what a sensor can be, it's defined from the constant specifities of a sensor
/// and uses a communication bus to transmit the data.
/// \note Not user instantiable class.
///
class SRC_API Sensor
{
//  public:
  protected:
    ///
    /// \brief Sensor
    /// \param sensorSpec
    /// [in] user defined sensor spec
    ///
    explicit Sensor( const SensorSpec& sensorSpec );
    Sensor( const Sensor& sensor ) = delete;

    Sensor& operator=( const Sensor& sensor ) = delete;

    ~Sensor();

  public:

//    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& sensor );

    ///
    /// \brief getSpec
    /// \return
    ///
    const SensorSpec &getSpec() const;

  protected:

    ///
    /// \brief m_spec
    ///
    SensorSpec m_spec;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const SensorSpec &Sensor::getSpec() const
{
    return m_spec;
}

} // namespace hub

#pragma once

#include "IO/Interface.hpp"
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
  public:
    ///
    /// \brief Sensor
    /// \param sensorSpec
    /// [in] user defined sensor spec
    /// \param interface_
    /// [in] given interface
    ///
    Sensor( const SensorSpec&& sensorSpec, io::Interface& interface_ );
    Sensor( const Sensor& sensor ) = delete;

  protected:
    ~Sensor();

    Sensor& operator=( const Sensor& sensor ) = delete;

  public:
    ///
    /// \brief m_spec
    ///
    SensorSpec m_spec;

  protected:
    ///
    /// \brief m_interface
    ///
    io::Interface& m_interface;

  public:
    ///
    /// \brief getInterface
    /// \return
    ///
    io::Interface& getInterface() const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param sensor
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& sensor );
};

} // namespace hub

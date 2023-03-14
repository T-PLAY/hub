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
  public:
    ///
    /// \brief Sensor
    /// \param sensorSpec
    /// [in] user defined sensor spec
    ///
    explicit Sensor( const SensorSpec& sensorSpec );
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
    /// \brief m_io
    ///
//    io::IO& m_io;

  public:
    ///
    /// \brief getInterface
    /// \return
    ///
//    io::IO& getInterface() const;

    ///
    /// \brief operator <<
    /// \param os
    /// \param sensor
    /// \return
    ///
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& sensor );
};

} // namespace hub

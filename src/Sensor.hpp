#pragma once

#include "IO/Interface.hpp"
#include "SensorSpec.hpp"
//#include <memory>

//#ifdef interface
//#undef interface
//#endif

namespace hub {

///
/// \brief The Sensor class
/// communicate the data through an input/output interface.
///
class SRC_API Sensor
{
  public:
    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };

  protected:
    Sensor( const SensorSpec&& sensorSpec, io::Interface& interface_ );
    ~Sensor();

    Sensor( const Sensor& sensor )            = delete;
    Sensor& operator=( const Sensor& sensor ) = delete;

  public:
    SensorSpec m_spec;

  protected:
    io::Interface& m_interface;

  public:
    io::Interface& getInterface() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& sensor );
};

} // namespace hub

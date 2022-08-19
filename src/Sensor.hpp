#pragma once

#include "Acquisition.hpp"
#include "IO/Interface.hpp"
#include "SensorSpec.hpp"

namespace hub {

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
    Sensor( const SensorSpec& sensorSpec, io::Interface& interface );
//    Sensor( io::Interface& interface );
    ~Sensor();

    Sensor( const Sensor& sensor ) = delete;
//    Sensor( Sensor&& sensor )      = delete;

    Sensor& operator=( const Sensor& sensor ) = delete;
//    Sensor& operator=( Sensor&& sensor )      = delete;

  protected:
    const SensorSpec spec;
    io::Interface& m_interface;

  public:
    const SensorSpec& getHeader() const;
    io::Interface& getIO() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& sensor );
};


} // namespace hub

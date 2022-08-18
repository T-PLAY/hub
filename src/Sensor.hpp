#pragma once

#include "Acquisition.hpp"
#include "Header.hpp"
//#include "Socket.hpp"

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
    Sensor( const Header& header, io::Interface& interface );
    Sensor( io::Interface& interface );
    ~Sensor();

    Sensor( const Sensor& stream ) = delete;
    Sensor( Sensor&& stream )      = delete;

    Sensor& operator=( const Sensor& stream ) = delete;
    Sensor& operator=( Sensor&& stream )      = delete;

  protected:
    Header m_header;

    io::Interface& mInterface;

  public:
    const Header& getHeader() const;
    io::Interface& getIO() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& stream );
};

//////////////////////////// InputSensor ///////////////////////////////////////

class OutputSensor;

class SRC_API InputSensor : public Sensor
{
  public:
//    InputSensor( const std::string& sensorName, const std::string& syncSensorName = "" );
    template <class InterfaceT>
    InputSensor( InterfaceT&& interface );
    template <class InterfaceT>
    InputSensor( InterfaceT& interface ) = delete;

    ~InputSensor();

  public:
    Acquisition getAcquisition() const;
    std::vector<Acquisition> getAllAcquisitions();

  public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const InputSensor& inputSensor );
};

template <class InterfaceT>
InputSensor::InputSensor( InterfaceT&& interface ) :
    Sensor( {}, *std::move( new InterfaceT( std::move( interface ) ) ) ) {

    m_header.read( mInterface );
}

//////////////////////////// OutputSensor ///////////////////////////////////////

class SRC_API OutputSensor : public Sensor
{
  public:
//    OutputSensor( const Header& header, ClientSocket&& interface = ClientSocket() );
    template <class InterfaceT>
    OutputSensor( const Header& header, InterfaceT&& interface );
    template <class InterfaceT>
    OutputSensor( const Header& header, InterfaceT& interface ) = delete;

    OutputSensor( const OutputSensor& outputSensor ) = delete;
    OutputSensor( OutputSensor&& outputSensor )      = delete;

  public:
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

template <class InterfaceT>
OutputSensor::OutputSensor( const Header& header, InterfaceT&& interface ) :
    Sensor( header, *std::move( new InterfaceT( std::move( interface ) ) ) ) {
    mInterface.setupOutput( m_header.getSensorName() );

    m_header.write( mInterface );
}

} // namespace hub

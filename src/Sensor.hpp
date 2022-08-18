#pragma once

#include "Acquisition.hpp"
#include "Header.hpp"
#include "Socket.hpp"
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
    Sensor( const Header& header, IO& ioStream );
    Sensor( IO& ioStream );
    ~Sensor();

    Sensor( const Sensor& stream ) = delete;
    Sensor( Sensor&& stream )      = delete;

    Sensor& operator=( const Sensor& stream ) = delete;
    Sensor& operator=( Sensor&& stream )      = delete;

  protected:
    Header m_header;

    IO& mIO;

  public:
    const Header& getHeader() const;
    IO& getIO() const;

    SRC_API friend std::ostream& operator<<( std::ostream& os, const Sensor& stream );
};

//////////////////////////// InputSensor ///////////////////////////////////////

class OutputSensor;

class SRC_API InputSensor : public Sensor
{
  public:
    InputSensor( const std::string& sensorName, const std::string& syncSensorName = "" );
    template <class IOT>
    InputSensor( IOT&& ioStream );
    template <class IOT>
    InputSensor( IOT& ioStream ) = delete;

    ~InputSensor();

  public:
    Acquisition getAcquisition() const;
    std::vector<Acquisition> getAllAcquisitions();

  public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const InputSensor& inputSensor );
};

template <class IOT>
InputSensor::InputSensor( IOT&& ioStream ) :
    Sensor( {}, *std::move( new IOT( std::move( ioStream ) ) ) ) {

    m_header.read( mIO );
}

//////////////////////////// OutputSensor ///////////////////////////////////////

class SRC_API OutputSensor : public Sensor
{
  public:
    OutputSensor( const Header& header, ClientSocket&& ioStream = ClientSocket() );
    template <class IOT>
    OutputSensor( const Header& header, IOT&& ioStream );
    template <class IOT>
    OutputSensor( const Header& header, IOT& ioStream ) = delete;

    OutputSensor( const OutputSensor& outputSensor ) = delete;
    OutputSensor( OutputSensor&& outputSensor )      = delete;

  public:
    void operator<<( const Acquisition& acquisition ) const;

  private:
};

template <class IOT>
OutputSensor::OutputSensor( const Header& header, IOT&& ioStream ) :
    Sensor( header, *std::move( new IOT( std::move( ioStream ) ) ) ) {
    mIO.setupOutput( m_header.getSensorName() );

    m_header.write( mIO );
}

} // namespace hub

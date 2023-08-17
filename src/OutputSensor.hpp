#pragma once

#include <type_traits>

#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"
#include "io/output/Output.hpp"
#include "Traits.hpp"

// user friendly useless includes
// #include "io/Output.hpp"
//#include "io/File.hpp"
#include "io/output/OutputFile.hpp"
#include "io/output/OutputStream.hpp"
#include "io/output/OutputStreamServer.hpp"
#include "io/output/OutputMemory.hpp"

namespace hub {

///
/// \brief The OutputSensor class
/// describes what a sensor physically is.
/// A sensor measures physical information from the environment around us.
/// We declare a sensor with its intrinsic data and the transportable
/// data format that the sensor driver proposes to export.
/// The sensor transmits volatile and potentially unrecordable data.
/// This class allows the data to be transmitted without being recorded
/// so that it can be used in real time on various network machines.
///
/// todo: template class
class SRC_API OutputSensor : public Sensor
{
  public:
    ///
    /// \brief OutputSensor
    /// is called when you want to share data sensor
    /// \param sensorSpec
    /// able client application to understand the data
    /// \param output
    /// is the communication bus you want to use
    ///
    template <
        class SensorSpec = hub::SensorSpec,
        class Output,
        typename = typename std::enable_if<std::is_base_of<io::Output, Output>::value &&
                                           !std::is_same<Output, output::OutputStreamServer>::value &&
                                           !std::is_same<Output, net::ClientSocket>::value
                                           >::type>
    OutputSensor( SensorSpec&& sensorSpec, Output&& output ) :

        Sensor( std::forward<SensorSpec>( sensorSpec ) ),
        m_output( new Output( std::move( output ) ) ) {

        static_assert( std::is_base_of<io::Output, Output>::value, "not a base class" );
        static_assert( !std::is_same<output::OutputStreamServer, Output>::value, "not outputStream class" );
        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not clientSocket class" );

        m_output->write( m_spec );
    }

    ///
    /// \brief OutputSensor
    /// OutputStream class init thread during its construction.
    /// To prevent non movable pointers within the thread,
    /// we construct OutputStream in OutputSensor place to instanciate correctly.
    /// \param sensorSpec
    /// \param args
    ///
    template <class SensorSpec = hub::SensorSpec,
              class Output     = output::OutputStream,
              class... Args,
              typename = typename std::enable_if<std::is_same<output::OutputStream, Output>::value
#if ( __cplusplus < 201703L )
                                                 && ( sizeof...( Args ) != 1 ||
                                                      !is_one_of<Output, Args...>::value )
#else
                                                 && ( sizeof...( Args ) != 1 ||
                                                      !( std::is_same<Output, Args> {} || ... ) )
#endif
                                                 >::type>
    OutputSensor( SensorSpec&& sensorSpec, Args&&... args ) :
        Sensor( std::forward<SensorSpec>( sensorSpec ) ),
        m_output( new Output( std::forward<Args>( args )... ) ) {
        static_assert( std::is_base_of<io::Output, Output>::value, "not a base class" );
        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not clientSocket class" );

        m_output->write( m_spec );
    }

    ////        io::Output(std::move(output)),
    ////        m_output( new Output( std::forward<Args>(args)... ) )

    ////        Output::write(m_spec);

    template <class Output>
    OutputSensor( const SensorSpec&& sensorSpec, Output& output ) = delete;

    OutputSensor( const OutputSensor& outputSensor ) = delete;
    OutputSensor operator=( const OutputSensor& outputSensor ) = delete;

    ///
    /// \brief OutputSensor
    /// \param outputSensor
    ///
    OutputSensor( OutputSensor&& outputSensor );

    ~OutputSensor();

    ///
    /// \brief operator <<
    /// is used when you want to share sensor data.
    /// \param acquisition
    /// to send through the communication bus.
    /// \warning The acquisition needs to be compatible with
    /// the internal sensor specification resolution and format.\n
    /// sensorSpec.resolutions == { acq.measures[0].resolution, acq.measures[1].resolution, ... }
    /// \exception "used interface exception"
    /// when communication is broken.
    ///

    void operator<<( const Acquisition& acquisition ) const;

    ///
    /// \brief getOutput
    /// \return
    ///
    io::Output& getOutput() const;

  private:
    std::unique_ptr<io::Output> m_output;
    bool m_moved = false;
};

} // namespace hub

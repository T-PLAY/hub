#pragma once

#include <type_traits>

#include "Sensor.hpp"
#include "SensorSpec.hpp"
#include "Acquisition.hpp"
#include "Traits.hpp"

#include "io/output/Output.hpp"
// user friendly useless includes
#include "io/output/OutputFile.hpp"
#include "io/output/OutputStream.hpp"
#include "io/output/OutputMemory.hpp"
#include "io/Memory.hpp"

//#include "net/ClientSocket.hpp"


namespace hub {
namespace sensor {


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
        class SensorSpec = sensor::SensorSpec,
          class OutputT>
//          ,
//          typename = typename std::enable_if<std::is_base_of<Output, Output>::value &&
//#ifdef HUB_BUILD_SERVER
//                                           !std::is_same<OutputT, output::OutputStreamServer>::value
//              &&
//#endif
//                                           !std::is_same<Output, net::ClientSocket>::value
//                                           >::type>
    OutputSensor( SensorSpec&& sensorSpec, OutputT&& output ) :

        Sensor( std::forward<SensorSpec>( sensorSpec ) ),
//          m_output(std::forward<OutputT>(output))
        m_output( new OutputT( std::move( output ) ) )
      {

        static_assert( std::is_base_of<Output, OutputT>::value, "not a base class" );
//#ifdef HUB_BUILD_SERVER
//        static_assert( !std::is_same<output::OutputStreamServer, Output>::value, "not outputStream class" );
//#endif
//        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not clientSocket class" );

        m_output->write( m_spec );
    }


//#ifdef HUB_BUILD_SERVER

////#define output::OutputStream(_streamName, _ipv4, _port) \
////    _streamName, _ipv4, _port

//    ///
//    /// \brief OutputSensor
//    /// OutputStream class init thread during its construction.
//    /// To prevent non movable pointers within the thread,
//    /// we construct OutputStream in OutputSensor place to instanciate correctly.
//    /// \param sensorSpec
//    /// \param args
//    ///
//    template <class SensorSpec = SensorSpec,
//              class... Args,
////             class Output,
////             class OutputT = Output,
////             typename = typename std::enable_if<std::is_base_of<Output, OutputT>::value
//              class OutputT     = output::OutputStreamServer,
//              typename = typename std::enable_if<std::is_same<output::OutputStreamServer, OutputT>::value
//#if ( __cplusplus < 201703L )
//                                                 && ( sizeof...( Args ) != 1 ||
//                                                      !is_one_of<OutputT, Args...>::value )
//#else
//                                                 && ( sizeof...( Args ) != 1 ||
//                                                      !( std::is_same<OutputT, Args> {} || ... ) )
//#endif
//                                                 >::type>
//    OutputSensor( SensorSpec&& sensorSpec, Args&&... args ) :
//        Sensor( std::forward<SensorSpec>( sensorSpec ) ),
//        m_output( new OutputT( std::forward<Args>( args )... ) )
////        m_output( std::make_unique<OutputT>( std::forward<Args>( args )... ) )
////        m_output( std::forward<Args>( args )...  )
//    {
//        static_assert( std::is_base_of<Output, OutputT>::value, "not a base class" );
////        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not clientSocket class" );

//        m_output->write( m_spec );
//    }

//#define OutputStream( ... ) \
//    __VA_ARGS__

////#define OutputStream( args... ) \
////    args
////#define OutputStream( sensorName, ipv4, port ) \
////    sensorName, ipv4, port
////#define OutputStreamServer( _params ) \
////    _params
////#define OutputStream( _params ) \
////    _params
//#endif



    ////        Output(std::move(output)),
    ////        m_output( new Output( std::forward<Args>(args)... ) )

    ////        Output::write(m_spec);

    template <class OutputT>
    OutputSensor( const SensorSpec&& sensorSpec, OutputT& output ) = delete;

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
    Output& getOutput() const;

  private:
    std::unique_ptr<Output> m_output;
//    Output & m_output;
    bool m_moved = false;
};

} // namespace sensor
} // namespace hub

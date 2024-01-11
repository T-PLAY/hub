/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28

#pragma once

//// #include <type_traits>

//// #include "io/output/Output.hpp"
// #include "Sensor.hpp"
// #include "SensorSpec.hpp"
//// #include "Acquisition.hpp"
//// #include "core/Traits.hpp"

// user friendly useless includes
// #include "core/Utils.hpp"
#include "io/output/OutputFile.hpp"
#include "io/output/OutputStream.hpp"

//// #include "io/output/OutputFile.hpp"
//// #include "io/output/OutputStream.hpp"
//// #include "io/output/OutputMemory.hpp"
//// #include "io/Memory.hpp"
// #include "core/Output.hpp"

// #include <span>

#include "core/Macros.hpp"
// #include "core/Matrix.hpp"
#include "core/Output.hpp"

#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

//// #include "net/ClientSocket.hpp"
// #include "Acquisition.hpp"
////#include "Format.hpp"
// #include "Measure.hpp"
// #include "Resolution.hpp"

namespace hub {
namespace sensor {

/////
///// \brief The OutputSensor class
///// describes what a sensor physically is.
///// A sensor measures physical information from the environment around us.
///// We declare a sensor with its intrinsic data and the transportable
///// data format that the sensor driver proposes to export.
///// The sensor transmits volatile and potentially unrecordable data.
///// This class allows the data to be transmitted without being recorded
///// so that it can be used in real time on various network machines.
/////
///// todo: template class
//// template <typename Output, typename Measures, typename Acquisition = Acquisition<Measures>>

// template <typename Output>
class OutputSensor : public Sensor
{
  public:
    using Sensor::acqMsg;

    // OutputSensor( const SensorSpec& sensorSpec,
    //               const std::string& streamName,
    //               int port                = HUB_SERVICE_PORT,
    //               const std::string& ipv4 = HUB_SERVICE_IPV4 ) :
    //     Sensor( sensorSpec ),
    //     m_output( *(
    //         new output::OutputStream( io::make_header( sensorSpec ), streamName, port, ipv4 ) )
    //         ),
    //     m_outputOwner( true ) {

    //     assert( m_spec.getResolution().nType() > 0 );
    // }

    template <class OutputT = output::OutputStream,
#if CPP_VERSION >= 20
                  requires std::is_base_of_v<hub::Output, OutputT>
#endif
              typename = typename std::enable_if_t<std::is_base_of_v<hub::Output, OutputT>>,
              class... Args>
              OutputSensor( const SensorSpec& sensorSpec, const Args&... args ) :
        Sensor( sensorSpec ),
        m_output( *( new OutputT( io::make_header( sensorSpec ), args... ) ) ),
        m_outputOwner( true ) {
        static_assert( std::is_base_of_v<hub::Output, OutputT> );

        assert( m_spec.getResolution().nType() > 0 );
    }

    template <class OutputT,
#if CPP_VERSION >= 20
                  requires std::is_base_of_v<hub::Output, OutputT>
#endif
              typename = typename std::enable_if_t<std::is_base_of_v<hub::Output, OutputT>>>
              OutputSensor( const SensorSpec& sensorSpec, OutputT& output ) :
        Sensor( sensorSpec ), m_output( output ) {
        static_assert( std::is_base_of_v<hub::Output, OutputT> );

#ifdef DEBUG
        const auto& header = output.getHeader();
        hub::io::Memory memory( header.getUserDefined() );
        SensorSpec headerSpec;
        memory.read( headerSpec );
        assert( sensorSpec == headerSpec );
#endif

        assert( m_spec.getResolution().nType() > 0 );
        // m_output.setRetain( true );
        // m_output.write( m_spec );
        // m_output.setRetain( false );
    }

    template <class OutputT,
#if CPP_VERSION >= 20
                  requires std::is_base_of_v<hub::Output, OutputT>
#endif
              typename = typename std::enable_if_t<std::is_base_of_v<hub::Output, OutputT>>>
              OutputSensor( const SensorSpec& sensorSpec, OutputT&& output ) :
        Sensor( sensorSpec ),
        m_output( *( new OutputT( std::move( output ) ) ) ),
        m_outputOwner( true ) {
        static_assert( std::is_base_of_v<hub::Output, OutputT> );

#ifdef DEBUG
        const auto& header = dynamic_cast<const OutputT&>( m_output ).getHeader();
        hub::io::Memory memory( header.getUserDefined() );
        SensorSpec headerSpec;
        memory.read( headerSpec );
        assert( sensorSpec == headerSpec );
#endif

        assert( m_spec.getResolution().nType() > 0 );
    }

    ~OutputSensor() {
        if ( m_outputOwner ) { delete &m_output; }
    }

    void operator<<( const Acquisition& acq ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(const Acquisition&) : " << acq << std::endl;
#endif
        assert( m_spec.getResolution() == acq.getResolution() );
        m_output.write( acq.data(), acq.size() );
    }

    Output& getOutput() const { return m_output; }

    //    Acquisition acq() const {
    //        return Acquisition{m_spec.getResolutions()};
    //    }

  private:
    Output& m_output;
    bool m_outputOwner = false;
    //    Measures m_measures;
};

/////////////////////////////////////// TEMPLATE //////////////////////////////////////////////////

template <class Resolution, class Output = output::OutputStream>
class OutputSensorT : public Sensor
{
    static_assert( std::is_base_of_v<hub::Output, Output> );
    using Acquisition = AcquisitionT<Resolution>;
    // using Acq = Acquisition;
  public:
    template <class... Args>
    OutputSensorT( const SensorSpec& sensorSpec, const Args&... args ) :
        Sensor( sensorSpec ),
        m_output( *( new Output( hub::io::make_header( sensorSpec ), args... ) ) ),
        m_outputOwner( true ) {

#ifdef DEBUG
        if constexpr ( isMatrix<Resolution> ) {
            assert( Resolution().getMatrix() == m_spec.getResolution() );
            // assert( m_spec.getResolution() == Resolution() );
            // assert( Resolution() == m_spec.getResolution() );
        }
        else { assert( make_matrix<Resolution>() == m_spec.getResolution() ); }
#endif
    }

    //     OutputSensorT( Output& output ) : m_output( output ) {

    //         auto& header = output.getHeader();
    //         hub::io::Memory memory( header.getUserDefined() );
    //         memory.read( m_spec );

    //         // if constexpr ( isMatrix<Resolution> ) { m_spec.setResolution(
    //         Resolution().getMatrix() );
    //         // } else { m_spec.setResolution( make_matrix<Resolution>() ); }
    // #ifdef DEBUG
    //         if constexpr ( isMatrix<Resolution> ) {
    //             assert( Resolution().getMatrix() == m_spec.getResolution() );
    //         }
    //         else { assert( make_matrix<Resolution>() == m_spec.getResolution() ); }
    // #endif
    //     }

    //     OutputSensorT( Output&& output ) :
    //         m_output( *( new Output( std::move( output ) ) ) ), m_outputOwner( true ) {

    //         const auto& header = dynamic_cast<const Output&>( m_output ).getHeader();
    //         hub::io::Memory memory( header.getUserDefined() );
    //         memory.read( m_spec );

    // #ifdef DEBUG
    //         if constexpr ( isMatrix<Resolution> ) {
    //             assert( Resolution().getMatrix() == m_spec.getResolution() );
    //         }
    //         else { assert( make_matrix<Resolution>() == m_spec.getResolution() ); }
    // #endif
    //     }

    ~OutputSensorT() {
        if ( m_outputOwner ) { delete &m_output; }
    }

    void operator<<( const Acquisition& acq ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(const Acquisition&) : " << acq << std::endl;
#endif
        m_output.write( acq.data(), acq.size() );
    }

    Acquisition acqMsg() const { return Acquisition(); }

    Output& getOutput() const { return m_output; }

  private:
    Output& m_output;
    bool m_outputOwner = false;

}; // end OutputSensorT

} // namespace sensor

template <class Output = output::OutputStream, class... Args>
inline auto make_outputSensor( const sensor::SensorSpec& sensorSpec, const Args&... args ) {
    // return sensor::OutputSensorT<Resolution>( sensorSpec, args... );
    return sensor::OutputSensor( sensorSpec, Output( io::make_header( sensorSpec ), args... ) );
}

template <class Resolution, class... Args>
inline auto make_outputSensorT( const sensor::SensorSpec& sensorSpec, const Args&... args ) {
    return sensor::OutputSensorT<Resolution>( sensorSpec, args... );
}

} // namespace hub

////template <typename Output, typename Measures>
////// template <typename Measures>
////class SRC_API OutputSensorT : public Sensor
////{
////  public:
////    ///
////    /// \brief OutputSensorT
////    /// is called when you want to share data sensor
////    /// \param sensorSpec
////    /// able client application to understand the data
////    /// \param output
////    /// is the communication bus you want to use
////    ///
////    template <typename OutputT, typename SensorSpec = sensor::SensorSpec>
////    //          class OutputT>
////    //          ,
////    //          typename = typename std::enable_if<std::is_base_of<Output, Output>::value &&
////    // #ifdef HUB_BUILD_SERVER
////    //                                           !std::is_same<OutputT,
////    //                                           output::OutputStreamServer>::value
////    //              &&
////    // #endif
////    //                                           !std::is_same<Output, net::ClientSocket>::value
////    //                                           >::type>
////    OutputSensorT( OutputT&& output, SensorSpec&& sensorSpec ) :
////        m_output( std::forward<OutputT>( output ) ),
////        Sensor( std::forward<SensorSpec>( sensorSpec ) )
////    //          m_output(std::forward<OutputT>(output))
////    //        m_output( new OutputT( std::move( output ) ) )
////    {
////        //        static_assert( std::is_base_of<Output, OutputT>::value, "not a base class" );
////        // #ifdef HUB_BUILD_SERVER
////        //        static_assert( !std::is_same<output::OutputStreamServer, Output>::value, "not
////        //        outputStream class" );
////        // #endif
////        //        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not
/// clientSocket /        //        class" );

//////        const auto nMeasure = Measures::nMeasure();
//////        for (int iMeasure = 0; iMeasure < nMeasure; ++iMeasure) {
//////            const auto nDim

//////        }

//////        Measures measures;
//////        m_output.write(Measures);

////        //        m_output.write( m_spec );
////    }

////    ////        m_output( new Output( std::forward<Args>(args)... ) )
////    ////        Output::write(m_spec);

////    template <class OutputT>
////    OutputSensorT( OutputT& output, const SensorSpec&& sensorSpec ) = delete;

////    OutputSensorT( const OutputSensorT& outputSensor )           = delete;
////    OutputSensorT operator=( const OutputSensorT& outputSensor ) = delete;

////    ///
////    /// \brief OutputSensorT
////    /// \param outputSensor
////    ///
////    OutputSensorT( OutputSensorT&& outputSensor )           = delete;
////    OutputSensorT operator=( OutputSensorT&& outputSensor ) = delete;

////    //    ~OutputSensorT();

////    ///
////    /// \brief operator <<
////    /// is used when you want to share sensor data.
////    /// \param acquisition
////    /// to send through the communication bus.
////    /// \warning The acquisition needs to be compatible with
////    /// the internal sensor specification resolution and format.\n
////    /// sensorSpec.resolutions == { acq.measures[0].resolution, acq.measures[1].resolution, ...
///} /    /// \exception "used interface exception" /    /// when communication is broken. /    ///

////    // todo acq
////    //    template <typename Acquisition>
//////    void operator<<( const AcquisitionT<Measures>& acquisition ) { m_output.write( acquisition
///); }

////    //    ///
////    //    /// \brief getOutput
////    //    /// \return
////    //    ///
////    Output& getOutput() const { return m_output; }

////  private:
////    //    std::unique_ptr<Output> m_output;
////    Output& m_output;
////    //    Output & m_output;
////    //    bool m_moved = false;
////};

//// template <typename ...T>
//// inline auto make_outputSensor(T&&... t) {
////     return OutputSensor<T...>(std::forward<OutputSensor<T...>>(t...));
//// }

//// template <typename OutputT, typename SensorSpec = sensor::SensorSpec>
//// inline constexpr auto make_outputSensor( OutputT&& output, SensorSpec&& sensorSpec ) {
////     using Measure1 = Measure<Format::RGB8, 640, 480>;
////     return OutputSensor<OutputT, Measure1>( std::forward<OutputT>( output ),
////                                             std::forward<SensorSpec>( sensorSpec ) );
////     //          class OutputT>
////     //          ,
////     //          typename = typename std::enable_if<std::is_base_of<Output, Output>::value &&
////     // #ifdef HUB_BUILD_SERVER
////     //                                           !std::is_same<OutputT,
////     //                                           output::OutputStreamServer>::value
////     //              &&
////     // #endif
////     //                                           !std::is_same<Output,
/// net::ClientSocket>::value /     //                                           >::type> /     //
/// OutputSensor( OutputT&& output, SensorSpec&& sensorSpec ) : /     //    return
/// std::forward<T>(t); / }

//// #ifdef HUB_BUILD_SERVER

//////#define output::OutputStream(_streamName, _ipv4, _port) \
//////    _streamName, _ipv4, _port

////    ///
////    /// \brief OutputSensor
////    /// OutputStream class init thread during its construction.
////    /// To prevent non movable pointers within the thread,
////    /// we construct OutputStream in OutputSensor place to instanciate correctly.
////    /// \param sensorSpec
////    /// \param args
////    ///
////    template <class SensorSpec = SensorSpec,
////              class... Args,
//////             class Output,
//////             class OutputT = Output,
//////             typename = typename std::enable_if<std::is_base_of<Output, OutputT>::value
////              class OutputT     = output::OutputStreamServer,
////              typename = typename std::enable_if<std::is_same<output::OutputStreamServer,
////              OutputT>::value
//// #if ( __cplusplus < 201703L )
////                                                 && ( sizeof...( Args ) != 1 ||
////                                                      !is_one_of<OutputT, Args...>::value )
//// #else
////                                                 && ( sizeof...( Args ) != 1 ||
////                                                      !( std::is_same<OutputT, Args> {} || ... )
///) / #endif /                                                 >::type> /    OutputSensor(
/// SensorSpec&& sensorSpec, Args&&... args ) : /        Sensor( std::forward<SensorSpec>(
/// sensorSpec ) ), /        m_output( new OutputT( std::forward<Args>( args )... ) )
//////        m_output( std::make_unique<OutputT>( std::forward<Args>( args )... ) )
//////        m_output( std::forward<Args>( args )...  )
////    {
////        static_assert( std::is_base_of<Output, OutputT>::value, "not a base class" );
//////        static_assert( !std::is_same<net::ClientSocket, Output>::value, "not clientSocket
/// class"
/////);

////        m_output->write( m_spec );
////    }

////#define OutputStream( ... ) \
////    __VA_ARGS__

//////#define OutputStream( args... ) \
//////    args
//////#define OutputStream( sensorName, ipv4, port ) \
//////    sensorName, ipv4, port
//////#define OutputStreamServer( _params ) \
//////    _params
//////#define OutputStream( _params ) \
//////    _params
//// #endif
//////        Output(std::move(output)),

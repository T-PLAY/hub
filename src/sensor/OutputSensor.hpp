/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/09/28

#pragma once

//// #include <type_traits>

//// #include "io/output/Output.hpp"
//// #include "Acquisition.hpp"
//// #include "core/Traits.hpp"

#include "core/Base.hpp"
#include "core/Output.hpp"
#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "SensorSpec.hpp"

#include "io/output/OutputFile.hpp"
#include "io/output/OutputStream.hpp"
//// #include "io/output/OutputFile.hpp"
//// #include "io/output/OutputStream.hpp"
//// #include "io/output/OutputMemory.hpp"
//// #include "io/Memory.hpp"



//// #include "net/ClientSocket.hpp"
////#include "Format.hpp"

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
/// template <typename Output, typename Measures, typename Acquisition = Acquisition<Measures>>
/// Todo clean all construction, template args ... are not user friendly
class OutputSensor : public Sensor
{
  public:
    // using Sensor::acqMsg;

    // OutputSensor( const SensorSpec& sensorSpec,

        ///
        /// \brief OutputSensor
        /// \param sensorSpec
        /// \param args
        ///
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

    ///
    /// \brief OutputSensor
    /// \param sensorSpec
    /// \param output
    ///
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
    }

    ///
    /// \brief OutputSensor
    /// \param sensorSpec
    /// \param output
    ///
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

    ///
    /// \brief operator <<
    /// \param acq
    ///
    void operator<<( const Acquisition& acq ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(const Acquisition&) : " << acq << std::endl;
#endif
        assert( m_spec.getResolution() == acq.getResolution() );
        m_output.write( acq.data(), acq.size() );
    }

    ///
    /// \brief getOutput
    /// \return
    ///
    Output& getOutput() const { return m_output; }

    ///
    /// \brief acqMsg
    /// \return
    ///
    Acquisition acqMsg() const { return make_acquisition( m_spec.getResolution() ); }

    ///
    /// \brief fill
    /// \param ts
    ///
    template <class Container,
              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
    void fill(Container & ts) {
        static_assert(std::is_same_v<T, Acquisition>);
        for (const auto & t : ts) {
            m_output.write(t.data(), t.size());
        }

    }

  private:
    Output& m_output;
    bool m_outputOwner = false;
};

/////////////////////////////////////// TEMPLATE //////////////////////////////////////////////////

///
/// \brief The OutputSensorT class
///
template <class Resolution, class Output = output::OutputStream>
// class OutputSensorT : public Sensor
class OutputSensorT : protected Sensor
{
  public:
    using Sensor::getSpec;
    static_assert( std::is_base_of_v<hub::Output, Output> );

    ///
    /// \brief Acquisition
    ///
    using Acquisition = AcquisitionT<Resolution>;

    ///
    /// \brief OutputSensorT
    /// \param sensorSpec
    /// \param args
    ///
    template <class... Args>
    OutputSensorT( const SensorSpec& sensorSpec, const Args&... args ) :
        Sensor( sensorSpec ),
        m_output( *( new Output( hub::io::make_header( sensorSpec ), args... ) ) ),
        m_outputOwner( true ) {

#ifdef DEBUG
        if constexpr ( isMatrix<Resolution> ) {
            assert( Resolution().getMatrix() == m_spec.getResolution() );
        }
        else {
            assert( make_matrix<Resolution>() == m_spec.getResolution() );
        }
#endif
    }

    ~OutputSensorT() {
        if ( m_outputOwner ) { delete &m_output; }
    }

    ///
    /// \brief operator <<
    /// \param acq
    ///
    void operator<<( const Acquisition& acq ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(const Acquisition&) : " << acq << std::endl;
#endif
        m_output.write( acq.data(), acq.size() );
    }

    ///
    /// \brief acqMsg
    /// \return
    ///
    Acquisition acqMsg() const { return Acquisition(); }

    ///
    /// \brief getOutput
    /// \return
    ///
    Output& getOutput() const { return m_output; }

  private:
    Output& m_output;
    bool m_outputOwner = false;

}; // end OutputSensorT

} // namespace sensor

///
/// \brief make_outputSensor
/// \param sensorSpec
/// \param args
/// \return
///
template <class Output = output::OutputStream, class... Args>
inline auto make_outputSensor( const sensor::SensorSpec& sensorSpec, const Args&... args ) {
    return sensor::OutputSensor( sensorSpec, Output( io::make_header( sensorSpec ), args... ) );
}

///
/// \brief make_outputSensorT
/// \param sensorSpec
/// \param args
/// \return
///
template <class Resolution, class... Args>
inline auto make_outputSensorT( const sensor::SensorSpec& sensorSpec, const Args&... args ) {
    return sensor::OutputSensorT<Resolution>( sensorSpec, args... );
}

} // namespace hub

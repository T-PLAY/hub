#pragma once

#include "core/Input.hpp"

#include "Acquisition.hpp"
#include "Sensor.hpp"
// #include "core/Traits.hpp"
//// #include "core/Tuple.hpp"

//  user friendly useless includes
#include "io/input/InputFile.hpp"
#include "io/input/InputStream.hpp"

//// #include "io/File.hpp"
//// #include "io/input/InputFile.hpp"
//// #include "io/input/InputStream.hpp"
//// #include "io/input/InputSyncStream.hpp"
//// #include "io/input/InputMemory.hpp"
//// #include "io/Memory.hpp"

//// #include "net/ClientSocket.hpp"
///

namespace hub {
namespace sensor {

///
/// \brief The InputSensorT class
/// represents the remote sensor.
/// From any communication bus (file, RAM, socket) you have access to all the information specific
/// to the sensors as well as the acquisition flow coming from the physical sensor.
/// This class allows client applications to completely abstract from the notion
/// of sensor and to be interested only in the carried data.
/// This enables several applications to work simultaneously on the same data in real time.
/// This also avoids the need to manage the drivers of each of the manufacturers of the sensors you
/// wish to use.
///
/// todo: template class
///

// template <typename Input>
class InputSensor : public Sensor
{
    void initSensorSpecFromInputs() {
        // std::cout << "[InputSensor] reading specs ..." << std::endl;
        m_specs.resize( m_inputs.size() );
        for ( int i = 0; i < m_inputs.size(); ++i ) {
            // std::cout << "[InputSensor] reading spec " << i << " : " << m_specs.at(i) <<
            // std::endl;
            auto* input = m_inputs.at( i );
            input->read( m_specs.at( i ) );
            m_spec += m_specs.at( i );
        }
        // std::cout << "[InputSensor] specs readed" << std::endl;
        // std::cout << "[InputSensor] spec: " << m_spec << std::endl;
    }

  public:
    using Sensor::acq;

    InputSensor( Input& input ) : Sensor( SensorSpec {} ), m_inputs( { &input } ) {
        initSensorSpecFromInputs();
    }

    InputSensor( Input& input, Input& input2 ) :
        Sensor( SensorSpec {} ), m_inputs( { &input, &input2 } ) {
        initSensorSpecFromInputs();
    }

    template <class InputT>
        requires std::is_base_of_v<Input, InputT>
    InputSensor( InputT&& input ) : Sensor( SensorSpec {} ) {
        m_inputs.push_back( new InputT( std::move( input ) ) );
        m_inputOwner = true;
        initSensorSpecFromInputs();
    }

    template <class InputT>
        requires std::is_base_of_v<Input, InputT>
    InputSensor( InputT&& input, InputT&& input2 ) : Sensor( SensorSpec {} ) {
        m_inputs.push_back( new InputT( std::move( input ) ) );
        m_inputs.push_back( new InputT( std::move( input2 ) ) );
        m_inputOwner = true;
        initSensorSpecFromInputs();
    }

    ~InputSensor() {
        if ( m_inputOwner ) {
            // std::cout << "[InputSensor] ~InputSensor()" << std::endl;
            auto it = m_inputs.begin();
            while ( it != m_inputs.end() ) {
                auto cur = it;
                it++;
                delete ( *cur );
            }
        }
    }

    void operator>>( Acquisition& acq ) {
        assert( m_spec.getResolution() == acq.getResolution() );

        if ( m_inputs.size() == 1 ) {
            Input& input = *m_inputs.at( 0 );
            input.read( acq.data(), acq.size() );
        }
        else {
            Input& leftInput  = *m_inputs.at( 0 );
            Input& rightInput = *m_inputs.at( 1 );

            Acquisition rightAcq = make_acquisition( m_specs.at( 1 ).getResolution() );
            rightInput.read( rightAcq );

            auto& leftLastAcqs = m_lastAcqs;
            assert( leftLastAcqs.size() < 20 );
            Acquisition leftAcq = make_acquisition( m_specs.at( 0 ).getResolution() );

            if ( leftLastAcqs.empty() ) {
                leftInput.read( leftAcq );
                leftLastAcqs.push_back( leftAcq.clone() );
            }

            while ( rightAcq.getStart() < leftLastAcqs.front().getStart() ) {
#ifdef HUB_DEBUG_INPUT
                std::cout << "[InputSensor] operator>>(InputSensor&) shift rightAcq : " << rightAcq
                          << std::endl;
#endif
                assert( !rightInput.isEnd() );
                rightInput.read( rightAcq );
            }

            while ( leftLastAcqs.back().getStart() < rightAcq.getStart() && !leftInput.isEnd() ) {
                assert( !leftInput.isEnd() );
                leftInput.read( leftAcq );
                leftLastAcqs.push_back( leftAcq.clone() );
            }

            while ( leftLastAcqs.size() > 2 ) {
                leftLastAcqs.pop_front();
            }

            const auto& leftBeforeRightAcq = leftLastAcqs.front();
            const auto& leftAfterRightAcq  = leftLastAcqs.back();

            assert( leftInput.isEnd() || leftBeforeRightAcq.getStart() <= rightAcq.getStart() );
            assert( leftInput.isEnd() || rightAcq.getStart() <= leftAfterRightAcq.getStart() );

            const auto& closestAcq =
                ( std::abs( leftBeforeRightAcq.getStart() - rightAcq.getStart() ) >
                  std::abs( leftAfterRightAcq.getStart() - rightAcq.getStart() ) )
                    ? ( leftAfterRightAcq )
                    : ( leftBeforeRightAcq );

            auto sync_acq    = closestAcq << rightAcq;
            sync_acq.start() = rightAcq.getStart();
            sync_acq.end()   = rightAcq.getEnd();
            assert( sync_acq.getResolution() == acq.getResolution() );

            acq = std::move( sync_acq );
        }

#ifdef HUB_DEBUG_INPUT
        std::cout << HEADER << "read(Acquisition&) : " << acq << std::endl;
#endif
    }

    const Input& getInput() const { return *m_inputs.at( 0 ); }
    Input& getInput() { return *m_inputs.at( 0 ); }

  private:
    std::vector<Input*> m_inputs;
    // std::vector<bool> m_inputOwners;
    bool m_inputOwner = false;
    std::list<sensor::Acquisition> m_lastAcqs;
    std::vector<hub::sensor::SensorSpec> m_specs;
};

///////////////////////////////////// TEMPLATE ////////////////////////////////////

////template <typename Input, typename Measures>
////class SRC_API InputSensorT : public Sensor
////{
////  public:
////    //    using Acquisition = Acquisition<Measures>;

////    ///
////    /// \brief InputSensorT
////    /// is called when you want to get data sensor
////    /// \param input
////    /// is the communication bus you want to use
////    ///
////    template <class InputT>
////    //              typename = typename std::enable_if<std::is_base_of<Input, Input>::value
////    //                                                 && !std::is_same<Input,
////    //                                                 net::ClientSocket>::value
////    //                                                 >::type>
////    explicit InputSensorT( InputT&& input ) :

////        m_input( std::forward<InputT>( input ) ),
////        Sensor( sensor::SensorSpec {} )
////    //        m_input( new Input( std::move( input ) ) )
////    {
////        //        static_assert( std::is_base_of<Input, Input>::value, "not a base class" );
////        //        static_assert( !std::is_same<net::ClientSocket, Input>::value, "not
/// clientSocket /        //        class" );

////        //        m_input.read( m_spec );
////    }

////    template <class InputT>
////    //              ,
////    //              typename = typename std::enable_if<std::is_base_of<Input,
/// Input>::value>::type> /    explicit InputSensorT( const InputT& input ) = delete;

////    InputSensorT( const InputSensorT& inputSensor )           = delete;
////    InputSensorT operator=( const InputSensorT& inputSensor ) = delete;

////    InputSensorT( InputSensorT&& inputSensor )           = delete;
////    InputSensorT operator=( InputSensorT&& inputSensor ) = delete;

////    ///
////    /// \brief InputSensorT
////    /// \param inputSensor
////    ///
////    //    InputSensorT( InputSensorT&& inputSensor );

////    //    ~InputSensorT();

////    template <typename T>
////    void operator>>( T& t ) {
////        m_input.read( t );
////    }

////  public:
////    //    Acquisition<Measures> getAcq() {
////    //        Acquisition<Measures> acq;
////    //        m_input.read(acq);
////    //        return acq;
////    //    }

////    // todo acq
////    //    ///
////    //    /// \brief operator >>
////    //    /// \param acquisition
////    //    ///
////    //    void operator>>( Acquisition& acquisition );

////    //    ///
////    //    /// \brief operator >>
////    //    /// \param inputSensor
////    //    /// \return
////    //    ///
////    //    Acquisition operator>>( InputSensorT& inputSensor );

////    //    ///
////    //    /// \brief getAllAcquisitions
////    //    /// can be an asynchronous function depending of the used interface.
////    //    /// \return
////    //    /// all acquisitions coming from the interface.
////    //    ///
////    //    std::vector<Acquisition> getAllAcquisitions();

////    //    ///
////    //    /// \brief close
////    //    ///
////    //    void close();

////    ///
////    /// \brief getInput
////    /// \return
////    ///
////    const Input& getInput() const { return m_input; }

////  private:
////    //    std::unique_ptr<Input> m_input;
////    Input& m_input;
////    //    bool m_moved = false;
////};

} // namespace sensor
} // namespace hub

////    template <class Input = input::InputStream,
////    template <class Input,
////              class... Args,
////              typename = typename std::enable_if<std::is_base_of<Input, Input>::value
//////              typename = typename std::enable_if<std::is_same<input::InputStream,
/// Input>::value / #if ( __cplusplus < 201703L ) / && ( sizeof...( Args ) != 1 || /
///!is_one_of<Input, Args...>::value ) / #else /                                                  &&
///( sizeof...( Args ) != 1 || /                                                       !(
/// std::is_same<Input, Args> {} || ... ) ) / #endif / >::type> /     InputSensorT( Args&&... args )
///: /         Sensor( sensor::SensorSpec {}  ), /         m_input( new Input( std::forward<Args>(
/// args )... ) ) { /         static_assert( std::is_base_of<Input, Input>::value, "not a base
/// class"
///);
//////        static_assert( !std::is_same<net::ClientSocket, Input>::value, "not clientSocket
/// class"
/////);

////        m_input->read( m_spec );
////    }

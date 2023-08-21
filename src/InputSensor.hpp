#pragma once

#include "Acquisition.hpp"
#include "Sensor.hpp"
#include "io/input/Input.hpp"
#include "Traits.hpp"

// user friendly useless includes
//#include "io/File.hpp"
#include "io/input/InputFile.hpp"
#include "io/input/InputStream.hpp"
#include "io/input/InputSyncStream.hpp"
#include "io/input/InputMemory.hpp"

#include "io/Memory.hpp"

namespace hub {

///
/// \brief The InputSensor class
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
class SRC_API InputSensor : public Sensor
{
  public:
    ///
    /// \brief InputSensor
    /// is called when you want to get data sensor
    /// \param input
    /// is the communication bus you want to use
    ///
    template <class Input,
              typename = typename std::enable_if<std::is_base_of<io::Input, Input>::value
                                                 && !std::is_same<Input, net::ClientSocket>::value
                                                 >::type>
    explicit InputSensor( Input&& input ) :

        Sensor( hub::SensorSpec {} ), m_input( new Input( std::move( input ) ) ) {
        static_assert( std::is_base_of<io::Input, Input>::value, "not a base class" );
        static_assert( !std::is_same<net::ClientSocket, Input>::value, "not clientSocket class" );

        m_input->read( m_spec );
    }

//    template <class Input = input::InputStream,
//              class... Args,
//              typename = typename std::enable_if<std::is_same<input::InputStream, Input>::value
//#if ( __cplusplus < 201703L )
//                                                 && ( sizeof...( Args ) != 1 ||
//                                                      !is_one_of<Input, Args...>::value )
//#else
//                                                 && ( sizeof...( Args ) != 1 ||
//                                                      !( std::is_same<Input, Args> {} || ... ) )
//#endif
//                                                 >::type>
//    InputSensor( Args&&... args ) :
//        Sensor( hub::SensorSpec {}  ),
//        m_input( new Input( std::forward<Args>( args )... ) ) {
//        static_assert( std::is_base_of<io::Input, Input>::value, "not a base class" );
//        static_assert( !std::is_same<net::ClientSocket, Input>::value, "not clientSocket class" );

//        m_input->read( m_spec );
//    }

    template <class Input>
//              ,
//              typename = typename std::enable_if<std::is_base_of<io::Input, Input>::value>::type>
    explicit InputSensor( const Input& input ) = delete;

    InputSensor(const InputSensor & inputSensor) = delete;
    InputSensor operator=( const InputSensor& inputSensor ) = delete;

    ///
    /// \brief InputSensor
    /// \param inputSensor
    ///
    InputSensor( InputSensor&& inputSensor );

    ~InputSensor();


  public:

    ///
    /// \brief operator >>
    /// \param acquisition
    ///
    void operator>>( Acquisition& acquisition );

    ///
    /// \brief operator >>
    /// \param inputSensor
    /// \return
    ///
    Acquisition operator>>( InputSensor& inputSensor );

    ///
    /// \brief getAllAcquisitions
    /// can be an asynchronous function depending of the used interface.
    /// \return
    /// all acquisitions coming from the interface.
    ///
    std::vector<Acquisition> getAllAcquisitions();

    ///
    /// \brief close
    ///
    void close();

    ///
    /// \brief getInput
    /// \return
    ///
    io::Input& getInput() const;

  private:
    std::unique_ptr<io::Input> m_input;
    bool m_moved = false;
};

} // namespace hub

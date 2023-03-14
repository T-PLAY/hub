#pragma once

// #include <any>
// #include <cassert>
// #include <iostream>
// #include <list>
// #include <map>
// #include <string>
// #include <typeinfo>
// #include <vector>

#include "Acquisition.hpp"
#include "Any.hpp"
#include "Macros.hpp"
#include "SensorSpec.hpp"

#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

namespace hub {
namespace io {

///
/// \brief The Input class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \note This class is an interface.
/// \warning Unable to perform write/read operations if communication is ended or closed.
/// TODO: split Input -> Input and Output classes
///
class SRC_API Input
{
  public:
    Input() = default;
    //    Input() = delete;

    ///
    /// \param input
    ///
    Input( Input&& input )                 = default;
    Input( const Input& input )            = delete;
    Input& operator=( const Input& input ) = delete;
    Input&& operator=( Input&& input )     = delete;

    virtual ~Input() = default;

  protected:
    ///
    /// \brief read
    /// function describes how to read data through from the communication bus.
    /// \param data
    /// [out] array of bytes in memory
    /// \param len
    /// [in] size of the data array to read
    ///
    virtual void read( unsigned char* data, size_t len ) = 0;

  public:
    ///
    /// \brief isEnd
    /// allows to know if the communication bus is no longer active,
    /// no memory bandwidth, no more acquisition coming.
    /// \return
    /// true if the bus is inactive.\n
    /// false otherwise.
    ///
    virtual bool isEnd() const = 0;

    ///
    /// \brief close
    /// is the capability to terminate communication at both ends if possible.
    ///
    virtual void close() = 0;

    ///
    /// \brief isOpen
    /// allows to verify the still alive communication of the bus.
    /// \return
    /// true if the bus is active.\n
    /// false otherwise.
    ///
    virtual bool isOpen() const = 0;

    ////////////////////////////////////////////////////////////////////////////

  public:
    ///
    /// \brief read
    /// \param any
    ///
    //    void read( std::any& any );
    void read( Any& any );

    ///
    /// \brief read
    /// \param str
    ///
    void read( char* str );

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t );

    ///
    /// \brief read
    /// \param list
    ///
    template <class T>
    void read( std::list<T>& list );

    ///
    /// \brief read
    /// \param vector
    ///
    template <class T>
    void read( std::vector<T>& vector );

    ///
    /// \brief read
    /// \param map
    ///
    template <class T, class U>
    void read( std::map<T, U>& map );

    //    template <class T, class U>
    //        typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
    //    template <class T, class U,
    //              typename = typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value
    //              )>::type> void
    ///
    /// \brief read
    /// \param pair
    ///
    template <class T, class U>
    void read( std::pair<T, U>& pair ) {

        //        typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ),
        //        void>::type read( std::pair<T, U>& pair ) const {

        assert( isOpen() );

#ifdef DEBUG_INPUT
        std::cout << "[Input] read(std::pair)" << std::endl;
#endif
        T first;
        read( first );
        //        hub::SensorSpec sensorSpec;
        //        read(sensorSpec);
        U second;
        read( second );
        //        auto second = get<U>();
        pair = std::make_pair( first, std::move( second ) );
    }

    //        template <class T, class U>
    //        typename std::enable_if<( std::is_same<U, hub::SensorSpec>::value ), void>::type
    //        read( std::pair<T, U>& pair )  {
    //            assert( isOpen() );

    //        #ifdef DEBUG_INPUT
    //            std::cout << "[Input] read(std::pair)" << std::endl;
    //        #endif
    //            T first;
    //            read( first );
    //            //    hub::SensorSpec sensorSpec;
    //            U second;
    //            second = getSensorSpec();
    //            //    read( second );
    //            pair = std::make_pair( first, second );
    //        }

  public:
    ///
    /// \brief read
    /// \param str
    ///
    void read( std::string& str );

    ///
    /// \brief read
    /// \param sensorSpec
    ///
    //    void read( SensorSpec& sensorSpec )  = delete;
    void read( SensorSpec& sensorSpec );

    //    ///
    //    /// \brief read
    //    /// \return
    //    ///
    //    SensorSpec read() const { return getSensorSpec(); }

    ///
    /// \brief read
    /// \param acquisition
    ///
    void read( Acquisition& acquisition ) = delete;

    ///
    /// \brief read
    /// \param measure
    ///
    void read( data::Measure& measure ) = delete;

    //    void read( data::UserData& userData );

    //    ///
    //    /// \brief getSensorSpec
    //    /// \return
    //    ///
    //    SensorSpec getSensorSpec() ;

    //    UserData getUserData() const;

    ///
    /// \brief getAcquisition
    /// \param sensorSpec
    /// \return
    ///
    virtual Acquisition getAcquisition( const SensorSpec& sensorSpec );

    //    friend class SensorSpec;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
void Input::read( T& t ) {
    assert( isOpen() );

    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_INPUT
#    ifdef USE_BOOST
    std::cout << "[Input] read(T) : " << typeid( T ).name() << " ("
              << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
#    else
    //    std::cout << "[Input] read(T) : " << typeid( T ).name() << " '" << t << "'"
    //              << std::endl;
    //    std::cout << "[Input] read(T) : " << typeid( T ).name() << " '" << t << "' : end"
    //              << std::endl;
    std::cout << "[Input] read(T) : '" << t << "' : end" << std::endl;
#    endif
#endif
}

template <class T>
void Input::read( std::list<T>& list ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[InpuInput] read(std::list)" << std::endl;
#endif

    int nbEl;
    read( nbEl );

    for ( int i = 0; i < nbEl; ++i ) {
        T el;
        read( el );
        list.push_back( std::move( el ) );
    }
}

template <class T>
void Input::read( std::vector<T>& vector ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] read(std::vector)" << std::endl;
#endif

    int nbEl;
    read( nbEl );

    vector.clear();
    vector.reserve( nbEl );

    for ( int i = 0; i < nbEl; ++i ) {
        T el;
        read( el );
        vector.push_back( std::move( el ) );
    }
}

template <class T, class U>
void Input::read( std::map<T, U>& map ) {
    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] read(std::map)" << std::endl;
#endif

    int nbEl;
    read( nbEl );
#ifdef DEBUG_INPUT
    std::cout << "[Input] map : nbEl = " << nbEl << std::endl;
#endif
    map.clear();

    for ( int i = 0; i < nbEl; ++i ) {
        std::pair<T, U> pair;
        read( pair );
        assert( map.find( pair.first ) == map.end() );
        map.emplace( std::move( pair ) );
    }
}

} // namespace io
} // namespace hub

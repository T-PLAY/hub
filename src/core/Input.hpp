#pragma once

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>

#include "core/Macros.hpp"
// #include "sensor/Acquisition.hpp"
#include "core/Any.hpp"
// #include "sensor/SensorSpec.hpp"
// #include "data/Measure.hpp"

#define DEBUG_INPUT
#define USE_BOOST

#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

#ifdef DEBUG_INPUT
#    define HEADER_INPUT_MSG \
        "\t\033[" << std::to_string( 31 + (long)this % 7 ) << "m[Input:" << this << "]\033[0m "
#endif


namespace hub {
// namespace io {

// class Input;

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
    template <typename T>
    //     using readable_t = decltype( std::declval<T>().isReadable() );
    using readable_t = decltype( std::declval<T>().read( std::declval<Input&>() ) );

    template <typename T, typename = std::void_t<>>
    struct readable : std::false_type {};

    template <typename T>
    struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};

    template <typename T>
    static constexpr bool readable_v = readable<T>::value;

    Input() = default;


    ///
    /// \param input
    ///
    Input( const Input& input )            = delete;
    Input( Input&& input )                 = delete;
    Input& operator=( const Input& input ) = delete;
    Input&& operator=( Input&& input )     = delete;

    virtual ~Input() = default;

    //  protected:
    ///
    /// \brief read
    /// function describes how to read data through from the communication bus.
    /// \param data
    /// [out] array of bytes in memory
    /// \param len
    /// [in] size of the data array to read
    ///
    virtual void read( Data_t* data, Size_t len ) = 0;

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
    //    ///
    //    /// \brief operator >>
    //    /// \param t
    //    ///
    //    template <class T>
    //    void operator>>( T& t );

    //    ///
    //    /// \brief get
    //    /// \return
    //    ///
    //    template <class T>
    //    T get();

    //    ///
    //    /// \brief readAll
    //    /// \param ts
    //    ///
    //    template <typename Container,
    //              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
    //    void readAll( Container& ts );

    //    ///
    //    /// \brief getAll
    //    /// \return
    //    ///
    //    template <typename Container>
    //    Container getAll();

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    // typename = typename std::enable_if<readable_v<T> >::type>
    //     void read( T& t );
//    std::enable_if<readable<T>::value>::type read( T& t ) {
    std::enable_if<readable_v<T>>::type read( T& t ) {
//        std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name()
//                  << std::endl;
        t.read(*this);
//	    debugInput(t);
//        DEBUG_INPUT(t);
//        hub::debugPrintTypeAndValue(t);
#ifdef DEBUG_INPUT
        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME(t) << ") = " << t << std::endl;
#endif

//#ifdef DEBUG_INPUT
//#    ifdef USE_BOOST
//        std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
//#    else
//        std::cout << HEADER_INPUT_MSG "read(T) : <" << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif
//#endif
    }

    template <class T>
    //    void read( T& t );
    std::enable_if<!readable_v<T>>::type read( T& t ) {
//        std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name()
//                  << std::endl;

        assert( isOpen() );
        assert( !isEnd() );

        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

#ifdef DEBUG_INPUT
        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME(t) << ") = " << t << std::endl;
#endif
//        debugPrintTypeAndValue(t);

//#ifdef DEBUG_INPUT
//#    ifdef USE_BOOST
//        std::cout << HEADER_INPUT_MSG "read(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << "> " << t  << std::endl;
//#    else
//        std::cout << HEADER_INPUT_MSG "read(T) : <" << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif
//#endif
    }

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

    ///
    /// \brief read
    /// \param pair
    ///
    template <class T, class U>
    void read( std::pair<T, U>& pair );

  public:
    ///
    /// \brief read
    /// \param any
    ///
    void read( Any& any );

    ///
    /// \brief read
    /// \param str
    ///
    void read( char* str );

    ///
    /// \brief read
    /// \param str
    ///
    void read( std::string& str );

    //    ///
    //    /// \brief read
    //    /// \param sensorSpec
    //    ///
    //    virtual void read( sensor::SensorSpec& sensorSpec );

    // todo acq
    //    ///
    //    /// \brief read
    //    /// \param acq
    //    ///
    //    virtual void read( sensor::Acquisition& acq );

    //    ///
    //    /// \brief read
    //    /// \param measure
    //    ///
    //    void read( Measure& measure );

#ifdef ARCH_X86
    void read( size_t size ) = delete; // non compatible format 32/64 bit
#endif

  public:
    // todo acq
    //    ///
    //    /// \brief operator >>
    //    /// \param input
    //    /// \return
    //    ///
    //    sensor::Acquisition operator>>( Input& input );

  private:
    //    std::list<sensor::Acquisition> m_lastAcqs;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// template<class T,
// typename = typename std::enable_if<readable_v<T> >::type>
// void Input::read(T &t) {
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " (readable)" <<
//     std::endl;

//}

// template<class T>
// std::enable_if<! readable_v<T>>::type Input::read(T &t) {
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " (not readable)" <<
//     std::endl;

//    assert( isOpen() );
//    assert( !isEnd() );

//    read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

// #ifdef DEBUG_INPUT
// #    ifdef USE_BOOST
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " ("
//               << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
// #    else
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " '" << t << "'"
//               << std::endl;
// #    endif
// #endif
// }

// template <class T>
// inline void Input::operator>>( T& t ) {
//     assert( isOpen() );
//     assert( !isEnd() );

//    read( t );
//}

// template <class T>
// inline T Input::get() {
//     assert( isOpen() );
//     assert( !isEnd() );

//    T t;
//    read( t );
//    return t;
//}

// template <typename Container>
// Container Input::getAll() {
//     assert( isOpen() );
//     assert( !isEnd() );

//    Container ts;
//    readAll( ts );
//    return ts;
//}

// template <typename Container, typename T>
// void Input::readAll( Container& ts ) {
//     assert( isOpen() );
//     assert( !isEnd() );

//    try {
//        while ( !isEnd() ) {
//            ts.emplace_back( get<T>() );
//        }
//    }
//    catch ( std::exception& e ) {
//        std::cout <<  "[Input] catch exception : " << e.what() << std::endl;
//        throw;
//    }
//}

// template <class T>
// inline void Input::read( T& t ) {
//     assert( isOpen() );
//     assert( !isEnd() );

//    read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );

// #ifdef DEBUG_INPUT
// #    ifdef USE_BOOST
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " ("
//               << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
// #    else
//     std::cout << HEADER_INPUT_MSG "read(T) : " << typeid( T ).name() << " '" << t << "'" <<
//     std::endl;
// #    endif
// #endif
// }

template <class T>
inline void Input::read( std::list<T>& list ) {
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
    std::cout << "[InpuInput] read(" << TYPE_NAME(list) << ")" << std::endl;
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
inline void Input::read( std::vector<T>& vector ) {
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME(vector) << ")" << std::endl;
#endif

    uint64_t nbEl;
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
inline void Input::read( std::map<T, U>& map ) {
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
//    std::cout << HEADER_INPUT_MSG "read(std::map)" << std::endl;
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME(map)  << ")" << std::endl;
#endif

    int nbEl;
    read( nbEl );
//#ifdef DEBUG_INPUT
//    std::cout << HEADER_INPUT_MSG "map : nbEl = " << nbEl << std::endl;
//#endif
    map.clear();

    for ( int i = 0; i < nbEl; ++i ) {
        std::pair<T, U> pair;
        read( pair );
        assert( map.find( pair.first ) == map.end() );
        map.emplace( std::move( pair ) );
    }
}

template <class T, class U>
inline void Input::read( std::pair<T, U>& pair ) {
    assert( isOpen() );
    assert( !isEnd() );

#ifdef DEBUG_INPUT
//    std::cout << HEADER_INPUT_MSG "read(std::pair)" << std::endl;
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME(pair) << ")" << std::endl;
#endif
    T first;
    read( first );
    U second;
    read( second );
    pair = std::make_pair( first, std::move( second ) );
}

//} // namespace io
} // namespace hub

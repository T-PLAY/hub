#pragma once

#include <exception>
#include <list>
#include <map>

// #include "sensor/Acquisition.hpp"
#include "core/Any.hpp"
#include "core/Macros.hpp"
// #include "sensor/SensorSpec.hpp"
// #include "data/Measure.hpp"

#define DEBUG_OUTPUT
#define USE_BOOST

#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

#ifdef DEBUG_OUTPUT
#    define HEADER_OUTPUT_MSG \
        "\033[" << std::to_string( 31 + (long)this % 7 ) << "m[Output:" << this << "]\033[0m "
#endif

namespace hub {
// namespace io {

///
/// \brief The Output class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \note This class is an interface.
/// \warning Unable to perform write/read operations if communication is ended or closed.
/// TODO: split Output -> Input and Output classes
///
class SRC_API Output
{
  public:
    template <typename T>
    using writable_t = decltype( std::declval<T>().write( std::declval<Output&>() ) );

    template <typename T, typename = std::void_t<>>
    struct writable : std::false_type {};

    template <typename T>
    struct writable<T, std::void_t<writable_t<T>>> : std::true_type {};

    template <typename T>
    static constexpr bool writable_v = writable<T>::value;

    Output() = default;

    ///
    /// \param output
    ///
    Output( const Output& output )            = delete;
    Output( Output&& output )                 = delete;
    Output& operator=( const Output& output ) = delete;
    Output&& operator=( Output&& output )     = delete;

//    virtual ~Output();

  public:
    ///
    /// \brief write
    /// function describes how to write data through the communication bus.
    /// \param data
    /// [in] array of bytes in memory
    /// \param len
    /// [in] size of the data array to write
    ///
    virtual void write( const Data_t* data, Size_t len ) = 0;

  public:
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

  public:
    //    ///
    //    /// \brief operator <<
    //    /// \param t
    //    ///
    //    template <class T>
    //    void operator<<( T& t );

    //    ///
    //    /// \brief put
    //    /// \param t
    //    ///
    //    template <class T>
    //    void put( const T& t );

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    //    void write( const T& t );
//    std::enable_if<writable<T>::value>::type write( const T& t ) {
    std::enable_if<writable_v<T>>::type write( const T& t ) {
//#ifdef DEBUG_OUTPUT
//#    ifdef USE_BOOST
//        std::cout << HEADER_OUTPUT_MSG "write(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << " = " << t << std::endl;
//#    else
//        std::cout << HEADER_OUTPUT_MSG "write(T) : <" << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif
//#endif
#ifdef DEBUG_OUTPUT
        std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(t) << ") = " << t << std::endl;
#endif
//        std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name()
//                  << std::endl;
        t.write(*this);
    }
    //    write( const T& t );

    template <class T>
    //    void write( const T& t );
//    std::enable_if<!writable<T>::value>::type write( const T& t ) {
    std::enable_if<!writable_v<T>>::type write( const T& t ) {

//        std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name()
//                  << std::endl;

        assert( isOpen() );

#ifdef DEBUG_OUTPUT
        std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(t) << ") = " << t << std::endl;
#endif

//#ifdef DEBUG_OUTPUT
//#    ifdef USE_BOOST
//        std::cout << HEADER_OUTPUT_MSG "write(T) : <"
////                  << typeid( T ).name() << " ("
//                  << boost::typeindex::type_id<T>().pretty_name() << " = " << t << std::endl;
//#    else
//        std::cout << HEADER_OUTPUT_MSG "write(T) : <" << typeid( T ).name() << "> = " << t
//                  << std::endl;
//#    endif
//#endif
        //    std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.ioTypeName() << "' = " << t <<
        //    std::endl;

        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
        //    write( t.ioGetData(), t.ioGetSize() );
    }

    ///
    /// \brief write
    /// \param list
    ///
    template <class T>
    void write( const std::list<T>& list );

    ///
    /// \brief write
    /// \param vector
    ///
    template <class T>
    void write( const std::vector<T>& vector );

    ///
    /// \brief write
    /// \param map
    ///
    template <class T, class U>
    void write( const std::map<T, U>& map );

    ///
    /// \brief write
    /// \param pair
    ///
    template <class T, class U>
    void write( const std::pair<T, U>& pair );

  public:
    ///
    /// \brief write
    /// \param any
    ///
    void write( const Any& any );

    ///
    /// \brief write
    /// \param str
    ///
    void write( const char* str );

    void write( char* str ) = delete; // non compatible format 32/64 bit

    ///
    /// \brief write
    /// \param str
    ///
    void write( const std::string& str );

    // todo acq
    //    ///
    //    /// \brief write
    //    /// \param sensorSpec
    //    ///
    //    virtual void write( const sensor::SensorSpec& sensorSpec );

    //    ///
    //    /// \brief write
    //    /// \param measure
    //    ///
    //    void write( const Measure& measure );

    // todo acq
    //    ///
    //    /// \brief write
    //    /// \param acq
    //    ///
    //    virtual void write( const sensor::Acquisition& acq );

    //    virtual void write(uint64_t size);

#ifdef ARCH_X86
    void write( size_t size ) = delete; // non compatible format 32/64 bit
#endif

    ////////////////////////////////////////////////////////////////////////////
};

// template <class T>
// inline void Output::put( const T& t ) {
//     assert( isOpen() );
//     write( t );
// }

// template <class T>
// inline void Output::operator<<( T& t ) {
//     assert( isOpen() );
//     write( t );
// }

// template <class T
//           >
// inline void Output::write( const T& t ) {
//     assert( isOpen() );

// #ifdef DEBUG_OUTPUT
// #    ifdef USE_BOOST
//     std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " ("
//               << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
// #    else
//     std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " '" << t << "'"
//               << std::endl;
// #    endif
// #endif
////    std::cout << HEADER_OUTPUT_MSG "write(T) : '" << t.ioTypeName() << "' = " << t << std::endl;

//    write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
////    write( t.ioGetData(), t.ioGetSize() );
//}

template <class T>
inline void Output::write( const std::list<T>& list ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(list) << ")" << std::endl;
#endif

    int nbEl = list.size(); // todo
    write( nbEl );

    for ( const T& el : list ) {
        write( el );
    }
}

template <class T>
inline void Output::write( const std::vector<T>& vector ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(vector) <<  ")" << std::endl;
#endif

    uint64_t nbEl = static_cast<uint64_t>( vector.size() );
    write( nbEl );

    for ( const T& el : vector ) {
        write( el );
    }
}

template <class T, class U>
inline void Output::write( const std::map<T, U>& map ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(map) <<  ")" << std::endl;
#endif

    int nbKey = static_cast<int>( map.size() );
    write( nbKey );
//#ifdef DEBUG_OUTPUT
//    std::cout << HEADER_OUTPUT_MSG "map : nbEl = " << nbKey << std::endl;
//#endif

    for ( const std::pair<T, U>& pair : map ) {
        write( pair );
    }
}

template <class T, class U>
inline void Output::write( const std::pair<T, U>& pair ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME(pair)  << ")" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}

//} // namespace io
} // namespace hub

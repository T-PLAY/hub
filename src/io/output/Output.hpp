#pragma once

#include <exception>

#include "Acquisition.hpp"
#include "Any.hpp"
#include "Macros.hpp"
#include "SensorSpec.hpp"

#ifdef USE_BOOST
#    include <boost/type_index.hpp>
#endif

#ifdef DEBUG_OUTPUT
#define HEADER_OUTPUT_MSG "\033[" << std::to_string(31 + (long)this % 7) << "m[Output:" << this << "]\033[0m "
#endif

namespace hub {
namespace io {

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
    Output() = default;

    ///
    /// \param output
    ///
    Output( Output&& output )      = default;
    Output( const Output& output ) = delete;
    Output& operator=( const Output& output ) = delete;
    Output&& operator=( Output&& output ) = delete;

    virtual ~Output();

  public:
    ///
    /// \brief write
    /// function describes how to write data through the communication bus.
    /// \param data
    /// [in] array of bytes in memory
    /// \param len
    /// [in] size of the data array to write
    ///
    virtual void write( const unsigned char* data, size_t len ) = 0;

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
    ///
    /// \brief operator <<
    /// \param t
    ///
    template <class T>
    void operator<<( T& t );

    ///
    /// \brief put
    /// \param t
    ///
    template <class T>
    void put( const T& t );

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t );

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

    ///
    /// \brief write
    /// \param sensorSpec
    ///
    virtual void write( const SensorSpec& sensorSpec );

    ///
    /// \brief write
    /// \param measure
    ///
    void write( const data::Measure& measure );

    ///
    /// \brief write
    /// \param acq
    ///
    virtual void write( const Acquisition& acq );

//    virtual void write(uint64_t size);

#ifdef ARCH_X86
    void write( size_t size ) = delete; // non compatible format 32/64 bit
#endif

    ////////////////////////////////////////////////////////////////////////////
};

template <class T>
inline void Output::put( const T& t ) {
    assert( isOpen() );
    write( t );
}

template <class T>
inline void Output::operator<<( T& t ) {
    assert( isOpen() );
    write( t );
}

template <class T>
inline void Output::write( const T& t ) {
    assert( isOpen() );

    write( reinterpret_cast<const unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_OUTPUT
#    ifdef USE_BOOST
    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " ("
              << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
#    else
    std::cout << HEADER_OUTPUT_MSG "write(T) : " << typeid( T ).name() << " '" << t << "'" << std::endl;
#    endif
#endif
}

template <class T>
inline void Output::write( const std::list<T>& list ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(std::list)" << std::endl;
#endif

    int nbEl = list.size();
    write( nbEl );

    for ( const T& el : list ) {
        write( el );
    }
}

template <class T>
inline void Output::write( const std::vector<T>& vector ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(std::vector)" << std::endl;
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
    std::cout << HEADER_OUTPUT_MSG "write(std::map)" << std::endl;
#endif

    int nbKey = static_cast<int>( map.size() );
    write( nbKey );
#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "map : nbEl = " << nbKey << std::endl;
#endif

    for ( const std::pair<T, U>& pair : map ) {
        write( pair );
    }
}

template <class T, class U>
inline void Output::write( const std::pair<T, U>& pair ) {
    assert( isOpen() );

#ifdef DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(std::pair)" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}

} // namespace io
} // namespace hub

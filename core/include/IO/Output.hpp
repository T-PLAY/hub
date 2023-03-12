#pragma once

//#include <any>
//#include <cassert>
//#include <iostream>
//#include <list>
//#include <map>
//#include <string>
//#include <typeinfo>
//#include <vector>

//#include "Macros.hpp"
//#include "Acquisition.hpp"
//#include "SensorSpec.hpp"

#include "IO.hpp"


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
//    Output() = delete;

    ///
    /// \param ioStream
    ///
    Output( Output&& ioStream )                 = default;
    Output( const Output& ioStream )            = delete;
    Output& operator=( const Output& ioStream ) = delete;
    Output&& operator=( Output&& ioStream )     = delete;

    virtual ~Output() = default;

  protected:
    ///
    /// \brief write
    /// function describes how to write data through the communication bus.
    /// \param data
    /// [in] array of bytes in memory
    /// \param len
    /// [in] size of the data array to write
    ///
    virtual void write( const unsigned char* data, size_t len )  = 0;


  public:
//    ///
//    /// \brief isEnd
//    /// allows to know if the communication bus is no longer active,
//    /// no memory bandwidth, no more acquisition coming.
//    /// \return
//    /// true if the bus is inactive.\n
//    /// false otherwise.
//    ///
//    virtual bool isEnd() const = 0;

    ///
    /// \brief close
    /// is the capability to terminate communication at both ends if possible.
    ///
    virtual void close()  = 0;

    ///
    /// \brief isOpen
    /// allows to verify the still alive communication of the bus.
    /// \return
    /// true if the bus is active.\n
    /// false otherwise.
    ///
    virtual bool isOpen() const  = 0;

    ///
    /// \brief write
    /// \param any
    ///
    void write( const std::any& any ) ;

    ///
    /// \brief write
    /// \param str
    ///
    void write( const char* str ) ;

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t ) ;

    ///
    /// \brief write
    /// \param list
    ///
    template <class T>
    void write( const std::list<T>& list ) ;

    ///
    /// \brief write
    /// \param vector
    ///
    template <class T>
    void write( const std::vector<T>& vector ) ;

    ///
    /// \brief write
    /// \param map
    ///
    template <class T, class U>
    void write( const std::map<T, U>& map ) ;

    ///
    /// \brief write
    /// \param pair
    ///
    template <class T, class U>
    void write( const std::pair<T, U>& pair ) ;

  public:
    ///
    /// \brief write
    /// \param str
    ///
    void write( const std::string& str ) ;

    ///
    /// \brief write
    /// \param sensorSpec
    ///
    void write( const SensorSpec& sensorSpec ) ;


    ///
    /// \brief write
    /// \param measure
    ///
    void write( const Measure& measure ) ;

    void write( const UserData& userData) ;

    ///
    /// \brief write
    /// \param acq
    ///
    virtual void write( const Acquisition& acq ) ;

    ////////////////////////////////////////////////////////////////////////////


    template <class T>
    void put( const T& t )  {
        write(t);
    }

};


template <class T>
void Output::write( const T& t )  {
    assert( isOpen() );

    write( reinterpret_cast<const unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_IOSTREAM
#ifdef USE_BOOST
    std::cout << "[Output] write(T) : " << typeid( T ).name() << " (" << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'"
              << std::endl;
#else
    std::cout << "[Output] write(T) : " << typeid( T ).name() << " '" << t << "'"
              << std::endl;
#endif
#endif
}

template <class T>
void Output::write( const std::list<T>& list )  {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Output] write(std::list)" << std::endl;
#endif

    int nbEl = list.size();
    write( nbEl );

    for ( const T& el : list ) {
        write( el );
    }
}

template <class T>
void Output::write( const std::vector<T>& vector )  {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Output] write(std::vector)" << std::endl;
#endif

    int nbEl = static_cast<int>( vector.size() );
    write( nbEl );

    for ( const T& el : vector ) {
        write( el );
    }
}

template <class T, class U>
void Output::write( const std::map<T, U>& map )  {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Output] write(std::map)" << std::endl;
#endif

    int nbKey = static_cast<int>( map.size() );
    write( nbKey );
#ifdef DEBUG_IOSTREAM
    std::cout << "[Output] map : nbEl = " << nbKey << std::endl;
#endif

    for ( const std::pair<T, U>& pair : map ) {
        write( pair );
    }
}

template <class T, class U>
void Output::write( const std::pair<T, U>& pair )  {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Output] write(std::pair)" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}


} // namespace io
} // namespace hub

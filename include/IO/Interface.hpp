#pragma once

#include <any>
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "Acquisition.hpp"
#include "Macros.hpp"
#include "SensorSpec.hpp"

// #define DEBUG_IOSTREAM

namespace hub {
namespace io {

///
/// \brief The Interface class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \attention This class is an interface.
///
class SRC_API Interface
{
  public:
    Interface()                            = default;

    ///
    /// \param ioStream
    ///
    Interface( Interface&& ioStream )      = default;
    Interface( const Interface& ioStream ) = delete;
    Interface& operator=( const Interface& ioStream ) = delete;
    Interface&& operator=( Interface&& ioStream ) = delete;


    virtual ~Interface() = default;
    //    virtual ~Interface();

protected:


    ///
    /// \brief write
    /// function describes how to write data through the communication bus.
    /// \param data
    /// [in] array of bytes in memory
    /// \param len
    /// [in] size of the data array to write
    ///
    virtual void write( const unsigned char* data, size_t len ) const = 0;

    ///
    /// \brief read
    /// function describes how to read data through from the communication bus.
    /// \param data
    /// [out] array of bytes in memory
    /// \param len
    /// [in] size of the data array to read
    ///
    virtual void read( unsigned char* data, size_t len ) const = 0;

public:

    ///
    /// \brief isEnd
    /// allows to know if the communication bus is no longer active,
    /// no memory bandwidth, no more acquisition coming.
    /// \return
    /// true if the bus is inactive
    /// false otherwise
    ///
    virtual bool isEnd() const  = 0;

    ///
    /// \brief close
    /// Capability to terminate communication at both ends if possible.
    ///
    virtual void close() const  = 0;

    ///
    /// \brief isOpen
    /// allows to verify the good communication of the bus.
    /// \return
    /// true if the bus is active
    /// false otherwise
    ///
    virtual bool isOpen() const = 0;

    ///
    /// \brief write
    /// \param any
    ///
    void write( const std::any& any ) const;


    ///
    /// \brief write
    /// \param str
    ///
    void write( const char* str ) const;

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t ) const;

    ///
    /// \brief write
    /// \param list
    ///
    template <class T>
    void write( const std::list<T>& list ) const;

    ///
    /// \brief write
    /// \param vector
    ///
    template <class T>
    void write( const std::vector<T>& vector ) const;

    ///
    /// \brief write
    /// \param map
    ///
    template <class T, class U>
    void write( const std::map<T, U>& map ) const;

    ///
    /// \brief write
    /// \param pair
    ///
    template <class T, class U>
    void write( const std::pair<T, U>& pair ) const;

    ///
    /// \brief write
    /// \param str
    ///
    void write( const std::string& str ) const;

    ///
    /// \brief write
    /// \param sensorSpec
    ///
    void write( const SensorSpec& sensorSpec ) const;

    ///
    /// \brief write
    /// \param measure
    ///
    void write( const Measure& measure ) const;

    ///
    /// \brief write
    /// \param acq
    ///
    virtual void write( const Acquisition& acq ) const;

    ////////////////////////////////////////////////////////////////////////////


    ///
    /// \brief read
    /// \param any
    ///
    void read( std::any& any ) const;


    ///
    /// \brief read
    /// \param str
    ///
    void read( char* str ) const;

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t ) const;


    ///
    /// \brief read
    /// \param list
    ///
    template <class T>
    void read( std::list<T>& list ) const;

    ///
    /// \brief read
    /// \param vector
    ///
    template <class T>
    void read( std::vector<T>& vector ) const;

    ///
    /// \brief read
    /// \param map
    ///
    template <class T, class U>
    void read( std::map<T, U>& map ) const;

    ///
    /// \brief read
    /// \param pair
    ///
    template <class T, class U>
    void read( std::pair<T, U>& pair ) const;

    ///
    /// \brief read
    /// \param str
    ///
    void read( std::string& str ) const;

    ///
    /// \brief read
    /// \param sensorSpec
    ///
    void read( SensorSpec& sensorSpec ) const   = delete;

    ///
    /// \brief read
    /// \param acquisition
    ///
    void read( Acquisition& acquisition ) const = delete;

    ///
    /// \brief read
    /// \param measure
    ///
    void read( Measure& measure ) const         = delete;

    //    Measure getMeasure() const;
    ///
    /// \brief getSensorSpec
    /// \return
    ///
    SensorSpec getSensorSpec() const;
    //    virtual Acquisition getAcquisition( int acquisitionSize ) const;
    ///
    /// \brief getAcquisition
    /// \param sensorSpec
    /// \return
    ///
    virtual Acquisition getAcquisition( const SensorSpec& sensorSpec ) const;

    ///
    /// \brief anyValue2string
    /// \param any
    /// \return
    ///
    static std::string anyValue2string( const std::any& any );

    ///
    /// \brief anyType2string
    /// \param any
    /// \return
    ///
    static const std::string& anyType2string( const std::any& any );
};

///
/// \brief The InputInterface class
/// is used to instantiate an InputSensor.
///
class SRC_API InputInterface : public virtual Interface
{};

///
/// \brief The OutputInterface class
/// is used to instantiate an OutputSensor.
///
class SRC_API OutputInterface : public virtual Interface
{};

///
/// \brief The InputOutputInterface class
/// is used to instantiate both InputSensor and OutputSensor.
///
class SRC_API InputOutputInterface : public InputInterface, public OutputInterface
{};

template <class T>
void Interface::write( const T& t ) const {
    assert( isOpen() );

    write( reinterpret_cast<const unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(T) : " << typeid( T ).name() << " '" << t << "' : end"
              << std::endl;
#endif
}

template <class T>
void Interface::write( const std::list<T>& list ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::list) : start" << std::endl;
#endif

    int nbEl = list.size();
    write( nbEl );

    for ( const T& el : list ) {
        write( el );
    }
}

template <class T>
void Interface::write( const std::vector<T>& vector ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::vector) : start" << std::endl;
#endif

    int nbEl = static_cast<int>( vector.size() );
    write( nbEl );

    for ( const T& el : vector ) {
        write( el );
    }
}

template <class T, class U>
void Interface::write( const std::map<T, U>& map ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::map) : start" << std::endl;
#endif

    int nbKey = static_cast<int>( map.size() );
    write( nbKey );
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] map : nbEl = " << nbKey << std::endl;
#endif

    for ( const std::pair<T, U>& pair : map ) {
        write( pair );
    }
}

template <class T, class U>
void Interface::write( const std::pair<T, U>& pair ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(std::pair) : start" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}

///////////////////////////////////////////////////////////////////////////////

template <class T>
void Interface::read( T& t ) const {
    assert( isOpen() );

    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(T) : " << typeid( T ).name() << " '" << t << "' : end"
              << std::endl;
#endif
}

template <class T>
void Interface::read( std::list<T>& list ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::list) : start" << std::endl;
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
void Interface::read( std::vector<T>& vector ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::vector) : start" << std::endl;
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
void Interface::read( std::map<T, U>& map ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::map) : start" << std::endl;
#endif

    int nbEl;
    read( nbEl );
#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] map : nbEl = " << nbEl << std::endl;
#endif
    map.clear();

    for ( int i = 0; i < nbEl; ++i ) {
        std::pair<T, U> pair;
        read( pair );
        assert( map.find( pair.first ) == map.end() );
        map.emplace( std::move( pair ) );
    }
}

template <class T, class U>
void Interface::read( std::pair<T, U>& pair ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::pair) : start" << std::endl;
#endif
    T first;
    read( first );
    U second;
    read( second );
    pair = std::make_pair( first, second );
}

} // namespace io
} // namespace hub

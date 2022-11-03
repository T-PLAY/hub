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

//#define DEBUG_IOSTREAM

namespace hub {
namespace io {

///
/// \brief The Interface class
/// allows the inherited classes to serialize the data.
/// This allows to send an acquisition flow on different peripherals (file, socket, RAM, etc).
///
class SRC_API Interface
{
  public:
    Interface()                                       = default;
    Interface( Interface&& ioStream )                 = default;
    Interface( const Interface& ioStream )            = delete;
    Interface& operator=( const Interface& ioStream ) = delete;
    Interface&& operator=( Interface&& ioStream )     = delete;

    virtual ~Interface() = default;
//    virtual ~Interface();

    virtual void close() const = 0;
    virtual bool isOpen() const = 0;
    virtual bool isEnd() const = 0;

    void write( const std::any& any ) const;
    virtual void write( const unsigned char* data, size_t len ) const = 0;
    void write( const char* str ) const;

    template <class T>
    void write( const T& t ) const;
    template <class T>
    void write( const std::list<T>& list ) const;
    template <class T>
    void write( const std::vector<T>& vector ) const;
    template <class T, class U>
    void write( const std::map<T, U>& map ) const;
    template <class T, class U>
    void write( const std::pair<T, U>& pair ) const;

    void write( const std::string& str ) const;
    void write( const SensorSpec& sensorSpec ) const;
    void write( const Measure& measure ) const;

    virtual void write( const Acquisition& acq ) const;

    ////////////////////////////////////////////////////////////////////////////

    void read( std::any& any ) const;
    virtual void read( unsigned char* data, size_t len ) const = 0;
    void read( char* str ) const;

    template <class T>
    void read( T& t ) const;
    template <class T>
    void read( std::list<T>& list ) const;
    template <class T>
    void read( std::vector<T>& vector ) const;
    template <class T, class U>
    void read( std::map<T, U>& map ) const;
    template <class T, class U>
    void read( std::pair<T, U>& pair ) const;

    void read( std::string& str ) const;
    void read( SensorSpec& sensorSpec ) const;

    Measure getMeasure() const;
    SensorSpec getSensorSpec() const;
    virtual Acquisition getAcquisition( int acquisitionSize ) const;

    static std::string anyValue2string( const std::any& any );
    static const std::string& anyType2string( const std::any& any );
};

class SRC_API InputInterface : public virtual Interface
{};

class SRC_API OutputInterface : public virtual Interface
{};

class SRC_API InputOutputInterface : public InputInterface, public OutputInterface
{};

template <class T>
void Interface::write( const T& t ) const {
    assert(isOpen());

    write( reinterpret_cast<const unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] write(T) : " << typeid( T ).name() << " '" << t << "' : end"
              << std::endl;
#endif
}

template <class T>
void Interface::write( const std::list<T>& list ) const {
    assert(isOpen());

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
    assert(isOpen());

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
    assert(isOpen());

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
    assert(isOpen());

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
    assert(isOpen());

    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(T) : " << typeid( T ).name() << " '" << t << "' : end"
              << std::endl;
#endif
}

template <class T>
void Interface::read( std::list<T>& list ) const {
    assert(isOpen());

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
    assert(isOpen());

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
    assert(isOpen());

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
    assert(isOpen());

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

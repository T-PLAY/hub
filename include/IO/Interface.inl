
#pragma once
#include "Interface.hpp"

namespace hub {
namespace io {

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
typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
Interface::read( std::pair<T, U>& pair ) const {
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

template <class T, class U>
typename std::enable_if<( std::is_same<U, hub::SensorSpec>::value ), void>::type
Interface::read( std::pair<T, U>& pair ) const {
    assert( isOpen() );

#ifdef DEBUG_IOSTREAM
    std::cout << "[Interface] read(std::pair) : start" << std::endl;
#endif
    T first;
    read( first );
    //    hub::SensorSpec sensorSpec;
    U second;
    second = getSensorSpec();
    //    read( second );
    pair = std::make_pair( first, second );
}

} // namespace io
} // namespace hub

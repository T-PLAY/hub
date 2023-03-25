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

    ///
    /// \param input
    ///
    Input( Input&& input ) = default;
    //    Input( Input&& input );
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
    template <class T>
    void operator>>( T& t );

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t );

    template <class T>
    T get();

    template <template <typename, typename...> class Container, typename T>
    void readAll( Container<T>& ts );

    template <class T>
    T getAll();

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

    ///
    /// \brief read
    /// \param sensorSpec
    ///
    virtual void read( SensorSpec& sensorSpec );

    ///
    /// \brief read
    /// \param acquisition
    ///
    //    void read( Acquisition& acquisition ) = delete;
    virtual void read( Acquisition& acq );

    ///
    /// \brief read
    /// \param measure
    ///
    //    void read( data::Measure& measure ) = delete;
    void read( data::Measure& measure );

    ///
    //    virtual Acquisition getAcq( const SensorSpec& sensorSpec );
    ///
    /// \brief getAcq
    /// \return
    ///
    //    virtual Acquisition getAcq();

    ///
    /// \brief operator >>
    /// \param input
    /// \return
    ///
    template <class T = Acquisition>
    //    typename std::enable_if<std::is_same<T, void>::value, T&>::type
    T operator>>( Input& input );
    //        return ret;
    //        return *this;
    //    Acquisition operator>>( Input& input );

    //    Acquisition getSyncAcq(Input & input2, std::list<Acquisition> & lastAcqs2);

    //    hub::SensorSpec getSensorSpec() const;

  private:
    std::list<Acquisition> m_lastAcqs;
    //    hub::SensorSpec m_sensorSpec;
    //    friend class data::Measure;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
inline void Input::operator>>( T& t ) {
    read( t );
}

template <class T>
inline T Input::get() {
    T t;
    read( t );
    return t;
}

template <template <typename, typename...> class Container, typename T>
void Input::readAll( Container<T>& ts ) {

    try {
        while ( !isEnd() ) {
            ts.emplace_back( get<T>() );
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[Input] catch exception : " << e.what() << std::endl;
        throw;
    }
}

template <class T>
T Input::getAll() {
    T ts;
    readAll( ts );
    return ts;
}

template <class T>
inline void Input::read( T& t ) {
    //    if (! isOpen())
    //        throw std::runtime_error("[Input] closed, unable to read");
    assert( isOpen() );

    read( reinterpret_cast<unsigned char*>( &t ), sizeof( T ) );

#ifdef DEBUG_INPUT
#    ifdef USE_BOOST
    std::cout << "[Input] read(T) : " << typeid( T ).name() << " ("
              << boost::typeindex::type_id<T>().pretty_name() << ") '" << t << "'" << std::endl;
#    else
    std::cout << "[Input] read(T) : '" << t << "' : end" << std::endl;
#    endif
#endif
}

template <class T>
inline void Input::read( std::list<T>& list ) {
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
inline void Input::read( std::vector<T>& vector ) {
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
inline void Input::read( std::map<T, U>& map ) {
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

template <class T, class U>
inline void Input::read( std::pair<T, U>& pair ) {

    assert( isOpen() );

#ifdef DEBUG_INPUT
    std::cout << "[Input] read(std::pair)" << std::endl;
#endif
    T first;
    read( first );
    U second;
    read( second );
    pair = std::make_pair( first, std::move( second ) );
}

template <class T>
//    typename std::enable_if<std::is_same<T, void>::value, T&>::type
T Input::operator>>( Input& input ) {

#ifdef DEBUG
    assert( !isEnd() );
    assert( !input.isEnd() );
#endif

    T t;
    read( t );

    auto& lastAcqs = input.m_lastAcqs;

    assert( lastAcqs.size() < 20 );

    T t2;

    if ( lastAcqs.empty() ) {
        input.read( t2 );
        lastAcqs.push_back( std::move( t2 ) );
    }

    while ( t.getStart() < lastAcqs.front().getStart() ) {
        read( t );
        std::cout << "[InputSensor] operator>>(InputSensor&) shift t : " << t << std::endl;
    }

    while ( lastAcqs.back().getStart() < t.getStart() ) {
        input.read( t2 );
        lastAcqs.push_back( std::move( t2 ) );
    }

    if ( lastAcqs.back().getStart() == t.getStart() ) {
        t << lastAcqs.back().getMeasures();
        return t;
    }

    while ( lastAcqs.size() > 2 ) {
        lastAcqs.pop_front();
    }

    const auto& left  = lastAcqs.front();
    const auto& right = lastAcqs.back();

    assert( left.getStart() <= t.getStart() );
    assert( t.getStart() <= right.getStart() );

    const auto& closestAcq =
        ( std::abs( left.getStart() - t.getStart() ) > std::abs( right.getStart() - t.getStart() ) )
            ? ( right )
            : ( left );

    t << closestAcq.getMeasures();
    return t;

    //    const auto dist = std::abs( closestAcq.m_start - masterAcq.m_start );

    //    // assert(minDist < 20'000); // 20 ms
    //    // if too far then abort synchronize
    //    // consider constant period of acquistion rate
    //    auto maxDist = ( right.m_start - left.m_start ) / 2;

    //    // find acceptable corresponding acquisition if interpolation is not possible
    //    if ( !left.isInterpolable() ) {
    //        const auto& lastMasterAcq = getLastAcq( syncViewerName );
    //        if ( lastMasterAcq != nullptr ) {
    //            maxDist =
    //                std::min( dist, std::abs( lastMasterAcq->m_start() - masterAcq.m_start() ) );
    //        }
    //    }
}

} // namespace io
} // namespace hub

#pragma once

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
//#include <thread>
#include <vector>

//#define HUB_THREAD_SAFE
#ifdef HUB_THREAD_SAFE
#include <mutex>
#endif

//#include "core/Any.hpp"
#include "core/Macros.hpp"

#if defined(OS_MACOS) && CPLUSPLUS_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
#include "core/Traits.hpp"
#endif

#ifdef HUB_DEBUG_INPUT
#    define HEADER_INPUT_MSG                                                             \
        "\t\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
                  << "m[Input:" << this << "]\033[0m "
#endif

namespace hub {

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
    using readable_t = decltype( std::declval<T>().read( std::declval<Input&>() ) );


    template <typename T, typename = std::void_t<>>
    struct readable : std::false_type {};

    template <typename T>
    struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};

    template <typename T>
    static constexpr bool readable_v = readable<T>::value;

  public:
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
    /// \brief isEmpty
    /// allows to know if the communication bus is no longer active,
    /// no memory bandwidth, no more acquisition coming.
    /// \return
    /// true if the bus is inactive.\n
    /// false otherwise.
    ///
    virtual bool isEmpty() const = 0;

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

    virtual void clear() = 0;

    ////////////////////////////////////////////////////////////////////////////

  public:
    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    typename std::enable_if<readable_v<T>>::type read( T& t ) {
#ifdef HUB_THREAD_SAFE
        m_mtx.lock();
#endif
        assert( isOpen() );
//        while (isOpen() && isEmpty()) {
#ifdef HUB_THREAD_SAFE
        while (isEmpty()) {
            std::cout << "[Input:" << std::this_thread::get_id() << "]  waiting for data" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
#endif
        assert( !isEmpty() );

        t.read( *this );
#ifdef HUB_DEBUG_INPUT
        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
#ifdef HUB_THREAD_SAFE
        m_mtx.unlock();
#endif
    }

    template <class T>
    typename std::enable_if<!readable_v<T>>::type read( T& t ) {
#ifdef HUB_THREAD_SAFE
        m_mtx.lock();
#endif
        assert( isOpen() );
//        while (isOpen() && isEmpty()) {
#ifdef HUB_THREAD_SAFE
        while (isEmpty()) {
            std::cout << "[Input:" << std::this_thread::get_id() << "]  waiting for data" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
#endif
//        if ( !isOpen() ) {
//            std::cout << "[Input" << std::this_thread::get_id() << "]  is closed, unable to read data"
//                      << std::endl;
//            return;
//        }
        assert( !isEmpty() );

        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
#ifdef HUB_DEBUG_INPUT
        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
#ifdef HUB_THREAD_SAFE
        m_mtx.unlock();
#endif
    }

//  private:
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

#ifdef ARCH_X86
    void read( size_t size ) = delete; // non compatible format 32/64 bit
#endif

  public:

    ///
    /// \brief readAll
    /// \param ts
    ///
    template <typename Container,
              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
    void readAll( Container& ts );

    ///
    /// \brief get
    /// \return
    ///
    template <class T>
    T get();

    ///
    /// \brief getAll
    /// \return
    ///
    template <typename Container>
    Container getAll();

    ///
    /// \brief operator >>
    /// \param t
    ///
    template <class T>
    //    template <class T,
    //              typename = typename std::enable_if<! std::is_base_of_v<Input, T>>>
    void operator>>( T& t );

    //    void read(Input & input) {
    //        int a;
    //        input.read(a);
    //    }

    //    template <typename Acquisition>
    //    Acquisition operator>>( Input& input );
//    Input& operator>>( Input& input );

    // sensor::Acquisition Input::operator>>( Input& input ) {

  private:
#ifdef HUB_THREAD_SAFE
    std::mutex m_mtx;
#endif

    //    std::list<sensor::Acquisition> m_lastAcqs;
//    size_t m_id;
//    std::list<std::any> m_lastTs;
//    std::list<hub::Input*> m_leftInputs;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
inline void Input::read( std::list<T>& list ) {
    assert( isOpen() );
    assert( !isEmpty() );

#ifdef HUB_DEBUG_INPUT
    std::cout << "[InpuInput] read(" << TYPE_NAME( list ) << ")" << std::endl;
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
    assert( !isEmpty() );

#ifdef HUB_DEBUG_INPUT
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
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
    assert( !isEmpty() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(std::map)" << std::endl;
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( map ) << ")" << std::endl;
#endif

    int nbEl;
    read( nbEl );
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
    assert( !isEmpty() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(std::pair)" << std::endl;
    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( pair ) << ")" << std::endl;
#endif
    T first;
    read( first );
    U second;
    read( second );
    pair = std::make_pair( first, std::move( second ) );
}

// template <class T>
// template <class T,
//           typename>
// inline void Input::operator>>( T& t ) {
//     assert( isOpen() );
//     assert( !isEmpty() );

//    read( t );
//}

template <typename Container, typename T>
void Input::readAll( Container& ts ) {
    assert( isOpen() );
    assert( !isEmpty() );

           //    ts.reserve( ts.size() );
    try {
        while ( !isEmpty() ) {
            ts.emplace_back( get<T>() );
        }
    }
    catch ( std::exception& e ) {
        std::cout << "[Input] catch exception : " << e.what() << std::endl;
        throw;
    }
}

template <class T>
inline T Input::get() {
    assert( isOpen() );
    assert( !isEmpty() );

    T t;
    read( t );
    return t;
}

template <typename Container>
Container Input::getAll() {
    assert( isOpen() );
    assert( !isEmpty() );

    Container ts;
    readAll( ts );
    return ts;
}

template<class T>
void Input::operator>>(T &t) {
    assert( isOpen() );
//    assert( !isEmpty() );

    read(t);
}

    //        std::cout << "[Input] operator>> ";
    //        //        for (int i = 0; i < m_leftInputs.size(); ++i) {
    //        for ( auto* leftInput : m_leftInputs ) {
    //            std::cout << leftInput << " ";
    //        }
    //        std::cout << std::endl;

    ////        tOut = 1;

    //        auto& rightInput = *this;
    //        T rightT;
    //        rightInput.read( rightT );

    //        //        read( t );
    ////        T ret;

    //        for ( auto* leftInputPtr : m_leftInputs ) {
    //            auto& leftInput  = *leftInputPtr;
    //            auto& leftLastTs = leftInput.m_lastTs;
    //            assert( leftLastTs.size() < 20 );

    //            if ( leftLastTs.empty() ) {
    //                T leftT;
    //                leftInput.read( leftT );
    //                leftLastTs.push_back( std::move( leftT ) );
    //            }

    //            //                while ( rightT.getStart() < leftLastTs.front().getStart() ) {
    //            while ( rightT < std::any_cast<const T&>(leftLastTs.front()) ) {
    //                std::cout << "[Input] operator>>(Input&) shift rightT : " << rightT
    //                          << std::endl;
    //                assert( !rightInput.isEmpty() );
    //                rightInput.read( rightT );
    //            }

    //            //                while ( leftLastTs.back().getStart() < rightT.getStart() &&
    //            //                !leftInput.isEmpty()
    //            while ( std::any_cast<const T&>(leftLastTs.back()) < rightT && !leftInput.isEmpty() ) {
    //                T leftT;
    //                assert( !leftInput.isEmpty() );
    //                leftInput.read( leftT );
    //                leftLastTs.push_back( std::move( leftT ) );
    //            }

    //            while ( leftLastTs.size() > 2 ) {
    //                leftLastTs.pop_front();
    //            }

    //            const auto& leftTBeforeRightT = std::any_cast<const T&>(leftLastTs.front());
    //            const auto& leftTAfterRightT  = std::any_cast<const T&>(leftLastTs.back());

    //            //                assert( leftInput.isEmpty() || leftTBeforeRightT.getStart() <=
    //            //                rightT.getStart());
    //            assert( leftInput.isEmpty() || leftTBeforeRightT <= rightT );
    //            //                assert( leftInput.isEmpty() || rightT.getStart() <= leftTAfterRightT );
    //            assert( leftInput.isEmpty() || rightT <= leftTAfterRightT );

    //            const auto& closestLeftT =
    //                ( std::abs( leftTBeforeRightT - rightT ) > std::abs( leftTAfterRightT - rightT ) )
    //                    ? ( leftTAfterRightT )
    //                    : ( leftTBeforeRightT );

    //            //    const auto & rightMeasures = rightT.getMeasures();
    //            //    const auto & closestMeasures = closestLeftT.getMeasures();
    //            ////    rightMeasures.insert(rightMeasures.begin(), closestMeasures.begin(),
    //            // closestMeasures.end());
    //            ////    rightT << closestLeftT.getMeasures();
    //            //    Acquisition acq(rightT.m_start, rightT.m_end);

    ////            (void)(tOut << closestLeftT);
    ////            tOut = tOut << closestLeftT;
    ////            tOut |= closestLeftT;
    //            tOut <<= closestLeftT;
    ////            (void)(tOut << closestLeftT);
    ////            tOut += closestLeftT;

    //            //    acq << closestMeasures;
    //            //    acq << rightMeasures;
    //            //    return acq;
    //            //    return rightT;
    //        }

    ////        (void)(tOut << rightT);
    ////        tOut = rightT;
    ////        tOut |= rightT;
    //        tOut <<= rightT;
    ////        tOut << rightT;
    ////        tOut += rightT;

    //        m_leftInputs.clear();
    //        return ret;
//}

// template <typename Acquisition>
//Input& Input::operator>>( Input& input ) {
//    assert( isOpen() );
//    assert( !isEmpty() );
//    assert( input.isOpen() );
//    assert( !input.isEmpty() );

//    assert( input.m_leftInputs.empty() );
//    input.m_leftInputs.swap( m_leftInputs );
//    assert( m_leftInputs.empty() );
//    //    m_leftInputs.clear();

//           //    input.m_leftInputs.emplace_back(std::make_unique<hub::Input>(this));
//    input.m_leftInputs.emplace_back( this );
//    return input;

//           //    Input& leftInput  = *this;
//           //    Input& rightInput = input;

//           //    Acquisition rightAcq;
//           //    rightInput.read( rightAcq );

//           //    return rightInput;
//           //    return *this;
//           //    return rightAcq;

//           //    assert(rightAcq.getMeasures().front().getResolution().second == Format::DOF6);

//           //    auto& leftLastAcqs = leftInput.m_lastAcqs;

//           //    assert( leftLastAcqs.size() < 20 );

//           //    Acquisition leftAcq;

//           //    if ( leftLastAcqs.empty() ) {
//    //        leftInput.read( leftAcq );
//    //        leftLastAcqs.push_back( std::move( leftAcq ) );
//    //    }

//    //    while ( rightAcq.getStart() < leftLastAcqs.front().getStart() ) {
//    //        std::cout << "[InputSensor] operator>>(InputSensor&) shift rightAcq : " << rightAcq <<
//    // std::endl; /        assert( !rightInput.isEmpty() ); /        rightInput.read( rightAcq ); / }

//    //    while ( leftLastAcqs.back().getStart() < rightAcq.getStart() && !leftInput.isEmpty() ) {
//    //        assert( !leftInput.isEmpty() );
//    //        leftInput.read( leftAcq );
//    //        leftLastAcqs.push_back( std::move( leftAcq ) );
//    //    }

//    //    while ( leftLastAcqs.size() > 2 ) {
//    //        leftLastAcqs.pop_front();
//    //    }

//    //    const auto& leftBeforeRightAcq  = leftLastAcqs.front();
//    //    const auto& leftAfterRightAcq = leftLastAcqs.back();

//    //    assert( leftInput.isEmpty() || leftBeforeRightAcq.getStart() <= rightAcq.getStart() );
//    //    assert( leftInput.isEmpty() || rightAcq.getStart() <= leftAfterRightAcq.getStart() );

//    //    const auto& closestAcq =
//    //        ( std::abs( leftBeforeRightAcq.getStart() - rightAcq.getStart() ) > std::abs(
//    // leftAfterRightAcq.getStart() - rightAcq.getStart() ) ) /            ? ( leftAfterRightAcq ) /
//    // : ( leftBeforeRightAcq );

//    //    const auto & rightMeasures = rightAcq.getMeasures();
//    //    const auto & closestMeasures = closestAcq.getMeasures();
//    ////    rightMeasures.insert(rightMeasures.begin(), closestMeasures.begin(),
//    // closestMeasures.end());
//    ////    rightAcq << closestAcq.getMeasures();
//    //    Acquisition acq(rightAcq.m_start, rightAcq.m_end);
//    //    acq << closestMeasures;
//    //    acq << rightMeasures;
//    //    return acq;
//    //    return rightAcq;
//}

//} // namespace io
} // namespace hub

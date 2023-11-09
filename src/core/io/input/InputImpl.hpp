#pragma once

#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
// #include <thread>
#include <vector>

// #define HUB_THREAD_SAFE

// #include "core/Any.hpp"
// #include "core/InputBase.hpp"
//#include "BasicInputI.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"

//#include "core/Serial.hpp"
//#include "core/Tuple.hpp"

//#include "InputBase.hpp"

#if defined( OS_MACOS ) && \
    CPLUSPLUS_VERSION <= 14 // std::void_t not supported by AppleClang (c++14)
#    include "core/Traits.hpp"
#endif

//#ifdef HUB_DEBUG_INPUT
//#    define HEADER_INPUT_MSG                                                             \
//        "\t\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                  << "m[InputImpl:" << this << "]\033[0m "
//#endif

namespace hub {
namespace io {
namespace input {

///
/// \brief The InputImpl class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \note This class is an interface.
/// \warning Unable to perform write/read operations if communication is ended or closed.
/// TODO: split InputImpl -> InputImpl and Output classes
///
//class SRC_API InputImpl : public InputBase
//{
//  public:
//    using InputBase::read;

//    template <typename T>
//    using notReadable_t = decltype( T::notReadable );

//    template <typename T, typename = std::void_t<>>
//    struct notReadable : std::false_type {};

//    template <typename T>
//    struct notReadable<T, std::void_t<notReadable_t<T>>> : std::true_type {};

//    template <typename T>
//    static constexpr bool notReadable_v = notReadable<T>::value;
//    //    static constexpr bool packable_v = ! std::is_constructible_v<T> ;

//    //////////////////////////////////

////    template <typename T>
////    using readable_t = decltype( std::declval<T>().read( std::declval<InputImpl&>() ) );

////    template <typename T, typename = std::void_t<>>
////    struct readable : std::false_type {};

////    template <typename T>
////    struct readable<T, std::void_t<readable_t<T>>> : std::true_type {};

////    template <typename T>
////    static constexpr bool readable_v = readable<T>::value;

//    //////////////////////////////////

//    template <typename T>
//    using getable_t = decltype( T::get( std::declval<InputImpl&>() ) );

//    template <typename T, typename = std::void_t<>>
//    struct getable : std::false_type {};

//    template <typename T>
//    struct getable<T, std::void_t<getable_t<T>>> : std::true_type {};

//    template <typename T>
//    static constexpr bool getable_v = getable<T>::value;

//    //////////////////////////

//  public:
//    InputImpl() = default;

//    ///
//    /// \param input
//    ///
//    InputImpl( const InputImpl& input )            = delete;
//    InputImpl( InputImpl&& input )                 = delete;
//    InputImpl& operator=( const InputImpl& input ) = delete;
//    InputImpl&& operator=( InputImpl&& input )     = delete;

//    virtual ~InputImpl() = default;

//    //    //  protected:
//    //    ///
//    //    /// \brief read
//    //    /// function describes how to read data through from the communication bus.
//    //    /// \param data
//    //    /// [out] array of bytes in memory
//    //    /// \param len
//    //    /// [in] size of the data array to read
//    //    ///
//    //    virtual void read( Data_t* data, Size_t len ) = 0;

//    //        return get<T>();
//    //        return T::get(*this);
//    //            T t = T::get(*this);
//    //        T t;
//    //        return t;

//  public:
//    //    ///
//    //    /// \brief isEmpty
//    //    /// allows to know if the communication bus is no longer active,
//    //    /// no memory bandwidth, no more acquisition coming.
//    //    /// \return
//    //    /// true if the bus is inactive.\n
//    //    /// false otherwise.
//    //    ///
//    //    virtual bool isEnd() const = 0;

//    //    ///
//    //    /// \brief close
//    //    /// is the capability to terminate communication at both ends if possible.
//    //    ///
//    //    virtual void close() = 0;

//    //    ///
//    //    /// \brief isOpen
//    //    /// allows to verify the still alive communication of the bus.
//    //    /// \return
//    //    /// true if the bus is active.\n
//    //    /// false otherwise.
//    //    ///
//    //    virtual bool isOpen() const = 0;

//    //    virtual void clear() = 0;

//    ////////////////////////////////////////////////////////////////////////////

//  public:
//    ///
//    /// \brief read
//    /// \param t
//    ///
//    template <class T>
//    typename std::enable_if<serializable_v<T>>::type read( T& t ) {
////        static_assert( !readable_v<T> );
////        assert(false);

//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
//        assert( isOpen() );
//        assert( !isEnd() );

////        read( t.serialize() );
//        t.serialize(*this);

//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( t ) << ") = " << t
//                  << std::endl;
//#endif
//    }

//    template <class T>
//    typename std::enable_if<readable_v<T>>::type read( T& t ) {
////        static_assert( !serializable_v<T> );
////        assert(false);

//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( t ) << ")" << std::endl;
//#endif
//        assert( isOpen() );
//        assert( !isEnd() );

//        t.read( *this );

//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( t ) << ") = " << t
//                  << std::endl;
//#endif
//    }

//    template <class T>
//    //    typename std::enable_if<!readable_v<T>>::type read( T& t ) {
////    typename std::enable_if<!serializable_v<T> && !readable_v<T> && !notReadable_v<T>>::type
//    typename std::enable_if<!serializable_v<T> && !readable_v<T>>::type
//    read( T& t ) {
////        assert(false);
////#ifdef HUB_DEBUG_INPUT
//////        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
////        std::cout << HEADER << "read(" << TYPE_NAME( t ) << ")" << std::endl;
////#endif
//        assert( isOpen() );
//        //        while (isOpen() && isEnd()) {
//        //        if ( !isOpen() ) {
//        //            std::cout << "[InputImpl" << std::this_thread::get_id() << "]  is closed,
//        //            unable to read data"
//        //                      << std::endl;
//        //            return;
//        //        }
//        assert( !isEnd() );

//        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "read(raw: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//    }

////    template <class T>
////    void operator()( T& t ) {
////        read( t );
////    }

//    template <class... Ts>
//    void operator()(Ts&... ts ) {
////        readAll_(ts...);
//    }

//  private:
//    template <class T>
//    void readAll_( T& t ) {
//        read(t);
////        if constexpr ( sizeof...( Ts ) > 0 ) { readAll_( ts... ); }
//    }
//    template <class T, class... Ts>
//    void readAll_( T& t, Ts&... ts ) {
//        read(t);
//        readAll_(ts...);
////        if constexpr ( sizeof...( Ts ) > 0 ) { readAll_( ts... ); }
//    }

//  public:

//    //    template <class T>
//    //    constexpr typename std::enable_if<getable_v<T>, T>::type get()
//    ////    constexpr T get()
//    //    {

//    ////        return get<T>();
//    ////        return T::get(*this);
//    ////            T t = T::get(*this);
//    ////        T t;
//    ////        return t;
//    //    }

//    template <class T>
//    typename std::enable_if<!getable_v<T>, T>::type get() {
//        //    typename std::enable_if<getable_v<T>, T>::type get() {
//        //              typename = typename std::enable_if<readable_v<T>>::type>
//        //              typename = typename std::enable_if<true>::type>
//        //    T get() {

//        //    template <class T>
//        //    inline T InputImpl::get() {
//        assert( isOpen() );
//        assert( !isEnd() );

//        static_assert( !readable_v<T> && !serializable_v<T> );

//        //    return T::create(*this);
//        //        return T::get( *this );
//        //        if constexpr (readable_v<T>) {
//        if constexpr ( getable_v<T> ) {
//#ifdef HUB_DEBUG_INPUT
////            std::cout << HEADER_INPUT_MSG << "\033[33mget<" << TYPE_NAME( T ) << ">\033[0m()"
//            std::cout << "\t" << HEADER << "\033[33mget<" << TYPE_NAME( T ) << ">\033[0m()"
//                      << std::endl;
//#endif
//            //            T t( *this );
//            T t = T::get( *this );
////            T t = T::get(*this);
//#ifdef HUB_DEBUG_INPUT
////            std::cout << HEADER_INPUT_MSG << "\033[1;33mget<" << TYPE_NAME( t )
//            std::cout << "\t" << HEADER << "\033[1;33mget<" << TYPE_NAME( t )
//                      << ">\033[0m() = " << t << std::endl;
//#endif
//            return t;
//            //            return T(*this);
//            //        if (readable_v<T>) {
//        }
//        else {
//            //            static_assert( !serializable_v<T> );
//            T t;
//            read( t );
//            // #ifdef HUB_DEBUG_INPUT
//            //         std::cout << HEADER_INPUT_MSG << "get<" << TYPE_NAME( t ) << ">() = " << t <<
//            //         std::endl;
//            // #endif
//            return t;
//        }
//    }

//    //    template <class T,
//    //              typename = typename std::enable_if<!readable_v<T>>::type>
//    ////              typename = typename std::enable_if<std::false_type>::type>
//    //    T get() {
//    //        //    template <class T>
//    //        //    inline T InputImpl::get() {
//    //        assert( isOpen() );
//    //        assert( !isEnd() );

//    //        //    return T::create(*this);
//    ////        return T::get( *this );
//    //            T t;
//    //        //    read( t );
//    //        //    return t;
//    //    }

//    //  private:
//    ///
//    /// \brief read
//    /// \param list
//    ///
//    template <class T>
//    void read( std::list<T>& list );

//    ///
//    /// \brief read
//    /// \param vector
//    ///
//    template <class T>
//    void read( std::vector<T>& vector );

//    ///
//    /// \brief read
//    /// \param map
//    ///
//    template <class T, class U>
//    void read( std::map<T, U>& map );

//    ///
//    /// \brief read
//    /// \param pair
//    ///
//    template <class T, class U>
//    void read( std::pair<T, U>& pair );

//    ///
//    /// \brief read
//    /// \param str
//    ///
//    void read( char* str );

//    ///
//    /// \brief read
//    /// \param str
//    ///
//    void read( std::string& str );

//#ifdef ARCH_X86
//    void read( size_t size ) = delete; // non compatible format 32/64 bit
//#endif

//    template <std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t );

//    template <std::size_t I = 0, typename... Tp>
//        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t );

//    template <std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...> && t );

//    template <std::size_t I = 0, typename... Tp>
//        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...> && t );

//  public:
//    ///
//    /// \brief readAll
//    /// \param ts
//    ///
//    template <typename Container,
//              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
//    void readAll( Container& ts );

//    //    ///
//    //    /// \brief get
//    //    /// \return
//    //    ///
//    //    template <class T>
//    //    T get();

//    ///
//    /// \brief getAll
//    /// \return
//    ///
//    template <typename Container>
//    Container getAll();

//    ///
//    /// \brief operator >>
//    /// \param t
//    ///
//    template <class T>
//    //    template <class T,
//    //              typename = typename std::enable_if<! std::is_base_of_v<InputImpl, T>>>
//    void operator>>( T& t );

//    //    void read(InputImpl & input) {
//    //        int a;
//    //        input.read(a);
//    //    }

//    //    template <typename Acquisition>
//    //    Acquisition operator>>( InputImpl& input );
//    //    InputImpl& operator>>( InputImpl& input );

//    // sensor::Acquisition InputImpl::operator>>( InputImpl& input ) {

//  private:
//    //    std::list<sensor::Acquisition> m_lastAcqs;
//    //    size_t m_id;
//    //    std::list<std::any> m_lastTs;
//    //    std::list<hub::InputImpl*> m_leftInputs;
//};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//template <class T>
//inline void InputImpl::read( std::list<T>& list ) {
//    assert( isOpen() );
//    assert( !isEnd() );

//#ifdef HUB_DEBUG_INPUT
//    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( list ) << ")" << std::endl;
//#endif

//    int nbEl;
//    read( nbEl );

//    for ( int i = 0; i < nbEl; ++i ) {
//        T el;
//        read( el );
//        list.push_back( std::move( el ) );
//    }
//}

//template <class T>
//inline void InputImpl::read( std::vector<T>& vector ) {
//    assert( isOpen() );
//    assert( !isEnd() );

//#ifdef HUB_DEBUG_INPUT
////    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
//    std::cout <<  "\t" <<HEADER << "read(" << TYPE_NAME( vector ) << ")" << std::endl;
//#endif

//    uint64_t nbEl;
//    static_assert(sizeof(uint64_t) == 8);
//    read( nbEl );

//    vector.clear();
//    vector.reserve( nbEl );

//    for ( int i = 0; i < nbEl; ++i ) {
//        //        if constexpr ( getable_v<T> ) { vector.push_back( get<T>() ); }
//        //        else {
//        T el;
//        read( el );
//        vector.push_back( std::move( el ) );
//        //        }
//    }
//}

//template <class T, class U>
//inline void InputImpl::read( std::map<T, U>& map ) {
//    assert( isOpen() );
//    assert( !isEnd() );

//#ifdef HUB_DEBUG_INPUT
//    //    std::cout << HEADER_INPUT_MSG "read(std::map)" << std::endl;
//    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( map ) << ")" << std::endl;
//#endif

//    int nbEl;
//    read( nbEl );
//    map.clear();

//    for ( int i = 0; i < nbEl; ++i ) {
//        std::pair<T, U> pair;
//        read( pair );
//        assert( map.find( pair.first ) == map.end() );
//        map.emplace( std::move( pair ) );
//    }
//}

//template <class T, class U>
//inline void InputImpl::read( std::pair<T, U>& pair ) {
//    assert( isOpen() );
//    assert( !isEnd() );

//#ifdef HUB_DEBUG_INPUT
//    //    std::cout << HEADER_INPUT_MSG "read(std::pair)" << std::endl;
//    std::cout << "\t" << HEADER << "read(" << TYPE_NAME( pair ) << ")" << std::endl;
//#endif
//    T first;
//    read( first );
//    U second;
//    read( second );
//    pair = std::make_pair( first, std::move( second ) );
//}

//template <std::size_t I, typename... Tp>
//inline typename std::enable_if<I == sizeof...( Tp ), void>::type
//InputImpl::read( std::tuple<Tp...>& t ) {
//#ifdef HUB_DEBUG_INPUT
//    std::cout << "\t" << HEADER << "\033[1mread\033[0m(" << TYPE_NAME( t ) << ") : '" << t << "'"
//              << std::endl;
//#endif
//}

//template <std::size_t I, typename... Tp>
//    typename std::enable_if <
//    I<sizeof...( Tp ), void>::type InputImpl::read( std::tuple<Tp...>& t ) {

//    if constexpr ( static_cast<int>( I ) == 0 ) {
//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ")" << std::endl;
//#endif
//    }
//    read( std::get<I>( t ) );

//    read<I + 1, Tp...>( t );
//}

//template <std::size_t I, typename... Tp>
//inline typename std::enable_if<I == sizeof...( Tp ), void>::type
//InputImpl::read( std::tuple<Tp...>&& t ) {
//#ifdef HUB_DEBUG_INPUT
//    std::cout << "\t" << HEADER << "\033[1mread\033[0m(" << TYPE_NAME( t ) << ") : '" << t << "'"
//              << std::endl;
//#endif
//}

//template <std::size_t I, typename... Tp>
//    typename std::enable_if <
//    I<sizeof...( Tp ), void>::type InputImpl::read( std::tuple<Tp...>&& t ) {

//    if constexpr ( static_cast<int>( I ) == 0 ) {
//#ifdef HUB_DEBUG_INPUT
//        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ")" << std::endl;
//#endif
//    }
//    read( std::get<I>( t ) );

//    read<I + 1, Tp...>( t );
//}

//// template <class T>
//// template <class T,
////           typename>
//// inline void InputImpl::operator>>( T& t ) {
////     assert( isOpen() );
////     assert( !isEnd() );

////    read( t );
////}

//template <typename Container, typename T>
//void InputImpl::readAll( Container& ts ) {
//    assert( isOpen() );
//    assert( !isEnd() );

//    //    ts.reserve( ts.size() );
//    try {
//        while ( !isEnd() ) {
//            T t;
//            read( t );
//            ts.push_back( std::move( t ) );
//            //            ts.emplace_back( get<T>() );
//        }
//    }
//    catch ( std::exception& e ) {
//        std::cout << "[InputImpl] catch exception : " << e.what() << std::endl;
//        throw;
//    }
//}

//template <typename Container>
//Container InputImpl::getAll() {
//    assert( isOpen() );
//    assert( !isEnd() );

//    Container ts;
//    readAll( ts );
//    return ts;
//}

//template <class T>
//void InputImpl::operator>>( T& t ) {
//    assert( isOpen() );
//    //    assert( !isEnd() );

//    read( t );
//}

////        std::cout << "[InputImpl] operator>> ";
////        //        for (int i = 0; i < m_leftInputs.size(); ++i) {
////        for ( auto* leftInput : m_leftInputs ) {
////            std::cout << leftInput << " ";
////        }
////        std::cout << std::endl;

//////        tOut = 1;

////        auto& rightInput = *this;
////        T rightT;
////        rightInput.read( rightT );

////        //        read( t );
//////        T ret;

////        for ( auto* leftInputPtr : m_leftInputs ) {
////            auto& leftInput  = *leftInputPtr;
////            auto& leftLastTs = leftInput.m_lastTs;
////            assert( leftLastTs.size() < 20 );

////            if ( leftLastTs.empty() ) {
////                T leftT;
////                leftInput.read( leftT );
////                leftLastTs.push_back( std::move( leftT ) );
////            }

////            //                while ( rightT.getStart() < leftLastTs.front().getStart() ) {
////            while ( rightT < std::any_cast<const T&>(leftLastTs.front()) ) {
////                std::cout << "[InputImpl] operator>>(InputImpl&) shift rightT : " << rightT
////                          << std::endl;
////                assert( !rightInput.isEnd() );
////                rightInput.read( rightT );
////            }

////            //                while ( leftLastTs.back().getStart() < rightT.getStart() &&
////            //                !leftInput.isEnd()
////            while ( std::any_cast<const T&>(leftLastTs.back()) < rightT && !leftInput.isEnd() )
////            {
////                T leftT;
////                assert( !leftInput.isEnd() );
////                leftInput.read( leftT );
////                leftLastTs.push_back( std::move( leftT ) );
////            }

////            while ( leftLastTs.size() > 2 ) {
////                leftLastTs.pop_front();
////            }

////            const auto& leftTBeforeRightT = std::any_cast<const T&>(leftLastTs.front());
////            const auto& leftTAfterRightT  = std::any_cast<const T&>(leftLastTs.back());

////            //                assert( leftInput.isEnd() || leftTBeforeRightT.getStart() <=
////            //                rightT.getStart());
////            assert( leftInput.isEnd() || leftTBeforeRightT <= rightT );
////            //                assert( leftInput.isEnd() || rightT.getStart() <= leftTAfterRightT
////            ); assert( leftInput.isEnd() || rightT <= leftTAfterRightT );

////            const auto& closestLeftT =
////                ( std::abs( leftTBeforeRightT - rightT ) > std::abs( leftTAfterRightT - rightT ) )
////                    ? ( leftTAfterRightT )
////                    : ( leftTBeforeRightT );

////            //    const auto & rightMeasures = rightT.getMeasures();
////            //    const auto & closestMeasures = closestLeftT.getMeasures();
////            ////    rightMeasures.insert(rightMeasures.begin(), closestMeasures.begin(),
////            // closestMeasures.end());
////            ////    rightT << closestLeftT.getMeasures();
////            //    Acquisition acq(rightT.m_start, rightT.m_end);

//////            (void)(tOut << closestLeftT);
//////            tOut = tOut << closestLeftT;
//////            tOut |= closestLeftT;
////            tOut <<= closestLeftT;
//////            (void)(tOut << closestLeftT);
//////            tOut += closestLeftT;

////            //    acq << closestMeasures;
////            //    acq << rightMeasures;
////            //    return acq;
////            //    return rightT;
////        }

//////        (void)(tOut << rightT);
//////        tOut = rightT;
//////        tOut |= rightT;
////        tOut <<= rightT;
//////        tOut << rightT;
//////        tOut += rightT;

////        m_leftInputs.clear();
////        return ret;
////}

//// template <typename Acquisition>
//// InputImpl& InputImpl::operator>>( InputImpl& input ) {
////    assert( isOpen() );
////    assert( !isEnd() );
////    assert( input.isOpen() );
////    assert( !input.isEnd() );

////    assert( input.m_leftInputs.empty() );
////    input.m_leftInputs.swap( m_leftInputs );
////    assert( m_leftInputs.empty() );
////    //    m_leftInputs.clear();

////           //    input.m_leftInputs.emplace_back(std::make_unique<hub::InputImpl>(this));
////    input.m_leftInputs.emplace_back( this );
////    return input;

////           //    InputImpl& leftInput  = *this;
////           //    InputImpl& rightInput = input;

////           //    Acquisition rightAcq;
////           //    rightInput.read( rightAcq );

////           //    return rightInput;
////           //    return *this;
////           //    return rightAcq;

////           //    assert(rightAcq.getMeasures().front().getResolution().second == Format::DOF6);

////           //    auto& leftLastAcqs = leftInput.m_lastAcqs;

////           //    assert( leftLastAcqs.size() < 20 );

////           //    Acquisition leftAcq;

////           //    if ( leftLastAcqs.empty() ) {
////    //        leftInput.read( leftAcq );
////    //        leftLastAcqs.push_back( std::move( leftAcq ) );
////    //    }

////    //    while ( rightAcq.getStart() < leftLastAcqs.front().getStart() ) {
////    //        std::cout << "[InputSensor] operator>>(InputSensor&) shift rightAcq : " << rightAcq
////    <<
////    // std::endl; /        assert( !rightInput.isEnd() ); /        rightInput.read( rightAcq );
////    / }

////    //    while ( leftLastAcqs.back().getStart() < rightAcq.getStart() && !leftInput.isEnd() ) {
////    //        assert( !leftInput.isEnd() );
////    //        leftInput.read( leftAcq );
////    //        leftLastAcqs.push_back( std::move( leftAcq ) );
////    //    }

////    //    while ( leftLastAcqs.size() > 2 ) {
////    //        leftLastAcqs.pop_front();
////    //    }

////    //    const auto& leftBeforeRightAcq  = leftLastAcqs.front();
////    //    const auto& leftAfterRightAcq = leftLastAcqs.back();

////    //    assert( leftInput.isEnd() || leftBeforeRightAcq.getStart() <= rightAcq.getStart() );
////    //    assert( leftInput.isEnd() || rightAcq.getStart() <= leftAfterRightAcq.getStart() );

////    //    const auto& closestAcq =
////    //        ( std::abs( leftBeforeRightAcq.getStart() - rightAcq.getStart() ) > std::abs(
////    // leftAfterRightAcq.getStart() - rightAcq.getStart() ) ) /            ? ( leftAfterRightAcq )
////    /
////    // : ( leftBeforeRightAcq );

////    //    const auto & rightMeasures = rightAcq.getMeasures();
////    //    const auto & closestMeasures = closestAcq.getMeasures();
////    ////    rightMeasures.insert(rightMeasures.begin(), closestMeasures.begin(),
////    // closestMeasures.end());
////    ////    rightAcq << closestAcq.getMeasures();
////    //    Acquisition acq(rightAcq.m_start, rightAcq.m_end);
////    //    acq << closestMeasures;
////    //    acq << rightMeasures;
////    //    return acq;
////    //    return rightAcq;
////}

} // namespace input
} // namespace io
} // namespace hub

#pragma once

#include <exception>
#include <list>
#include <map>
#include <tuple>
#include <vector>
// #include <semaphore>

// #define HUB_THREAD_SAFE
// #ifdef HUB_THREAD_SAFE
// #include <mutex>
// #endif

// #include "core/Any.hpp"
// #include "core/OutputI.hpp"
#include "core/Macros.hpp"
#include "core/Traits.hpp"

#include "core/Tuple.hpp"

#include "OutputI.hpp"

#if defined( OS_MACOS ) && CPLUSPLUS_VERSION <= 14
#    include "core/Traits.hpp"
#endif

// #ifdef HUB_DEBUG_OUTPUT
// #    def ine HEAD ER_OUTPUT_MSG                                                          \
//        "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
//                << "m[OutputImpl:" << this << "]\033[0m "
// #endif

namespace hub {
namespace io {
namespace output {

///
/// \brief The OutputImpl class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \note This class is an interface.
/// \warning Unable to perform write/read operations if communication is ended or closed.
/// TODO: split OutputImpl -> Input and OutputImpl classes
///
//class SRC_API OutputImpl : public OutputI
//{
//  public:
//    using OutputI::write;

//    template <typename T>
//    using notWritable_t = decltype( T::notWritable );

//    template <typename T, typename = std::void_t<>>
//    struct notWritable : std::false_type {};

//    template <typename T>
//    struct notWritable<T, std::void_t<notWritable_t<T>>> : std::true_type {};

//    template <typename T>
//    static constexpr bool notWritable_v = notWritable<T>::value;

//    //////////////////////////////////////

//    //    template <typename T>
//    //    using writable_t = decltype( std::declval<T>().write( std::declval<OutputImpl&>() ) );
//    //    //    using writable_t = decltype( write( std::declval<T>(), std::declval<OutputImpl&>() )
//    //    );

//    //    template <typename T, typename = std::void_t<>>
//    //    struct writable : std::false_type {};

//    //    template <typename T>
//    //    struct writable<T, std::void_t<writable_t<T>>> : std::true_type {};

//    //    template <typename T>
//    //    static constexpr bool writable_v = writable<T>::value;

//  public:
//    OutputImpl() = default;

//    ///
//    /// \param output
//    ///
//    OutputImpl( const OutputImpl& output )            = delete;
//    OutputImpl( OutputImpl&& output )                 = delete;
//    OutputImpl& operator=( const OutputImpl& output ) = delete;
//    OutputImpl&& operator=( OutputImpl&& output )     = delete;

//    //    virtual ~OutputImpl();

//  public:
//    //    ///
//    //    /// \brief write
//    //    /// function describes how to write data through the communication bus.
//    //    /// \param data
//    //    /// [in] array of bytes in memory
//    //    /// \param len
//    //    /// [in] size of the data array to write
//    //    ///
//    //    virtual void write( const Data_t* data, Size_t len ) = 0;

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

//  public:
//    //    template <class... Ts>
//    //    void write(const std::tuple<Ts...> & serial) {

//    //    }

//    ///
//    /// \brief write
//    /// \param t
//    ///
//    template <class T>
//    //    typename std::enable_if<writable_v<T>>::type write( const T& t ) {
//    typename std::enable_if<serializable_v<T>>::type write( const T& t ) {
//        static_assert( !writable_v<T> );
//        //        assert(false);

//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.lock();
//        // #endif
//        assert( isOpen() );
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(serial: " << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
//        //        t.write( *this );
//        //        write( const_cast<T&>( t ).serialize() );
//        const_cast<T&>( t ).serialize( *this );
//        //        write( reinterpret_cast<T&>(t).serialize() );

//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(serial: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif

//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.unlock();
//        // #endif
//    }

//    template <class T>
//    //    typename std::enable_if<writable_v<T>>::type write( const T& t ) {
//    typename std::enable_if<writable_v<T>>::type write( const T& t ) {
////        static_assert( !serializable_v<T> );
////        assert( false );

//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.lock();
//        // #endif
//        assert( isOpen() );
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
//        t.write( *this );
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//        //        write(t, *this);

//        //        write( const_cast<T&>(t).serialize() );
//        //        write( reinterpret_cast<T&>(t).serialize() );

//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.unlock();
//        // #endif
//    }

//    template <class T>
//    //    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
////    typename std::enable_if<!serializable_v<T> && !writable_v<T> && !notWritable_v<T>>::type
//    typename std::enable_if<!serializable_v<T> && !writable_v<T>>::type
//    write( const T& t ) {
//        //        OutputI::write(t);
//        //        return;
//        //        assert(false);

//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.lock();
//        // #endif
//        assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(raw: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//        // #ifdef HUB_THREAD_SAFE
//        //         m_mtx.unlock();
//        // #endif
//    }

//    template <class... Ts>
//    void operator()( const Ts&... ts ) {
//        //        write(ts...);
////        if constexpr (sizeof...(ts) > 0) { writeAll_( ts... ); };
//    }

//  private:
//    template <class T>
//    void writeAll_( const T& t ) {
//        write( t );
//    }
//    template <class T, class... Ts>
//    void writeAll_( const T& t, const Ts&... ts ) {
////#ifdef HUB_DEBUG_OUTPUT
//////        std::cout << HEADER << "write(raw: " << TYPE_NAME( ts ) << ") = " << t << std::endl;
////#endif
//        //        assert( isOpen() );
//        write( t );
//        writeAll_(ts...);
////        if constexpr ( sizeof...( Ts ) > 0 ) { writeAll_( ts... ); }
//    }

//  public:
//    ///
//    /// \brief write
//    /// \param list
//    ///
//    template <class T>
//    void write( const std::list<T>& list );

//    ///
//    /// \brief write
//    /// \param vector
//    ///
//    template <class T>
//    void write( const std::vector<T>& vector );

//    ///
//    /// \brief write
//    /// \param map
//    ///
//    template <class T, class U>
//    void write( const std::map<T, U>& map );

//    ///
//    /// \brief write
//    /// \param pair
//    ///
//    template <class T, class U>
//    void write( const std::pair<T, U>& pair );

//  public:
//    ///
//    /// \brief write
//    /// \param any
//    ///
//    //    void write( const Any& any );

//    ///
//    /// \brief write
//    /// \param str
//    ///
//    void write( const char* str );

//    void write( char* str ) = delete; // non compatible format 32/64 bit

//    ///
//    /// \brief write
//    /// \param str
//    ///
//    void write( const std::string& str );

//    //    virtual void write(uint64_t size);

//#ifdef ARCH_X86
//    void write( size_t size ) = delete; // non compatible format 32/64 bit
//#endif

//    template <std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I == sizeof...( Tp ), void>::type
//    write( const std::tuple<Tp...>& t );

//    template <std::size_t I = 0, typename... Tp>
//        typename std::enable_if <
//        I<sizeof...( Tp ), void>::type write( const std::tuple<Tp...>& t );

//    template <std::size_t I = 0, typename... Tp>
//    inline typename std::enable_if<I == sizeof...( Tp ), void>::type write( std::tuple<Tp...>&& t );

//    template <std::size_t I = 0, typename... Tp>
//        typename std::enable_if < I<sizeof...( Tp ), void>::type write( std::tuple<Tp...>&& t );

//    // void writeAll
//    template <typename Container>
//    //              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
//    void writeAll( Container& c );

//    ///
//    /// \brief put
//    /// \param t
//    ///
//    template <class T>
//    void put( const T& t );

//    //    void putAll( const T& t );
//    template <typename Container>
//    void putAll( Container& c );

//    ///
//    /// \brief operator <<
//    /// \param t
//    ///
//    template <class T>
//    void operator<<( const T& t );

//    ////////////////////////////////////////////////////////////////////////////
//  private:
//#ifdef HUB_THREAD_SAFE
//    std::mutex m_mtx;
//#endif
//};

//template <class T>
//inline void OutputImpl::write( const std::list<T>& list ) {
//    assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//    std::cout << HEADER << "write(" << TYPE_NAME( list ) << ")" << std::endl;
//#endif

//    int nbEl = list.size(); // todo
//    write( nbEl );

//    for ( const T& el : list ) {
//        write( el );
//    }
//}

//template <class T>
//inline void OutputImpl::write( const std::vector<T>& vector ) {
//    assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//    std::cout << HEADER << "write(" << TYPE_NAME( vector ) << ")" << std::endl;
//#endif

//    uint64_t nbEl = static_cast<uint64_t>( vector.size() );
//    write( nbEl );

//    for ( const T& el : vector ) {
//        write( el );
//    }
//}

//template <class T, class U>
//inline void OutputImpl::write( const std::map<T, U>& map ) {
//    assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//    std::cout << HEADER << "write(" << TYPE_NAME( map ) << ")" << std::endl;
//#endif

//    int nbKey = static_cast<int>( map.size() );
//    write( nbKey );
//    // #ifdef HUB_DEBUG_OUTPUT
//    //     std::cout << HEADER_OUTPUT_MSG "map : nbEl = " << nbKey << std::endl;
//    // #endif

//    for ( const std::pair<T, U>& pair : map ) {
//        write( pair );
//    }
//}

//template <class T, class U>
//inline void OutputImpl::write( const std::pair<T, U>& pair ) {
//    assert( isOpen() );

//#ifdef HUB_DEBUG_OUTPUT
//    std::cout << HEADER << "write(" << TYPE_NAME( pair ) << ")" << std::endl;
//#endif
//    const T& first  = pair.first;
//    const U& second = pair.second;
//    write( first );
//    write( second );
//}

//template <std::size_t I, typename... Tp>
//inline typename std::enable_if<I == sizeof...( Tp ), void>::type
//OutputImpl::write( const std::tuple<Tp...>& t ) {}

//template <std::size_t I, typename... Tp>
//    typename std::enable_if <
//    I<sizeof...( Tp ), void>::type OutputImpl::write( const std::tuple<Tp...>& t ) {

//    if constexpr ( static_cast<int>( I ) == 0 ) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") : '" << t << "'" << std::endl;
//#endif
//    }
//    //        std::cout << std::get<I>( t ) << std::endl;
//    write( std::get<I>( t ) );

//    write<I + 1, Tp...>( t );
//}

//template <std::size_t I, typename... Tp>
//inline typename std::enable_if<I == sizeof...( Tp ), void>::type
//OutputImpl::write( std::tuple<Tp...>&& t ) {}

//template <std::size_t I, typename... Tp>
//    typename std::enable_if <
//    I<sizeof...( Tp ), void>::type OutputImpl::write( std::tuple<Tp...>&& t ) {

//    if constexpr ( static_cast<int>( I ) == 0 ) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(" << TYPE_NAME( t ) << ") : '" << t << "'" << std::endl;
//#endif
//    }
//    //        std::cout << std::get<I>( t ) << std::endl;
//    write( std::get<I>( t ) );

//    write<I + 1, Tp...>( t );
//}

//template <typename Container>
//void OutputImpl::writeAll( Container& ts ) {
//    assert( isOpen() );

//    for ( const auto& t : ts ) {
//        write( t );
//    }
//}

//template <class T>
//inline void OutputImpl::put( const T& t ) {
//    assert( isOpen() );
//    write( t );
//}

//template <class T>
//inline void OutputImpl::operator<<( const T& t ) {
//    assert( isOpen() );
//    write( t );
//}

//template <typename Container>
//void OutputImpl::putAll( Container& ts ) {
//    assert( isOpen() );

//    writeAll( ts );
//}

} // namespace output
} // namespace io
} // namespace hub

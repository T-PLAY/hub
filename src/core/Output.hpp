#pragma once

#include <exception>
#include <list>
#include <map>

#define HUB_THREAD_SAFE
#ifdef HUB_THREAD_SAFE
#include <mutex>
#endif

#include "core/Any.hpp"
#include "core/Macros.hpp"
#if defined(OS_MACOS) && CPLUSPLUS_VERSION <= 14
#include "core/Traits.hpp"
#endif

#ifdef HUB_DEBUG_OUTPUT
#    define HEADER_OUTPUT_MSG                                                          \
        "\033[" << std::to_string( 31 + reinterpret_cast<std::uintptr_t>( this ) % 7 ) \
                << "m[Output:" << this << "]\033[0m "
#endif

namespace hub {

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

  public:
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
    /// \brief write
    /// \param t
    ///
    template <class T>
    typename std::enable_if<writable_v<T>>::type write( const T& t ) {
#ifdef HUB_THREAD_SAFE
        m_mtx.lock();
#endif
        assert( isOpen() );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        t.write( *this );
#ifdef HUB_THREAD_SAFE
        m_mtx.unlock();
#endif
    }

    template <class T>
    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
#ifdef HUB_THREAD_SAFE
        m_mtx.lock();
#endif
        assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
#ifdef HUB_THREAD_SAFE
        m_mtx.unlock();
#endif
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

    //    virtual void write(uint64_t size);

#ifdef ARCH_X86
    void write( size_t size ) = delete; // non compatible format 32/64 bit
#endif

    // void writeAll
    template <typename Container>
//              typename T = std::decay_t<decltype( *begin( std::declval<Container>() ) )>>
    void writeAll( Container& c );

    ///
    /// \brief put
    /// \param t
    ///
    template <class T>
    void put( const T& t );

    //    void putAll( const T& t );
    template <typename Container>
    void putAll(Container & c);

    ///
    /// \brief operator <<
    /// \param t
    ///
    template <class T>
    void operator<<( const T& t );

           ////////////////////////////////////////////////////////////////////////////
  private:
#ifdef HUB_THREAD_SAFE
    std::mutex m_mtx;
#endif
};



template <class T>
inline void Output::write( const std::list<T>& list ) {
    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( list ) << ")" << std::endl;
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

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( vector ) << ")" << std::endl;
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

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( map ) << ")" << std::endl;
#endif

    int nbKey = static_cast<int>( map.size() );
    write( nbKey );
    // #ifdef HUB_DEBUG_OUTPUT
    //     std::cout << HEADER_OUTPUT_MSG "map : nbEl = " << nbKey << std::endl;
    // #endif

    for ( const std::pair<T, U>& pair : map ) {
        write( pair );
    }
}

template <class T, class U>
inline void Output::write( const std::pair<T, U>& pair ) {
    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER_OUTPUT_MSG "write(" << TYPE_NAME( pair ) << ")" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}

template<typename Container>
void Output::writeAll(Container &ts) {
    assert( isOpen() );

    for ( const auto& t : ts ) {
        write( t );
    }
}


template <class T>
inline void Output::put( const T& t ) {
    assert( isOpen() );
    write( t );
}

template <class T>
inline void Output::operator<<( const T& t ) {
    assert( isOpen() );
    write( t );
}

template<typename Container>
void Output::putAll(Container & ts)
{
    assert( isOpen() );

    writeAll( ts );
}

//} // namespace io
} // namespace hub

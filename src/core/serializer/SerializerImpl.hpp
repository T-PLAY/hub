#pragma once

// #include <vector>
#include <array>
#include <exception>
#include <list>
#include <map>
#include <tuple>
#include <vector>
#include <cstring>

// #include "io/input/InputI.hpp"
// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"
#include "core/Macros.hpp"
#include "core/Vector.hpp"
#include "core/ioBase.hpp"

#include "SerializerI.hpp"

namespace hub {
namespace serializer {

class SerializerImpl : public SerializerI
{
    static constexpr Size_t BuffSize = 1'000'000;

  public:
    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        //        zpp::bits::out out( m_serialBuff );
        //        assert( out.position() == 0 );
        //        out( ts... ).or_throw();
        m_position = 0;
        writeAll( ts... );

        //                const Size_t size = out.position();
        const Size_t size = m_position;
        assert( size < BuffSize );
        output.write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        //                output.write( m_serialBuff.data(), size );
        output.write( m_serialBuff.data(), size );
    }

    template <class Input, class... Ts>
    void unpack( Input& input, Ts&... ts ) {
        Size_t size;
        input.read( reinterpret_cast<Data_t*>( &size ), sizeof( Size_t ) );
        m_position = 0;
        assert( size < BuffSize );
        input.read( m_serialBuff.data(), size );

        //        zpp::bits::in in( m_serialBuff );
        //        in( ts... ).or_throw();
        readAll( ts... );
        assert( m_position == size );
        //        assert()
    }

  private:
    virtual void write( const Data_t* data, Size_t size ) {
#ifdef HUB_DEBUG_OUTPUT
        std::vector<Data_t> vector(data, data + size);
        std::cout << "<---" << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif

        memcpy( &m_serialBuff[m_position], data, size );
        m_position += size;
    }

    template <class T>
    //    typename std::enable_if<!writable_v<T>>::type write( const T& t ) {
    //    typename std::enable_if<!serializable_v<T> && !writable_v<T> && ! notWritable_v<T>>::type
    //    write( const T& t ) {
    //        typename std::enable_if_t<std::isww
    typename std::enable_if_t<packable_v<T>, void>
    //    void write( const T& t ) {
    write( const T& t ) {
//        assert(false);
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(" << TYPE_NAME( T ) << ") = " << t << std::endl;
#endif
        //        assert( isOpen() );
        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

    template <class T>
    void writeAll( const T& t ) {
        write( t );
    }
    template <class T, class... Ts>
    void writeAll( const T& t, const Ts&... ts ) {
        write( t );
        writeAll( ts... );
    }

    //////////////////////////////////////////////////////////////////////////////

    virtual void read( Data_t* data, Size_t size ) {
        memcpy( data, &m_serialBuff[m_position], size );
        m_position += size;

#ifdef HUB_DEBUG_INPUT
        std::vector<Data_t> vector(data, data + size);
        std::cout << "\t --->" << HEADER << "read(Data_t*, Size_t) : data = " << vector
                  << ", read size = " << size << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if_t<packable_v<T>, void>
    //    void read( T& t ) {
    read( T& t ) {
        ////#ifdef HUB_DEBUG_INPUT
        ////        std::cout << HEADER_INPUT_MSG << "read(" << TYPE_NAME( t ) << ")" << std::endl;
        ////        std::cout << "\t" << HEADER << "read(" << TYPE_NAME( t ) << ") ..." <<
        /// std::endl;
        ////#endif
        //        assert( isOpen() );
        //        assert( !isEnd() );

        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( t ) << ") = " << t << std::endl;
#endif
    }

    template <class T>
    void readAll( T& t ) {
        read( t );
    }
    template <class T, class... Ts>
    void readAll( T& t, Ts&... ts ) {
        read( t );
        readAll( ts... );
    }

    ////////////////////////////////////////////////////////////////////

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
    typename std::enable_if_t<packable_v<T>, void>
    //    void
    write( const std::vector<T>& vector );

    template <class T>
    typename std::enable_if_t<!packable_v<T>, void>
    //    void
    write( const std::vector<T>& vector );

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

    //  public:
    ///
    /// \brief write
    /// \param any
    ///
    //    void write( const Any& any );

    ///
    /// \brief write
    /// \param str
    ///
//    void write( const char* str );

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

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type
    write( const std::tuple<Tp...>& t );

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if <
        I<sizeof...( Tp ), void>::type write( const std::tuple<Tp...>& t );

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type write( std::tuple<Tp...>&& t );

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type write( std::tuple<Tp...>&& t );

    //////////////////////////////////////////////////////////////////////////////

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
    typename std::enable_if_t<packable_v<T>, void>
    //    void
    read( std::vector<T>& vector );

    template <class T>
    typename std::enable_if_t<!packable_v<T>, void>
    //    void
    read( std::vector<T>& vector );

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
//    void read( char* str );

    ///
    /// \brief read
    /// \param str
    ///
    void read( std::string& str );

#ifdef ARCH_X86
    void read( size_t size ) = delete; // non compatible format 32/64 bit
#endif

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t );

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t );

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...>&& t );

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...>&& t );

  private:
    //    std::vector<Data_t> m_serial;
    std::array<unsigned char, BuffSize> m_serialBuff;
    Size_t m_position = 0;
    //    Result m_result{m_serial.data(), 0};
};

////////////////////////////////////////////////////////////////////////////

template <class T>
inline void SerializerImpl::write( const std::list<T>& list ) {
    //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << HEADER << "write(" << TYPE_NAME( list ) << ")" << std::endl;
#endif

    int nbEl = list.size(); // todo
    write( nbEl );

    for ( const T& el : list ) {
        write( el );
    }
}

template <class T>
typename std::enable_if_t<packable_v<T>, void>
// void
SerializerImpl::write( const std::vector<T>& vector ) {
    // template <class T>
    // inline void SerializerImpl::write( const std::vector<T>& vector ) {
    //     assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "<---" << HEADER << "write(" << TYPE_NAME( vector ) << ")" << std::endl;
#endif

    uint64_t nEl = static_cast<uint64_t>( vector.size() );
    write( nEl );
    write( (unsigned char*)vector.data(), nEl * sizeof( T ) );
    //    write((unsigned char*)vector.data(), nEl);
    //    write( nbEl );
    //    for ( const El& el : vector ) {
    //        write( el );
    //    }
}

template <class T>
typename std::enable_if_t<!packable_v<T>, void>
SerializerImpl::write( const std::vector<T>& vector ) {
    // template <class T>
    // inline void SerializerImpl::write( const std::vector<T>& vector ) {
    //     assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "<---" << HEADER << "write(" << TYPE_NAME( vector ) << ")" << std::endl;
#endif

    uint64_t nbEl = static_cast<uint64_t>( vector.size() );
    write( nbEl );

    for ( const T& el : vector ) {
        write( el );
    }
}

template <class T, class U>
inline void SerializerImpl::write( const std::map<T, U>& map ) {
    //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "<---" << HEADER << "write(" << TYPE_NAME( map ) << ")" << std::endl;
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
inline void SerializerImpl::write( const std::pair<T, U>& pair ) {
    //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
    std::cout << "<---" << HEADER << "write(" << TYPE_NAME( pair ) << ")" << std::endl;
#endif
    const T& first  = pair.first;
    const U& second = pair.second;
    write( first );
    write( second );
}

template <std::size_t I, typename... Tp>
inline typename std::enable_if<I == sizeof...( Tp ), void>::type
SerializerImpl::write( const std::tuple<Tp...>& t ) {}

template <std::size_t I, typename... Tp>
    typename std::enable_if <
    I<sizeof...( Tp ), void>::type SerializerImpl::write( const std::tuple<Tp...>& t ) {

    if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(" << TYPE_NAME( t ) << ") : '" << t << "'" << std::endl;
#endif
    }
    //        std::cout << std::get<I>( t ) << std::endl;
    write( std::get<I>( t ) );

    write<I + 1, Tp...>( t );
}

template <std::size_t I, typename... Tp>
inline typename std::enable_if<I == sizeof...( Tp ), void>::type
SerializerImpl::write( std::tuple<Tp...>&& t ) {}

template <std::size_t I, typename... Tp>
    typename std::enable_if <
    I<sizeof...( Tp ), void>::type SerializerImpl::write( std::tuple<Tp...>&& t ) {

    if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(" << TYPE_NAME( t ) << ") : '" << t << "'" << std::endl;
#endif
    }
    //        std::cout << std::get<I>( t ) << std::endl;
    write( std::get<I>( t ) );

    write<I + 1, Tp...>( t );
}

////////////////////////////////////////////////////////////////////////////////

template <class T>
inline void SerializerImpl::read( std::list<T>& list ) {
    //    assert( isOpen() );
    //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( list ) << ")" << std::endl;
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
typename std::enable_if_t<packable_v<T>, void>
// inline void
SerializerImpl::read( std::vector<T>& vector ) {
    //    assert( isOpen() );
    //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
    std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( vector ) << ")" << std::endl;
#endif

    uint64_t nEl;
    static_assert( sizeof( uint64_t ) == 8 );
    read( nEl );
//    std::cout << "nEl = " << nEl << std::endl;

    //    const auto nEl = size / sizeof(T);
    //    uint64_t size = static_cast<uint64_t>( vector.size() * sizeof(T) );
    //    std::cout << "nEl = " << nEl << std::endl;

    //    vector.clear();
    //    vector.reserve( nEl );
    vector.resize( nEl );
    read( (unsigned char*)vector.data(), nEl * sizeof( T ) );

    //    for ( int i = 0; i < nbEl; ++i ) {
    //        //        if constexpr ( getable_v<T> ) { vector.push_back( get<T>() ); }
    //        //        else {
    //        T el;
    //        read( el );
    //        vector.push_back( std::move( el ) );
    //        //        }
    //    }
}

template <class T>
typename std::enable_if_t<!packable_v<T>, void>
// inline void
SerializerImpl::read( std::vector<T>& vector ) {
    //    assert( isOpen() );
    //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
    std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( vector ) << ")" << std::endl;
#endif

    uint64_t nEl;
    static_assert( sizeof( uint64_t ) == 8 );
    read( nEl );
//    std::cout << "nEl = " << nEl << std::endl;

    //    const auto nEl = size / sizeof(T);
    //    uint64_t size = static_cast<uint64_t>( vector.size() * sizeof(T) );
    //    std::cout << "nEl = " << nEl << std::endl;

    //    vector.clear();
    //    vector.reserve( nEl );
    vector.resize( nEl );
    //    read( (unsigned char*)vector.data(), nEl * sizeof( T ) );

    for ( int i = 0; i < nEl; ++i ) {
        //        if constexpr ( getable_v<T> ) { vector.push_back( get<T>() ); }
        //        else {
        T el;
        read( el );
        vector.push_back( std::move( el ) );
        //        }
    }
}

template <class T, class U>
inline void SerializerImpl::read( std::map<T, U>& map ) {
    //    assert( isOpen() );
    //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(std::map)" << std::endl;
    std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( map ) << ")" << std::endl;
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
inline void SerializerImpl::read( std::pair<T, U>& pair ) {
    //    assert( isOpen() );
    //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
    //    std::cout << HEADER_INPUT_MSG "read(std::pair)" << std::endl;
    std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( pair ) << ")" << std::endl;
#endif
    T first;
    read( first );
    U second;
    read( second );
    pair = std::make_pair( first, std::move( second ) );
}

template <std::size_t I, typename... Tp>
inline typename std::enable_if<I == sizeof...( Tp ), void>::type
SerializerImpl::read( std::tuple<Tp...>& t ) {
#ifdef HUB_DEBUG_INPUT
    std::cout << "\t --->" << HEADER << "\033[1mread\033[0m(" << TYPE_NAME( t ) << ") : '" << t << "'"
              << std::endl;
#endif
}

template <std::size_t I, typename... Tp>
    typename std::enable_if <
    I<sizeof...( Tp ), void>::type SerializerImpl::read( std::tuple<Tp...>& t ) {

    if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( t ) << ")" << std::endl;
#endif
    }
    read( std::get<I>( t ) );

    read<I + 1, Tp...>( t );
}

template <std::size_t I, typename... Tp>
inline typename std::enable_if<I == sizeof...( Tp ), void>::type
SerializerImpl::read( std::tuple<Tp...>&& t ) {
#ifdef HUB_DEBUG_INPUT
    std::cout << "\t --->" << HEADER << "\033[1mread\033[0m(" << TYPE_NAME( t ) << ") : '" << t << "'"
              << std::endl;
#endif
}

template <std::size_t I, typename... Tp>
    typename std::enable_if <
    I<sizeof...( Tp ), void>::type SerializerImpl::read( std::tuple<Tp...>&& t ) {

    if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( t ) << ")" << std::endl;
#endif
    }
    read( std::get<I>( t ) );

    read<I + 1, Tp...>( t );
}

} // namespace serializer
} // namespace hub

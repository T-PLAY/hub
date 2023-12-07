#pragma once

// #include <vector>
#include <array>
#include <cstring>
#include <exception>
#include <list>
#include <map>
#include <tuple>
#include <vector>

#include "core/InputBase.hpp"
#include "core/OutputBase.hpp"
#include "core/ios.hpp"

// #include "io/input/InputBase.hpp"
// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"
#include "core/Macros.hpp"
// #include "core/Vector.hpp"
#include "core/ios.hpp"

#include "SerializerI.hpp"

namespace hub {
namespace serializer {

template <typename T>
using serializable_t = decltype( std::declval<T>().serialize( std::declval<ios&>() ) );
//using serializable_t = decltype( T::serialize );
// using serializable_t = decltype( std::declval<T&>().serialize() );

template <typename T, typename = std::void_t<>>
struct serializable : std::false_type {};

template <typename T>
struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

template <typename T>
// static constexpr bool serializable_v = serializable<std::remove_cvref_t<T>>::value;
static constexpr bool serializable_v = serializable<T>::value;


class SerializerImpl : public SerializerI
{
    // static constexpr Size_t BuffSize = 1'000'000;
    static constexpr Size_t BuffSize = MAX_STACK_SIZE;
    bool m_writing                   = false;
    bool m_reading                   = false;

  public:
    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        //        zpp::bits::out out( m_serialBuff );
        //        assert( out.position() == 0 );
        //        out( ts... ).or_throw();
        m_writing  = true;
        m_position = 0;
        writeAll( ts... );

        //                const Size_t size = out.position();
        const Size_t size = m_position;
        assert( size < BuffSize );
        output.write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
        //                output.write( m_serialBuff.data(), size );
        output.write( m_serialBuff.data(), size );
        m_writing = false;
    }

    template <class Input, class... Ts>
    void unpack( Input& input, Ts&... ts ) {
        m_reading = true;
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
        m_reading = false;
    }
    template <class... Ts>
    void operator()( Ts&... ts ) {
        //        assert( isOpen() );
        if ( m_writing ) { writeAll( ts... ); }
        else {
            assert( m_reading );
            readAll( ts... );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////// WRITE
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  private:
    virtual void write( const Data_t* data, Size_t size ) {
#ifdef HUB_DEBUG_OUTPUT
        std::vector<Data_t> vector( data, data + size );
        std::cout << "<---" << HEADER << "write(Data_t*, Size_t) : data = " << vector << std::endl;
#endif

        memcpy( &m_serialBuff[m_position], data, size );
        m_position += size;
    }

    template <class T>
    typename std::enable_if<serializable_v<T>>::type write( const T& t ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(serializable: " << TYPE_NAME( T ) << ") ..."
                  << std::endl;
#endif
        //        assert( isOpen() );
        const_cast<T&>( t ).serialize( *this );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "\033[1mwrite\033[0m(serializable: " << TYPE_NAME( T ) << ") = " << t
                  << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if_t<!serializable_v<T> && packable_v<T>, void> write( const T& t ) {
//        assert(false);
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(" << TYPE_NAME( T ) << ") = " << t << std::endl;
#endif
        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
    }

    template <class T>
    void write( const std::list<T>& list ) {
        //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
        std::cout << HEADER << "write(std::list<" << TYPE_NAME( T ) << ">)" << std::endl;
#endif

        int nbEl = list.size(); // todo
        write( nbEl );

        for ( const T& el : list ) {
            write( el );
        }
    }

    template <class T>
    typename std::enable_if_t<packable_v<T>, void>
    //    void
    write( const std::vector<T>& vector ) {

#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(std::vector<" << TYPE_NAME( T ) << ">)"
                  << std::endl;
#endif

        uint64_t nEl = static_cast<uint64_t>( vector.size() );
        write( nEl );
        write( (unsigned char*)vector.data(), nEl * sizeof( T ) );
    }

    template <class T>
    typename std::enable_if_t<!packable_v<T>, void> write( const std::vector<T>& vector ) {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(std::vector" << TYPE_NAME( T ) << ">)" << std::endl;
#endif
        uint64_t nbEl = static_cast<uint64_t>( vector.size() );
        write( nbEl );

        for ( const T& el : vector ) {
            write( el );
        }
    }

    template <class T, class U>
    void write( const std::map<T, U>& map ) {
        //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(std::map<" << TYPE_NAME( T ) << ", "
                  << TYPE_NAME( U ) << ")" << std::endl;
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
    void write( const std::pair<T, U>& pair ) {
        //    assert( isOpen() );

#ifdef HUB_DEBUG_OUTPUT
        std::cout << "<---" << HEADER << "write(std::pair<" << TYPE_NAME( T ) << ", "
                  << TYPE_NAME( U ) << ">)" << std::endl;
#endif
        const T& first  = pair.first;
        const U& second = pair.second;
        write( first );
        write( second );
    }

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
    write( const std::tuple<Tp...>& t ) {}

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if <
        I<sizeof...( Tp ), void>::type write( const std::tuple<Tp...>& t ) {

        if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_OUTPUT
            std::cout << "<---" << HEADER << "write(std::tuple<" << TYPE_NAME( Tp... ) << ">) : '"
                      << t << "'" << std::endl;
#endif
        }
        //        std::cout << std::get<I>( t ) << std::endl;
        write( std::get<I>( t ) );

        write<I + 1, Tp...>( t );
    }

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type
    write( std::tuple<Tp...>&& t ) {}

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type write( std::tuple<Tp...>&& t ) {

        if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_OUTPUT
            std::cout << "<---" << HEADER << "write(std::tuple<" << TYPE_NAME( Tp... ) << ">) : '"
                      << t << "'" << std::endl;
#endif
        }
        //        std::cout << std::get<I>( t ) << std::endl;
        write( std::get<I>( t ) );

        write<I + 1, Tp...>( t );
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

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////// READ
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void read( Data_t* data, Size_t size ) {
        memcpy( data, &m_serialBuff[m_position], size );
        m_position += size;

#ifdef HUB_DEBUG_INPUT
        std::vector<Data_t> vector( data, data + size );
        std::cout << "\t --->" << HEADER << "read(Data_t*, Size_t) : data = " << vector
                  << ", read size = " << size << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if<serializable_v<T>>::type read( T& t ) {
//        assert( isOpen() );
//        assert( !isEnd() );
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( T ) << ") ..."
                  << std::endl;
#endif
        t.serialize( *this );
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t" << HEADER << "\033[1mread\033[0m(serial: " << TYPE_NAME( T )
                  << ") = " << t << std::endl;
#endif
    }

    template <class T>
    typename std::enable_if_t<!serializable_v<T> && packable_v<T>, void>
    read( T& t ) {
        //        assert( isOpen() );
        //        assert( !isEnd() );
        read( reinterpret_cast<Data_t*>( &t ), sizeof( T ) );
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "read(" << TYPE_NAME( T ) << ") = " << t << std::endl;
#endif
    }

    template <class T>
    void read( std::list<T>& list ) {
        //    assert( isOpen() );
        //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "read(std::list<" << TYPE_NAME( T ) << ">)"
                  << std::endl;
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
    //    void
    read( std::vector<T>& vector ) {
        //    assert( isOpen() );
        //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
        //    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
        std::cout << "\t --->" << HEADER << "read(std::vector<" << TYPE_NAME( T ) << ">)"
                  << std::endl;
#endif
        uint64_t nEl;
        static_assert( sizeof( uint64_t ) == 8 );
        read( nEl );
        vector.resize( nEl );
        read( (unsigned char*)vector.data(), nEl * sizeof( T ) );
    }

    template <class T>
    typename std::enable_if_t<!packable_v<T>, void>
    //    void
    read( std::vector<T>& vector ) {
        //    assert( isOpen() );
        //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
        //    std::cout << HEADER_INPUT_MSG "read(" << TYPE_NAME( vector ) << ")" << std::endl;
        std::cout << "\t --->" << HEADER << "read(std::vector<" << TYPE_NAME( T ) << ">)"
                  << std::endl;
#endif

        uint64_t nEl;
        static_assert( sizeof( uint64_t ) == 8 );
        read( nEl );
        //    std::cout << "nEl = " << nEl << std::endl;

        //    const auto nEl = size / sizeof(T);
        //    uint64_t size = static_cast<uint64_t>( vector.size() * sizeof(T) );
        //    std::cout << "nEl = " << nEl << std::endl;

        vector.clear();
        //    vector.reserve( nEl );
//        vector.resize( nEl );
        //    read( (unsigned char*)vector.data(), nEl * sizeof( T ) );

            for ( int i = 0; i < nEl; ++i ) {
                //        if constexpr ( getable_v<T> ) { vector.push_back( get<T>() ); }
                //        else {
                T el;
                read( el );
                vector.push_back( std::move( el ) );
//                vector
                //        }
            }
#ifdef HUB_DEBUG_INPUT
            std::cout << "\t --->" << HEADER << "read(std::vector<" << TYPE_NAME( T ) << ">) = " << vector
                  << std::endl;
#endif
    }

    ///
    /// \brief read
    /// \param map
    ///
    template <class T, class U>
    void read( std::map<T, U>& map ) {
        //    assert( isOpen() );
        //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
        //    std::cout << HEADER_INPUT_MSG "read(std::map)" << std::endl;
        std::cout << "\t --->" << HEADER << "read(std::map<" << TYPE_NAME( T ) << ", "
                  << TYPE_NAME( U ) << ")" << std::endl;
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

    ///
    /// \brief read
    /// \param pair
    ///
    template <class T, class U>
    void read( std::pair<T, U>& pair ) {
        //    assert( isOpen() );
        //    assert( !isEnd() );

#ifdef HUB_DEBUG_INPUT
        //    std::cout << HEADER_INPUT_MSG "read(std::pair)" << std::endl;
        std::cout << "\t --->" << HEADER << "read(std::pair<" << TYPE_NAME( T ) << ", "
                  << TYPE_NAME( U ) << ")" << std::endl;
#endif
        T first;
        read( first );
        U second;
        read( second );
        pair = std::make_pair( first, std::move( second ) );
    }

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
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t ) {
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "\033[1mread\033[0m(std::tuple<" << TYPE_NAME( Tp... )
                  << ">) : '" << t << "'" << std::endl;
#endif
    }

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...>& t ) {

        if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_INPUT
            std::cout << "\t --->" << HEADER << "read(std::tuple<" << TYPE_NAME( Tp... ) << ">)"
                      << std::endl;
#endif
        }
        read( std::get<I>( t ) );

        read<I + 1, Tp...>( t );
    }

    template <std::size_t I = 0, typename... Tp>
    inline typename std::enable_if<I == sizeof...( Tp ), void>::type read( std::tuple<Tp...>&& t ) {
#ifdef HUB_DEBUG_INPUT
        std::cout << "\t --->" << HEADER << "\033[1mread\033[0m(std::tuple<" << TYPE_NAME( Tp... )
                  << ">) : '" << t << "'" << std::endl;
#endif
    }

    template <std::size_t I = 0, typename... Tp>
        typename std::enable_if < I<sizeof...( Tp ), void>::type read( std::tuple<Tp...>&& t ) {

        if constexpr ( static_cast<int>( I ) == 0 ) {
#ifdef HUB_DEBUG_INPUT
            std::cout << "\t --->" << HEADER << "read(std::tuple<" << TYPE_NAME( Tp... ) << ">)"
                      << std::endl;
#endif
        }
        read( std::get<I>( t ) );

        read<I + 1, Tp...>( t );
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

    //////////////////////////////////////////////////////////////////////////////

  private:
    //    std::vector<Data_t> m_serial;
    std::array<unsigned char, BuffSize> m_serialBuff;
    Size_t m_position = 0;
    //    Result m_result{m_serial.data(), 0};
};

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

} // namespace serializer
} // namespace hub

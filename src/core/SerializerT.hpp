/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2024/01/07

#pragma once

#include "core/InputBase.hpp"
#include "core/Macros.hpp"
#include "core/OutputBase.hpp"
#include "cstring"

namespace hub {

template <class Serializer>
class SRC_API SerializerT
{
  public:
    using Base = Serializer;

    template <typename T>
    using Readable_t = decltype( std::declval<T&>().read( std::declval<SerializerT&>() ) );
    template <typename T, typename = std::void_t<>>
    struct Readable : std::false_type {};
    template <typename T>
    struct Readable<T, std::void_t<Readable_t<T>>> : std::true_type {};
    template <typename T>
    static constexpr bool Readable_v = Readable<T>::value;

    //////////////////////////

    template <typename T>
    using Writable_t = decltype( std::declval<T&>().write( std::declval<SerializerT&>() ) );
    template <typename T, typename = std::void_t<>>
    struct Writable : std::false_type {};
    template <typename T>
    struct Writable<T, std::void_t<Writable_t<T>>> : std::true_type {};
    template <typename T>
    static constexpr bool Writable_v = Writable<T>::value;

    ////////////////////////////////

    template <typename T>
    using has_it_first_t = decltype( std::declval<T&>().begin()->first );
    template <typename T, typename = std::void_t<>>
    struct has_it_first : std::false_type {};
    template <typename T>
    struct has_it_first<T, std::void_t<has_it_first_t<T>>> : std::true_type {};
    template <typename T>
    static constexpr bool has_it_first_v = has_it_first<T>::value;

    ///////////////////

    template <typename T>
    using has_it_second_t = decltype( std::declval<T&>().begin()->second );
    template <typename T, typename = std::void_t<>>
    struct has_it_second : std::false_type {};
    template <typename T>
    struct has_it_second<T, std::void_t<has_it_second_t<T>>> : std::true_type {};
    template <typename T>
    static constexpr bool has_it_second_v = has_it_second<T>::value;

#if defined( COMPILER_GCC )
#    if __GNUC_PREREQ( 12, 3 )
    template <class T>
    static constexpr auto isMap = has_it_first_v<T>&& has_it_second_v<T>;
#    else
    template <class T>
    static constexpr bool isMap = has_it_first_v<T>&& has_it_second_v<T>;
#    endif

#elif defined( COMPILER_CLANG )
#    if CLANG_VERSION >= 14
    template <class T>
    static constexpr auto isMap = has_it_first_v<T>&& has_it_second_v<T>;
#    else
    template <class T>
    static constexpr auto isMap = has_it_first_v<T>&& has_it_second_v<T>;
#    endif

#else
    template <class T>
    static constexpr auto isMap = has_it_first_v<T>&& has_it_second_v<T>;
#endif

    template <class T>
    REQUIRES( static constexpr, !isMap<T>, bool )
    Serializable() {
        return !Readable_v<T> && !Writable_v<T> && !packable_v<T>;
    };

    template <class Map,
              class Pair = std::decay_t<decltype( *std::declval<Map>().begin() )>,
              class T    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).first )>,
              class U    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).second )>>
    REQUIRES( static constexpr, isMap<Map>, bool )
    Serializable() {
        return Serializable<T>() && Serializable<U>();
    };

    template <class... Ts>
    static constexpr auto Serializables = ( Serializable<Ts>() && ... );

    static constexpr Size_t BuffSize = 2'000'000; // 2 Mo

    SerializerT() :
        m_serialBuff( BuffSize ),
        m_packData( m_serialBuff.data() ),
        m_dataWrote( 0 ),
        m_dataReaded( 0 ),
        m_serializer( m_serialBuff ) {}
    SerializerT( const SerializerT& ) :
        m_serialBuff( BuffSize ),
        m_packData( m_serialBuff.data() ),
        m_dataWrote( 0 ),
        m_dataReaded( 0 ),
        m_serializer( m_serialBuff ) {}
    SerializerT( SerializerT&& ) :
        m_serialBuff( BuffSize ),
        m_packData( m_serialBuff.data() ),
        m_dataWrote( 0 ),
        m_dataReaded( 0 ),
        m_serializer( m_serialBuff ) {}

    using ByteView = std::vector<Data_t>;
    ByteView m_serialBuff;

    Data_t* const m_packData;
    Size_t m_dataWrote;
    Size_t m_dataReaded;
    Serializer m_serializer;

  public:
    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        assert( &m_serializer.getBuff() == &m_serialBuff );
        m_serializer.resetOut();
        m_dataWrote = 0;
        assert( m_serializer.outPosition() == 0 );
        writeAll( ts... );

        Size_t m_packSize = m_serializer.outPosition();
        assert( m_packSize == m_dataWrote );
        assert( 0 < m_packSize && m_packSize < BuffSize );
        output.write( reinterpret_cast<const Data_t*>( &m_packSize ), sizeof( Size_t ) );
        output.write( m_packData, m_packSize );
#ifdef HUB_DEBUG_OUTPUT
        std::vector<Data_t> data( m_packData, m_packData + m_packSize );
        DEBUG_MSG( "<---" << HEADER << "packing serial data : " << data )
#endif
    }

    template <class Input, class... Ts>
    void unpack( Input& input, Ts&... ts ) {
        assert( &m_serializer.getBuff() == &m_serialBuff );
        assert( !input.isEnd() );
        Size_t m_packSize;
        input.read( reinterpret_cast<Data_t*>( &m_packSize ), sizeof( Size_t ) );
        assert( 0 < m_packSize && m_packSize < BuffSize );
#ifndef HUB_USE_ZPP_BITS
        m_serialBuff.resize( m_packSize );
#endif
        input.read( m_packData, m_packSize );
#ifdef HUB_DEBUG_INPUT
        std::vector<Data_t> data( m_packData, m_packData + m_packSize );
        DEBUG_MSG( "\t--->" << HEADER << "unpacking serial data : " << data );
#endif

        m_serializer.resetIn();
        m_dataReaded = 0;
        assert( m_serializer.inPosition() == 0 );
        readAll( ts... );

        assert( m_dataReaded == m_serializer.inPosition() );
        assert( m_packSize == m_dataReaded );
    }

    /////////////////////////////////// WRITE ALL ////////////////////////////

    template <class... Ts>
    REQUIRES(, Serializables<Ts...>, void )
    writeAll( const Ts&... ts ) {
        const auto lastPosition = m_serializer.outPosition();

        m_serializer.serialize( ts... );

        const auto newPosition = m_serializer.outPosition();
        m_dataWrote += newPosition - lastPosition;
    }

    template <class T, class... Ts>
    REQUIRES(, (!Serializables<T, Ts...>), void )
    writeAll( const T& t, const Ts&... ts ) {
        write( t );
        if constexpr ( sizeof...( Ts ) > 0 ) { writeAll( ts... ); }
    }

    /////////////////////////////////// READ ALL ////////////////////////////////

    template <class... Ts>
    REQUIRES(, Serializables<Ts...>, void )
    readAll( Ts&... ts ) {
        const auto lastPosition = m_serializer.inPosition();

        m_serializer.deserialize( ts... );

        const auto newPosition = m_serializer.inPosition();
        m_dataReaded += newPosition - lastPosition;
    }

    template <class T, class... Ts>
    REQUIRES(, (!Serializables<T, Ts...>), void )
    readAll( T& t, Ts&... ts ) {
        read( t );
        if constexpr ( sizeof...( Ts ) > 0 ) { readAll( ts... ); }
    }

    /////////////////////////////////// TEMPLATE ////////////////////////////////////

//    template <class T>
//    REQUIRES(, !Serializable<T>() && packable_v<T>, void )
//    write( const T& t ) {
//        // DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t )
//        const auto lastPosition = m_serializer.outPosition();

//        m_serializer.serialize( t );

//        const auto newPosition = m_serializer.outPosition();
//        m_dataWrote += newPosition - lastPosition;

//#ifdef HUB_DEBUG_OUTPUT
//        // DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t )
//        const std::vector<Data_t> data( m_serialBuff.data() + lastPosition,
//                                        m_serialBuff.data() + newPosition );
//        DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t ) << ") = "
//                          << t << " (" << lastPosition << "->" << newPosition << ")" << data );
//#endif
//    }

    template <class T>
//    REQUIRES(, !Serializable<T>() && !packable_v<T>, void )
    REQUIRES(, Writable_v<T>, void )
//    typename std::enable_if_t<!Serializable<T>() && !packable_v<T>, void>
    write( const T& t ) {
        static_assert( Writable_v<T> );
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t )
                          << ") ..." );
#endif
        t.write( *this );
#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t )
                          << ") = " << t );
#endif
    }

    template <class T>
    REQUIRES(, ! Writable_v<T>, void )
    write( const T& t ) {

        const auto lastPosition = m_serializer.outPosition();

        m_serializer.serialize( t );

        const auto newPosition = m_serializer.outPosition();
        m_dataWrote += newPosition - lastPosition;

#ifdef HUB_DEBUG_OUTPUT
        const std::vector<Data_t> data( m_serialBuff.data() + lastPosition,
                                        m_serialBuff.data() + newPosition );
        DEBUG_MSG( "<---" << HEADER << "\033[1;36mwrite\033[0m(serial: " << TYPE_NAME( t ) << ") = "
                          << t << " (" << lastPosition << "->" << newPosition << ")" << data );
#endif
    }

    //////////////////////////////////

//    template <class T>
//    REQUIRES(, !Serializable<T>() && packable_v<T>, void )
//    read( T& t ) {
//#ifdef HUB_DEBUG_INPUT
//        // DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t )
//        auto serialCopy = m_serialBuff;
//#endif
//        const auto lastPosition = m_serializer.inPosition();

//        m_serializer.deserialize( t );

//        const auto newPosition = m_serializer.inPosition();
//        m_dataReaded += newPosition - lastPosition;
//#ifdef HUB_DEBUG_INPUT
//        const std::vector<Data_t> data( serialCopy.data(),
//                                        serialCopy.data() + newPosition - lastPosition );
//        DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t ) << ") = "
//                            << t << " (" << lastPosition << "->" << newPosition << ")" << data );
//        // DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t )
//#endif
//    }

    template <class T>
//    REQUIRES(, (!Serializable<T>() && !packable_v<T>), void )
    REQUIRES(, Readable_v<T>, void )
    read( T& t ) {
        static_assert( Readable_v<T> );
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( t )
                            << ") ..." );
#endif
        t.read( *this );
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( t )
                            << ") = " << t );
#endif
    }

    template <class T>
    REQUIRES(, ! Readable_v<T>, void )
    read( T& t ) {
#ifdef HUB_DEBUG_INPUT
        // DEBUG_MSG( "\t--->" << HEADER << "\033[1;36mread\033[0m(serial: " << TYPE_NAME( t )
        auto serialCopy = m_serialBuff;
#endif
        const auto lastPosition = m_serializer.inPosition();

        m_serializer.deserialize( t );

        const auto newPosition = m_serializer.inPosition();
        m_dataReaded += newPosition - lastPosition;

#ifdef HUB_DEBUG_INPUT
        const std::vector<Data_t> data( serialCopy.data(),
                                        serialCopy.data() + newPosition - lastPosition );
        DEBUG_MSG( "\t--->" << HEADER << "\033[1;36mread\033[0m(serial: " << TYPE_NAME( t )
                            << ") = " << t << " (" << lastPosition << "->" << newPosition << ")"
                            << data );
#endif
    }

    /////////////////////////////////// CHAR PTR //////////////////////////////////////////////////

    void write( char* str ) = delete; // non compatible format 32/64 bit

    void write( const char* str ) {
        assert( str != nullptr );

#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( "<---" << HEADER << "write(const char*)" );
#endif
        write( std::string( str ) );
    }

#ifdef ARCH_X86
    void read( size_t size ) = delete; // non compatible format 32/64 bit
#else
    void read( uint32_t size )  = delete; // non compatible format 32/64 bit
#endif

    void read( char* str ) {
        assert( str != nullptr );

#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t--->" << HEADER << "read(char *)" );
        ;
#endif

        std::string string;
        read( string );
        memcpy( str, string.data(), string.size() );
        str[string.size()] = 0;
    }

    /////////////////////////////////// PAIR ///////////////////////

    template <class T, class U>
    REQUIRES(, (!Serializables<T, U>), void )
    write( const std::pair<T, U>& pair ) {

#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( "<---" << HEADER << "write(std::pair<" << TYPE_NAME( T() ) << ", "
                          << TYPE_NAME( U() ) << ">) = " << pair );
#endif
        const T& first  = pair.first;
        const U& second = pair.second;
        write( first );
        write( second );
    }

    template <class T, class U>
    REQUIRES(, (!Serializables<T, U>), void )
    read( std::pair<T, U>& pair ) {

        T first;
        read( first );
        U second;
        read( second );
        pair = std::make_pair( std::move( first ), std::move( second ) );
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t--->" << HEADER << "read(std::pair<" << TYPE_NAME( T() ) << ", "
                            << TYPE_NAME( U() ) << ">) = " << pair );
#endif
    }

    /////////////////////////////////// MAP /////////////////////////////

    template <class T, class U>
    REQUIRES(, (!Serializables<T, U>), void )
    write( const std::map<T, U>& map ) {

#ifdef HUB_DEBUG_OUTPUT
        DEBUG_MSG( "<---" << HEADER << "write(std::map<" << TYPE_NAME( T() ) << ", "
                          << TYPE_NAME( U() ) << ") = " << map );
#endif

        uint64_t nbKey = static_cast<uint64_t>( map.size() );
        write( nbKey );

        for ( const std::pair<T, U>& pair : map ) {
            write( pair );
        }
    }

    template <class T, class U>
    REQUIRES(, (!Serializables<T, U>), void )
    read( std::map<T, U>& map ) {

        uint64_t nbEl;
        read( nbEl );
        map.clear();

        for ( int i = 0; i < nbEl; ++i ) {
            std::pair<T, U> pair;
            read( pair );
            assert( map.find( pair.first ) == map.end() );
            map.emplace( std::move( pair ) );
        }
#ifdef HUB_DEBUG_INPUT
        DEBUG_MSG( "\t--->" << HEADER << "read(std::map<" << TYPE_NAME( T() ) << ", "
                            << TYPE_NAME( U() ) << ") = " << map );
#endif
    }
};

} // namespace hub

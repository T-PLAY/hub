#pragma once

#include "core/InputBase.hpp"
#include "core/Macros.hpp"
#include "core/OutputBase.hpp"
#include "cstring"

namespace hub {

template <class Serializer>
class SerializerT
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

    template <class T>
    static constexpr auto isMap = has_it_first_v<T> && has_it_second_v<T>;

// template <class T>
// concept isMap = requires( T t ) {
//     t.begin()->first;
//     t.begin()->second;
// };

    template <class T>
    // requires( !isMap<T> )
    // static constexpr auto Serializable() {
    REQUIRES(static constexpr, ! isMap<T>, bool)
    Serializable() {
        return !Readable_v<T> && !Writable_v<T> && !packable_v<T>;
    };

    template <class Map,
              class Pair = std::decay_t<decltype( *std::declval<Map>().begin() )>,
              class T    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).first )>,
              class U    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).second )>>
        // requires( isMap<Map> )
    // static constexpr auto Serializable() {
    REQUIRES(static constexpr, isMap<Map>, bool)
    Serializable() {
        return Serializable<T>() && Serializable<U>();
    };

    template <class... Ts>
    static constexpr auto Serializables = ( Serializable<Ts>() && ... );

    static constexpr Size_t BuffSize = 2'000'000; // 2 Mo

  private:
    using ByteView        = std::vector<Data_t>;
    ByteView m_serialBuff = std::vector<Data_t>( BuffSize );

    Data_t* const m_packData = m_serialBuff.data();
    Size_t m_dataWrote       = 0;
    Size_t m_dataReaded      = 0;
    Serializer m_serializer { m_serialBuff };

  public:
    template <class Output, class... Ts>
    void pack( Output& output, const Ts&... ts ) {
        // m_out.reset( 0 );
        m_serializer.resetOut();
        // assert(m_out.processed_data().size() == 0);
        // assert(m_out.remaining_data().size() == BuffSize);
        m_dataWrote = 0;
        // assert( m_out.position() == 0 );
        assert( m_serializer.outPosition() == 0 );
        writeAll( ts... );

        // m_packSize = m_out.position();
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

        // m_in.reset( 0 );
        m_serializer.resetIn();
        // assert(m_in.processed_data().size() == 0);
        // assert(m_in.remaining_data().size() == BuffSize);
        m_dataReaded = 0;
        // assert( m_in.position() == 0 );
        assert( m_serializer.inPosition() == 0 );
        readAll( ts... );

        // assert( m_dataReaded == m_in.position() );
        assert( m_dataReaded == m_serializer.inPosition() );
        // assert( m_packSize == m_in.position() );
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

    /////////////////////////////////// PACKABLE ////////////////////////////////////

    template <class T>
    REQUIRES(, !Serializable<T>() && packable_v<T>, void )
    write( const T& t ) {
// #ifdef HUB_DEBUG_OUTPUT
        // DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t )
                          // << ") ..." );
// #endif
        const auto lastPosition = m_serializer.outPosition();

        // memcpy( m_packData + lastPosition, reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
        m_serializer.serialize(t);

        // m_serializer.setOutPosition( lastPosition + sizeof( T ) );
        const auto newPosition = m_serializer.outPosition();
        m_dataWrote += newPosition - lastPosition;

#ifdef HUB_DEBUG_OUTPUT
        // DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t )
        // << ") = " << t );
        const std::vector<Data_t> data( m_serialBuff.data() + lastPosition,
                                        m_serialBuff.data() + newPosition );
        DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(packable: " << TYPE_NAME( t ) << ") = "
                          << t << " (" << lastPosition << "->" << newPosition << ")" << data );
#endif
    }

    template <class T>
    REQUIRES(, !Serializable<T>() && packable_v<T>, void )
    read( T& t ) {
#ifdef HUB_DEBUG_INPUT
        // DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t )
                            // << ") ..." );
        auto serialCopy = m_serialBuff;
#endif
        const auto lastPosition = m_serializer.inPosition();

        m_serializer.deserialize(t);
        // memcpy( reinterpret_cast<Data_t*>( &t ), m_packData + lastPosition, sizeof( T ) );
        // memcpy( reinterpret_cast<Data_t*>( &t ), m_packData + lastPosition, sizeof( T ) );

        // m_serializer.setInPosition( lastPosition + sizeof( T ) );
        const auto newPosition = m_serializer.inPosition();
        m_dataReaded += newPosition - lastPosition;
#ifdef HUB_DEBUG_INPUT
        const std::vector<Data_t> data( serialCopy.data(),
                                        serialCopy.data() + newPosition - lastPosition );
        DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t ) << ") = "
                          << t << " (" << lastPosition << "->" << newPosition << ")" << data );
        // DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(packable: " << TYPE_NAME( t )
                            // << ") = " << t );
#endif
    }

    /////////////////////////////////// WRITABLE/READABLE ////////////////////////////////////

    template <class T>
    REQUIRES(, !Serializable<T>() && !packable_v<T>, void )
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
    REQUIRES(, (!Serializable<T>() && !packable_v<T>), void )
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

    //////////////////////////////////////// SERIALIZABLE //////////////////

    template <class T>
    REQUIRES(, Serializable<T>(), void )
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

    template <class T>
    REQUIRES(, Serializable<T>(), void )
    read( T& t ) {
#ifdef HUB_DEBUG_INPUT
        // DEBUG_MSG( "\t--->" << HEADER << "\033[1;36mread\033[0m(serial: " << TYPE_NAME( t )
                            // << ") ..." );
        auto serialCopy = m_serialBuff;
#endif
        const auto lastPosition = m_serializer.inPosition();

        m_serializer.deserialize( t );

        const auto newPosition = m_serializer.inPosition();
        m_dataReaded += newPosition - lastPosition;

#ifdef HUB_DEBUG_INPUT
        const std::vector<Data_t> data( serialCopy.data(), serialCopy.data() + newPosition - lastPosition );
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

        uint32_t nbKey = static_cast<uint32_t>( map.size() );
        write( nbKey );

        for ( const std::pair<T, U>& pair : map ) {
            write( pair );
        }
    }

    template <class T, class U>
    REQUIRES(, (!Serializables<T, U>), void )
    read( std::map<T, U>& map ) {

        uint32_t nbEl;
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

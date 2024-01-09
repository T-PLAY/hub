/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/31
	
#pragma once

#ifdef HUB_USE_ZPP_BITS

#    include <array>
// #include <concepts>

#    include "zpp_bits.h"

// #include "io/input/InputBase.hpp"

// #include "io/input/InputImpl.hpp"
// #include "io/input/InputZppBits.hpp"

#    include "SerializerI.hpp"
#    include "core/InputBase.hpp"
#    include "core/OutputBase.hpp"

namespace hub {
namespace serializer {

class SerializerZppBits : public SerializerI
{
  public:
    SerializerZppBits( ByteView& byteView ) : SerializerI( byteView ) {
        // m_buff.clear();
        m_in.reset( 0 );
        m_out.reset( 0 );
    }

    void resetIn() override {
        assert( !m_buff.empty() );
        m_in.reset( 0 );
    }
    void resetOut() override {
        m_buff.clear();
        m_out.reset( 0 );
    }
    Size_t inPosition() const override { return m_in.position(); }
    Size_t outPosition() const override { return m_out.position(); }
    void setInPosition( Size_t inPosition ) override { m_in.reset( inPosition ); }
    void setOutPosition( Size_t outPosition ) override { m_out.position() = outPosition; }

    // template <class T>
    // void serialize( const T& t ) {
    //     const auto lastBuffSize = m_buff.size();

    //     m_out( t );

    //     const auto curBuffSize = m_buff.size();
    //     assert( curBuffSize > lastBuffSize );
    //     m_outPosition += curBuffSize - lastBuffSize;
    // }

    template <class... Ts>
    void serialize( const Ts&... ts ) {
        // const auto lastBuffSize = m_buff.size();

        m_out( ts... );

        // const auto curBuffSize = m_buff.size();
        // assert( curBuffSize > lastBuffSize );
        // m_outPosition += curBuffSize - lastBuffSize;
    }

    // template <class T>
    // void deserialize( T& t ) {
    //     const auto lastBuffSize = m_buff.size();

    //     m_in( t );

    //     const auto curBuffSize = m_buff.size();
    //     assert( curBuffSize < lastBuffSize );
    //     m_inPosition += lastBuffSize - curBuffSize;
    // }

    template <class... Ts>
    void deserialize( Ts&... ts ) {
        // const auto lastBuffSize = m_buff.size();

        m_in( ts... );

        // const auto curBuffSize = m_buff.size();
        // assert( curBuffSize < lastBuffSize );
        // m_inPosition += lastBuffSize - curBuffSize;
    }

  private:
    // zpp::serializer::memory_output_archive m_out { m_buff };
    zpp::bits::out<ByteView> m_out { m_buff };
    // zpp::serializer::memory_input_archive m_in { m_buff };
    zpp::bits::in<ByteView> m_in { m_buff };

    // Size_t m_inPosition = 0;
    // Size_t m_outPosition = 0;
};

} // namespace serializer
} // namespace hub

// // template <class Out, class... Ts>
// // concept Outable = requires(Out & out, Ts&... ts) {out(ts...); };

// // template <class In, class... Ts>
// // concept Inable = requires(In & in, Ts&... ts) {in(ts...); };

// template <class T>
// concept isMap = requires( T t ) {
//     t.begin()->first;
//     t.begin()->second;
// };

// class SerializerZppBits : public SerializerI
// {
//   public:
//     template <typename T>
//     using Readable_t = decltype( std::declval<T&>().read( std::declval<SerializerZppBits&>() ) );
//     template <typename T, typename = std::void_t<>>
//     struct Readable : std::false_type {};
//     template <typename T>
//     struct Readable<T, std::void_t<Readable_t<T>>> : std::true_type {};
//     template <typename T>
//     static constexpr bool Readable_v = Readable<T>::value;

//     //////////////////////////

//     template <typename T>
//     using Writable_t = decltype( std::declval<T&>().write( std::declval<SerializerZppBits&>() )
//     ); template <typename T, typename = std::void_t<>> struct Writable : std::false_type {};
//     template <typename T>
//     struct Writable<T, std::void_t<Writable_t<T>>> : std::true_type {};
//     template <typename T>
//     static constexpr bool Writable_v = Writable<T>::value;

//     ////////////////////////////////

//     template <class T>
//         requires( !isMap<T> )
//     static constexpr auto Serializable() {
//         return !Readable_v<T> && !Writable_v<T> && !packable_v<T>;
//     };

//     template <class Map,
//               class Pair = std::decay_t<decltype( *std::declval<Map>().begin() )>,
//               class T    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).first )>,
//               class U    = std::decay_t<decltype( ( *std::declval<Map>().begin() ).second )>>
//         requires( isMap<Map> )
//     static constexpr auto Serializable() {
//         return Serializable<T>() && Serializable<U>();
//     };

//     template <class... Ts>
//     static constexpr auto Serializables = ( Serializable<Ts>() && ... );

//     static constexpr Size_t BuffSize = 1'000'000; // 1 Mo

//   private:
//     //    using ByteView = std::array<Data_t, BuffSize>;
//     using ByteView        = std::vector<Data_t>;
//     ByteView m_serialBuff = std::vector<Data_t>( BuffSize );

//     Size_t m_packSize        = 0;
//     Data_t* const m_packData = m_serialBuff.data();

//     zpp::bits::out<ByteView> m_out { m_serialBuff };
//     Size_t m_dataWrote  = 0;
//     Size_t m_dataReaded = 0;
//     zpp::bits::in<ByteView> m_in { m_serialBuff };
//     // zpp::bits::out<ByteView> m_out;
//     // zpp::bits::in<ByteView> m_in;

//   public:
//     // SerializerZppBits() = default;

//     // template <class Container>
//     // SerializerZppBits(Container & container) {
//     //     std::copy(container.begin(), container.end(), m_serialBuff.begin());
//     // }

//     template <class Output, class... Ts>
//     void pack( Output& output, const Ts&... ts ) {
//         // m_serialBuff.reserve( BuffSize );
//         // m_serialBuff.resize( BuffSize );
//         //       zpp::bits::out m_out( m_serialBuff );
//         // m_out = zpp::bits::out(m_serialBuff);
//         m_out.reset( 0 );
//         assert(m_out.processed_data().size() == 0);
//         assert(m_out.remaining_data().size() == BuffSize);
//         // m_out.enlarge_for()
//         m_dataWrote = 0;
//         //        m_in.reset(0);
//         assert( m_out.position() == 0 );
//         //                m_out( ts... ).or_throw();
//         writeAll( ts... );

//         m_packSize = m_out.position();
//         assert( m_packSize == m_dataWrote );
//         assert( 0 < m_packSize && m_packSize < BuffSize );
//         output.write( reinterpret_cast<const Data_t*>( &m_packSize ), sizeof( Size_t ) );
//         output.write( m_packData, m_packSize );
// #    ifdef HUB_DEBUG_OUTPUT
//         // std::cout << "<---" << HEADER << "packing " << size << " bytes" << std::endl;
//         std::vector<Data_t> data( m_packData, m_packData + m_packSize );
//         DEBUG_MSG( "<---" << HEADER << "packing serial data : " << data )
// #    endif
//     }

//     template <class Input, class... Ts>
//     void unpack( Input& input, Ts&... ts ) {
//         // m_serialBuff.reserve( BuffSize );
//         // m_serialBuff.resize( BuffSize );
//         // Size_t size;
//         assert( !input.isEnd() );
//         input.read( reinterpret_cast<Data_t*>( &m_packSize ), sizeof( Size_t ) );
//         assert( 0 < m_packSize && m_packSize < BuffSize );
//         input.read( m_packData, m_packSize );
// #    ifdef HUB_DEBUG_INPUT
//         // std::cout << "\t--->" << HEADER << "unpacking " << size << " bytes" << std::endl;
//         std::vector<Data_t> data( m_packData, m_packData + m_packSize );
//         DEBUG_MSG ("\t--->" << HEADER << "unpacking serial data : " << data);
// #    endif

//         // zpp::bits::in m_in( m_serialBuff );
//         //        m_in = zpp::bits::in(m_serialBuff);
//         m_in.reset( 0 );
//         assert(m_in.processed_data().size() == 0);
//         assert(m_in.remaining_data().size() == BuffSize);
//         m_dataReaded = 0;
//         //        m_in( ts... ).or_throw();
//         assert( m_in.position() == 0 );
//         // m_in.reset( 0 );
//         readAll( ts... );

//         // auto inPosition = m_in.position();
//         assert( m_dataReaded == m_in.position() );
//         assert( m_packSize == m_in.position() );
//     }

//     /////////////////////////////////// WRITE ALL ////////////////////////////

//     template <class... Ts>
//         requires( Serializables<Ts...> )
//     void writeAll( const Ts&... ts ) {
//         const auto position = m_out.position();
//         try {
//             m_out( ts... ).or_throw();
//         }
//         catch ( std::exception& ex ) {
//             std::cout << "[Serializer] catch exception : " << ex.what() << std::endl;
//             assert( false );
//         }
//         const auto newPosition = m_out.position();
//         m_dataWrote += newPosition - position;
//     }

//     template <class T, class... Ts>
//         requires( !Serializables<T, Ts...> )
//     void writeAll( const T& t, const Ts&... ts ) {
//         write( t );
//         if constexpr ( sizeof...( Ts ) > 0 ) { writeAll( ts... ); }
//     }

//     /////////////////////////////////// READ ALL ////////////////////////////////

//     template <class... Ts>
//         requires( Serializables<Ts...> )
//     void readAll( Ts&... ts ) {
//         const auto position = m_in.position();
//         try {
//             m_in( ts... ).or_throw();
//         }
//         catch ( std::exception& ex ) {
//             std::cout << "[Serializer] catch exception : " << ex.what() << std::endl;
//             assert( false );
//         }
//         const auto newPosition = m_in.position();
//         m_dataReaded += newPosition - position;
//     }

//     template <class T, class... Ts>
//         requires( !Serializables<T, Ts...> )
//     void readAll( T& t, Ts&... ts ) {
//         read( t );
//         if constexpr ( sizeof...( Ts ) > 0 ) { readAll( ts... ); }
//     }

//     //////////////////////////////////////// WRITE T //////////////////

//     template <class T>
//         requires( Serializable<T>() )
//     void write( const T& t ) {

//         const auto lastPosition = m_out.position();

//         try {
//             m_out( t ).or_throw();
//         }
//         catch ( std::exception& ex ) {
//             std::cout << "[Serializer] catch exception : " << ex.what() << std::endl;
//             assert( false );
//         }

//         const auto newPosition = m_out.position();
//         m_dataWrote += newPosition - lastPosition;

// #    ifdef HUB_DEBUG_OUTPUT
//         // const auto span = m_out.processed_data();
//         // const std::vector<Data_t> data( span.begin() + lastPosition, span.end() );
//         const std::vector<Data_t> data( m_serialBuff.data() + lastPosition,
//                                         m_serialBuff.data() + newPosition );
//         DEBUG_MSG( "<---" << HEADER << "\033[1;36mwrite\033[0m(const " << TYPE_NAME( T )
//                           << "&) = " << t << " (" << lastPosition << "->" << newPosition << ")"
//                           << data);

// #    endif
//     }

//     template <class T>
//         requires( !Serializable<T>() && packable_v<T> )
//     void write( const T& t ) {
// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( T )
//                           << ") ..." );
// #    endif

//         const auto lastPosition = m_out.position();

//         memcpy( m_packData + lastPosition, reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//         m_out.position() = lastPosition + sizeof( T );

//         const auto newPosition = m_out.position();
//         m_dataWrote += newPosition - lastPosition;

// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( T )
//                           << ") = " << t );
// #    endif
//     }

//     template <class T>
//         requires( !Serializable<T>() && !packable_v<T> )
//     void write( const T& t ) {
//         static_assert( Writable_v<T> );
// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( T )
//                           << ") ..." );
// #    endif

//         t.write( *this );

// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( T )
//                           << ") = " << t );
// #    endif
//     }

//     /////////////////////////////////// READ T ////////////////////////////////////

//     template <class T>
//         requires( Serializable<T>() )
//     void read( T& t ) {
// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "\033[1;36mread\033[0m(" << TYPE_NAME( T ) << "&) ..."
//                      );
// #    endif

//         const auto lastPosition = m_in.position();

//         try {
//             m_in( t ).or_throw();
//         }
//         catch ( std::exception& ex ) {
//             std::cout << "[Serializer] catch exception : " << ex.what() << std::endl;
//             assert( false );
//         }

//         const auto newPosition = m_in.position();
//         m_dataReaded += newPosition - lastPosition;

// #    ifdef HUB_DEBUG_INPUT
//         // const auto span = m_in.processed_data();
//         const std::vector<Data_t> data( m_packData + lastPosition, m_packData + newPosition );
//         DEBUG_MSG(  "\t--->" << HEADER << "\033[1;36mread\033[0m(" << TYPE_NAME( T )
//                   << "&) = " << t << " (" << lastPosition << "->" << newPosition << ")" << data
//                    );
// #    endif
//     }

//     template <class T>
//         requires( !Serializable<T>() && packable_v<T> )
//     void read( T& t ) {
// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T )
//                             << ") ..." );
// #    endif

//         const auto lastPosition = m_in.position();

//         memcpy( reinterpret_cast<Data_t*>( &t ), m_packData + lastPosition, sizeof( T ) );
//         m_in.position() = lastPosition + sizeof( T );

//         const auto newPosition = m_in.position();
//         m_dataReaded += newPosition - lastPosition;

// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T )
//                             << ") = " << t );
// #    endif
//     }

//     template <class T>
//         requires( !Serializable<T>() && !packable_v<T> )
//     void read( T& t ) {
//         static_assert( Readable_v<T> );
// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG(  "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T )
//                             << ") ..." );
// #    endif

//         t.read( *this );

// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "\033[1mread\033[0m(readable: " << TYPE_NAME( T )
//                             << ") = " << t );
// #    endif
//     }

//     /////////////////////////////////// CHAR PTR
//     //////////////////////////////////////////////////

//     void write( char* str ) = delete; // non compatible format 32/64 bit

//     void write( const char* str ) {
//         assert( str != nullptr );

// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG ("<---" << HEADER << "write(const char*)" );
// #    endif
//         write( std::string( str ) );
//     }

// #    ifdef ARCH_X86
//     void read( size_t size ) = delete; // non compatible format 32/64 bit
// #    endif

//     void read( char* str ) {
//         assert( str != nullptr );

// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "read(char *)" );;
// #    endif

//         std::string string;
//         read( string );
//         memcpy( str, string.data(), string.size() );
//         str[string.size()] = 0;
//     }

//     /////////////////////////////////// PAIR ///////////////////////

//     template <class T, class U>
//     //            requires (writable_v<T> || writable_v<U>)
//         requires( !Serializables<T, U> )
//     void write( const std::pair<T, U>& pair ) {

// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "write(std::pair<" << TYPE_NAME( T ) << ", "
//                           << TYPE_NAME( U ) << ">) = " << pair );
// #    endif
//         const T& first  = pair.first;
//         const U& second = pair.second;
//         write( first );
//         write( second );
//     }

//     template <class T, class U>
//         requires( !Serializables<T, U> )
//     void read( std::pair<T, U>& pair ) {

//         T first;
//         read( first );
//         U second;
//         read( second );
//         pair = std::make_pair( std::move( first ), std::move( second ) );
//         //        pair = std::make_pair( first, second );
// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG( "\t--->" << HEADER << "read(std::pair<" << TYPE_NAME( T ) << ", "
//                             << TYPE_NAME( U ) << ">) = " << pair );
// #    endif
//     }

//     /////////////////////////////////// MAP /////////////////////////////

//     template <class T, class U>
//     //            requires (writable_v<T> || writable_v<U>)
//         requires( !Serializables<T, U> )
//     void write( const std::map<T, U>& map ) {

// #    ifdef HUB_DEBUG_OUTPUT
//         DEBUG_MSG( "<---" << HEADER << "write(std::map<" << TYPE_NAME( T ) << ", "
//                           << TYPE_NAME( U ) << ") = " << map );
// #    endif

//         uint32_t nbKey = static_cast<uint32_t>( map.size() );
//         write( nbKey );

//         for ( const std::pair<T, U>& pair : map ) {
//             write( pair );
//         }
//     }

//     template <class T, class U>
//         requires( !Serializables<T, U> )
//     void read( std::map<T, U>& map ) {

//         uint32_t nbEl;
//         read( nbEl );
//         map.clear();

//         for ( int i = 0; i < nbEl; ++i ) {
//             std::pair<T, U> pair;
//             read( pair );
//             assert( map.find( pair.first ) == map.end() );
//             map.emplace( std::move( pair ) );
//         }
// #    ifdef HUB_DEBUG_INPUT
//         DEBUG_MSG(  "\t--->" << HEADER << "read(std::map<" << TYPE_NAME( T ) << ", "
//                             << TYPE_NAME( U ) << ") = " << map );
// #    endif
//     }
// };

#endif

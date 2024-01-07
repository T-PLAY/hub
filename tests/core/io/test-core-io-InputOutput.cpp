
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

// #if CPP_VERSION <= 14
// #    include "core/traits/std_any.hpp"
// #else
// #    include <any>
// #endif
// #include <core/Any.hpp>

// #include <any>
#include <cstring>
#include <iostream>
#include <set>

#include "test_common.hpp"
#include "test_core_io_common.hpp"
// #include "core/test_core_common.hpp"
// #include "core/io/test_core_io_common.hpp"

#include <core/Macros.hpp>
#include <core/Traits.hpp>
#include <core/io/Archive.hpp>
#include <core/io/InputOutput.hpp>

#include <core/serializer/SerializerZpp.hpp>
// #include <core/Serializer.hpp>

// auto serialize(const Node &) -> zpp::bits::pb_protocol;

using namespace testCoreIoCommon;

template <class Archive>
void process( Archive& archive ) {
    assert( archive.isEnd() );

    // using NodeType = TestNonPackable::Node;
    // NodeType node { 1 };
    // std::vector<NodeType> nodes { node };

#ifndef HUB_DEBUG_OUTPUT
    for ( int i = 0; i < 1'000; ++i ) {
#endif

        checkType<int>( archive, 5 );
        checkType<double>( archive, 2.0 );
        checkType<std::string>( archive, "hello" );
        checkType<UserEnum2>( archive, UserEnum2::B );
        checkType<SerializedClass>( archive, SerializedClass { 2, true } );
        checkType<WritableReadableData>( archive, WritableReadableData { "gauthier", 30 } );

#ifndef HUB_DEBUG_OUTPUT
    }
#endif
#ifdef DEBUG
    std::cout << "nCall : " << archive.getNCall() << std::endl;
#endif
}

TEST_CASE( "InputOutput test" ) {
    TEST_BEGIN()

    long long durationArchiveZppBits = 0;
    long long durationArchiveImpl    = 0;
    long long durationArchiveZpp     = 0;

    //    hub::io::Archive<hub::io::InputOutput> archive;
//    hub::io::Archive archive;
#ifdef HUB_USE_ZPP_BITS
    {
        std::cout << "-------------------- ArchiveZppBits --------" << std::endl;
        hub::io::ArchiveT<hub::serializer::SerializerZppBits> archiveZppBits;
        const auto startClock = std::chrono::high_resolution_clock::now();
        process( archiveZppBits );
        const auto endClock = std::chrono::high_resolution_clock::now();
        durationArchiveZppBits =
            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
        std::cout << "duration: " << durationArchiveZppBits / 1'000'000.0 << " ms" << std::endl;
    }
#endif

    {
        std::cout << "-------------------- ArchiveZpp --------" << std::endl;
        hub::io::ArchiveT<hub::SerializerT<hub::serializer::SerializerZpp>> archiveZpp;
        const auto startClock = std::chrono::high_resolution_clock::now();
        process( archiveZpp );
        const auto endClock = std::chrono::high_resolution_clock::now();
        durationArchiveZpp =
            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
        std::cout << "duration: " << durationArchiveZpp / 1'000'000.0 << " ms" << std::endl;
    }

    //    {
    //        std::cout << "-------------------- ArchiveImpl --------" << std::endl;
    //        hub::io::ArchiveT<hub::serializer::SerializerImpl> archiveImpl;
    //        const auto startClock = std::chrono::high_resolution_clock::now();
    //        process( archiveImpl );
    //        const auto endClock = std::chrono::high_resolution_clock::now();
    //        durationArchiveImpl =
    //            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock
    //            ).count();
    //        std::cout << "duration: " << durationArchiveImpl / 1'000'000.0 << " ms" << std::endl;
    //    }

    // #ifndef DEBUG
    //     CHECK( durationArchiveZppBits < durationArchiveImpl );
    // #endif
    // #ifdef HUB_USE_ZPP_BITS

    // #if !defined( DEBUG ) && defined( HUB_USE_ZPP_BITS )
    //     const auto ratio = durationArchiveImpl / (double)durationArchiveZppBits;
    //     CHECK_VALUE( ratio, 1.0, 0.5, "ArchiveZppBits/ArchiveImpl", "/" );

    //    static_assert(
    //        std::is_same_v<hub::io::Archive,
    //        hub::io::ArchiveT<hub::serializer::SerializerZppBits>> );
    // #endif

    TEST_END()
}

// class TestNonPackable
// {
//   public:
//     class Node
//     {
//       public:
//         using Dims     = std::vector<int>;

//         bool operator==( const Node& other ) const { return m_a == other.m_a; }

//         auto toString() const { return std::to_string( m_a ); }

//         Node() = default;
//         Node( int a ) : m_a { a } {};

// //        template <class Serial>
// //        void serialize( Serial& serial ) {
// //            serial( m_a );
// //        }
// #if CPP_VERSION >= 20
//         static constexpr auto serialize( auto& archive, auto& self ) { return archive( self.m_a
//         ); }
// #endif

//       private:
//         int m_a;
//     };
//     static_assert( !hub::notWritable_v<Node> );
//     static_assert( !hub::writable_v<Node> );
//     static_assert( !hub::packable_v<Node> );
// //    static_assert( hub::serializable_v<Node> );

//     TestNonPackable() = default;
//     TestNonPackable( double a,
//                      int b,
//                      std::string name,
//                      std::vector<int> vints,
//                      std::vector<Node> nodes ) :
//         m_a { a }, m_b { b }, m_name { name }, m_vints { vints }, m_nodes { nodes } {}

//     bool operator==( const TestNonPackable& test ) const {
//         return m_a == test.m_a && m_b == test.m_b && m_name == test.m_name &&
//                m_vints == test.m_vints && m_nodes == test.m_nodes;
//     }
//     auto toString() const {
//         std::string str;
//         str += "{" + std::to_string( m_a ) + " " + std::to_string( m_b ) + " " + m_name + " " +
//                hub::to_string( m_vints ) + "}";
//         return str;
//     }
//     //    friend std::ostream& operator<<( std::ostream& os, const TestNonPackable& test ) {
//     //        os << test.a << " " << test.b << " " << test.name << " ";
//     ////        ::operator<<( os, test.vints );
//     //        return os;
//     //    }

// //    template <class Serial>
// //    void serialize( Serial& serial ) {
// //        //            serial( m_a, m_b, m_name);
// //        //        serial( m_nodes );
// //        serial( m_a, m_b, m_name, m_vints, m_nodes );
// //    }
// #if CPP_VERSION >= 20
//     static constexpr auto serialize( auto& archive, auto& self ) {
//         return archive( self.m_a, self.m_b, self.m_name, self.m_vints, self.m_nodes );
//     }
// #endif

//   private:
//     double m_a;
//     int m_b;
//     std::string m_name;
//     std::vector<int> m_vints;
//     std::vector<Node> m_nodes;
// };

// struct TestPackable {
//     double a;
//     int b;
//     bool c;

//     static struct {
//     } packable;

//     bool operator==( const TestPackable& test ) const {
//         return a == test.a && b == test.b && c == test.c;
//     }
//     friend std::ostream& operator<<( std::ostream& os, const TestPackable& test ) {
//         os << test.a << " " << test.b << " " << test.c;
//         return os;
//     }
// };

// struct TestForceSerializable {
//     double a;
//     int b;
//     bool c;
//     std::any any;

// //    template <class Serial>
// //    void serialize( Serial& serial ) {
// //        serial( a, b, c );
// //        any = 1;
// //    }

//    void write(hub::Serializer & serializer) const {
//     // void write(auto & serializer) const {
//         serializer.writeAll(a, b, c);
//         serializer.write(std::any_cast<int>(any));
//     }
//    void read(hub::Serializer & serializer) {
//     // void read(auto & serializer) {
//         serializer.readAll(a, b, c);
//         int v;
//         serializer.read(v);
//         any = v;
//     }
// //    static constexpr auto serialize( auto& archive, auto& self ) {
// //        return archive( self.a, self.b, self.c);
// //    }
//     //    static struct {} packable;

//     bool operator==( const TestForceSerializable& test ) const {
//         return a == test.a && b == test.b && c == test.c &&
//                std::any_cast<int>( any ) == std::any_cast<int>( test.any );
//     }
//     friend std::ostream& operator<<( std::ostream& os, const TestForceSerializable& test ) {
//         os << test.a << " " << test.b << " " << test.c << " " << std::any_cast<int>( test.any );
//         return os;
//     }
// };

//         {
//             //            const TestNonPackable toWrite { 1.0, 5, "hello", { 1, 2, 3 }, nodes };
//             //            NodeType toWrite;
//             std::vector<NodeType> toWrite = nodes;
//             using toWriteType             = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( !hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const TestNonPackable toWrite { 1.0, 5, "hello", { 1, 2, 3 }, nodes };
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( !hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             std::vector<TestNonPackable> toWrite { { 1.0, 5, "hello", { 1, 2, 3 }, nodes },
//                                                    { 2.0, 2, "gauthier", { 0, 0, 0 }, nodes } };
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( !hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }
//         //                return;
//         {
//             const int toWrite = 5.0;
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const double& toWrite = 5.0;
//             using toWriteType     = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const uint32_t&& toWrite = 2;
//             using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const unsigned char toWrite[9] = "gauthier";
//             using toWriteType              = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWrite )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( memcmp( toWrite, toRead, strlen( (const char*)toWrite ) ) == 0 );
//             assert( archive.isEnd() );
//         }

//         {
//             const unsigned char buff[9]  = "gauthier";
//             const unsigned char* toWrite = buff;
//             using toWriteType            = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( !hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite, strlen( (const char*)buff ) );
//             assert( !archive.isEnd() );
//             unsigned char toRead[9];
//             archive.read( toRead, strlen( (const char*)buff ) );
//             assert( memcmp( toWrite, toRead, strlen( (const char*)toWrite ) ) == 0 );
//             assert( archive.isEnd() );
//         }

//         {
//             const std::string toWrite = "gauthier";
//             using toWriteType         = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( !hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const TestPackable toWrite { 1.0, 5, true };
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             static_assert( hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         ///////////

//         {
//             const TestForceSerializable toWrite { 1.0, 5, true, 2 };
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             // todo c++17
//             // static_assert( !hub::Serializer::Serializable<toWriteType>() );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }

//         {
//             const std::vector<double> toWrite( 1'000, 5 );
//             using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
//             //        static_assert(! hub::packable_v<toWriteType> );
// #ifdef HUB_DEBUG_OUTPUT
//             std::cout << std::endl
//                       << "------------------ " << TYPE_NAME( toWriteType() )
//                       << " ---------------------" << std::endl;
// #endif
//             archive.write( toWrite );
//             assert( !archive.isEnd() );
//             toWriteType toRead;
//             archive.read( toRead );
//             assert( toWrite == toRead );
//             assert( archive.isEnd() );
//         }


// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#if CPLUSPLUS_VERSION <= 14
#    include "core/std_any.hpp"
#else
#    include <any>
#endif

//#include <any>
#include <cstring>
#include <set>

#include "test_common.hpp"
#include "core/Traits.hpp"
// #include "core/test_core_common.hpp"
// #include "core/io/test_core_io_common.hpp"

#include <core/io/Archive.hpp>
#include <core/io/InputOutput.hpp>

struct TestNonPackable {
    double a;
    int b;
    std::string name;
    std::vector<int> vints;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, name, vints );
    }

    bool operator==( const TestNonPackable& test ) const {
        return a == test.a && b == test.b && name == test.name && vints == test.vints;
    }
    friend std::ostream& operator<<( std::ostream& os, const TestNonPackable& test ) {
        os << test.a << " " << test.b << " " << test.name << " ";
        ::operator<<( os, test.vints );
        return os;
    }
};

struct TestPackable {
    double a;
    int b;
    bool c;

    static struct {
    } packable;

    bool operator==( const TestPackable& test ) const {
        return a == test.a && b == test.b && c == test.c;
    }
    friend std::ostream& operator<<( std::ostream& os, const TestPackable& test ) {
        os << test.a << " " << test.b << " " << test.c;
        return os;
    }
};

struct TestForceSerializable {
    double a;
    int b;
    bool c;
    std::any any;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, c );
        any = 1;
    }
    //    static struct {} packable;

    bool operator==( const TestForceSerializable& test ) const {
        return a == test.a && b == test.b && c == test.c &&
               std::any_cast<int>( any ) == std::any_cast<int>( test.any );
    }
    friend std::ostream& operator<<( std::ostream& os, const TestForceSerializable& test ) {
        os << test.a << " " << test.b << " " << test.c << " " << std::any_cast<int>( test.any );
        return os;
    }
};

template <class Archive>
void process( Archive& archive ) {
    assert( archive.isEnd() );

#ifndef HUB_DEBUG_OUTPUT
    for ( int i = 0; i < 100000; ++i ) {
#endif
        {
            const int toWrite = 5.0;
            using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const double& toWrite = 5.0;
            using toWriteType     = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const uint32_t&& toWrite = 2;
            using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const unsigned char toWrite[9] = "gauthier";
            using toWriteType              = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( memcmp( toWrite, toRead, strlen( (const char*)toWrite ) ) == 0 );
            assert( archive.isEnd() );
        }

        {
            const unsigned char buff[9]  = "gauthier";
            const unsigned char* toWrite = buff;
            using toWriteType            = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( !hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite, strlen( (const char*)buff ) );
            assert( !archive.isEnd() );
            unsigned char toRead[9];
            archive.read( toRead, strlen( (const char*)buff ) );
            assert( memcmp( toWrite, toRead, strlen( (const char*)toWrite ) ) == 0 );
            assert( archive.isEnd() );
        }

        {
            const std::string toWrite = "gauthier";
            using toWriteType         = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( !hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const TestPackable toWrite { 1.0, 5, true };
            using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const TestNonPackable toWrite { 1.0, 5, "hello", { 1, 2, 3 } };
            using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( !hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const TestForceSerializable toWrite { 1.0, 5, true, 2 };
            using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
            static_assert( hub::serializable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }

        {
            const std::vector<double> toWrite( 1'000, 5 );
            using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
            //        static_assert(! hub::packable_v<toWriteType> );
#ifdef HUB_DEBUG_OUTPUT
            std::cout << std::endl
                      << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                      << std::endl;
#endif
            archive.write( toWrite );
            assert( !archive.isEnd() );
            toWriteType toRead;
            archive.read( toRead );
            assert( toWrite == toRead );
            assert( archive.isEnd() );
        }
#ifndef HUB_DEBUG_OUTPUT
    }
#endif
#ifdef DEBUG
        std::cout << "nCall : " << archive.getNCall() << std::endl;
#endif
}

TEST_CASE( "InputOutput test" ) {

    long long durationArchiveZppBits = 0;
    long long durationArchiveImpl = 0;

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
        std::cout << "-------------------- ArchiveImpl --------" << std::endl;
        hub::io::ArchiveT<hub::serializer::SerializerImpl> archiveImpl;
        const auto startClock = std::chrono::high_resolution_clock::now();
        process( archiveImpl );
        const auto endClock = std::chrono::high_resolution_clock::now();
        durationArchiveImpl =
            std::chrono::duration_cast<std::chrono::nanoseconds>( endClock - startClock ).count();
        std::cout << "duration: " << durationArchiveImpl / 1'000'000.0 << " ms" << std::endl;
    }

    const auto ratio = durationArchiveImpl / (double)durationArchiveZppBits;
    CHECK_VALUE( ratio, 1.0, 1.0, "ArchiveZppBits/ArchiveImpl" );
//#ifndef DEBUG
//    CHECK( durationArchiveZppBits < durationArchiveImpl );
//#endif
#ifdef HUB_USE_ZPP_BITS
    static_assert(std::is_same_v<hub::io::Archive, hub::io::ArchiveT<hub::serializer::SerializerZppBits>>);
#endif

    return;
}


#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include <cstring>
#include <set>
#include <any>

#include "test_common.hpp"
// #include "core/test_core_common.hpp"
// #include "core/io/test_core_io_common.hpp"

#include <core/io/Archive.hpp>
#include <core/io/InputOutput.hpp>

struct TestNonPackable {
    double a;
    int b;
    std::string name;
    std::vector<int> vints;

    bool operator==( const TestNonPackable& test ) const {
        return a == test.a && b == test.b && name == test.name &&
               vints == test.vints;
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

    static struct {} packable;

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
        serial( a, b, c);
        any = 1;
    }
//    static struct {} packable;

    bool operator==( const TestForceSerializable& test ) const {
        return a == test.a && b == test.b && c == test.c && std::any_cast<int>(any) == std::any_cast<int>(test.any);
    }
    friend std::ostream& operator<<( std::ostream& os, const TestForceSerializable& test ) {
        os << test.a << " " << test.b << " " << test.c << " " << std::any_cast<int>(test.any);
        return os;
    }
};

TEST_CASE( "InputOutput test" ) {

    hub::io::Archive<hub::io::InputOutput> archive;
    assert( archive.isEnd() );

    {
        const int toWrite = 5.0;
        using toWriteType = std::remove_cvref_t<decltype( toWrite )>;
        static_assert( hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
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
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
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
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
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
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
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
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite, strlen( (const char*)buff ) );
        assert( !archive.isEnd() );
        unsigned char toRead[9];
        archive.read( toRead, strlen( (const char*)buff ) );
        assert( memcmp( toWrite, toRead, strlen( (const char*)toWrite ) ) == 0 );
        assert( archive.isEnd() );
    }

    {
        const std::string toWrite = "gauthier";
        using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
        static_assert(! hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite );
        assert( !archive.isEnd() );
        toWriteType toRead;
        archive.read( toRead );
        assert( toWrite == toRead );
        assert( archive.isEnd() );
    }

    {
        const TestPackable toWrite{1.0, 5, true};
        using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
        static_assert(hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite );
        assert( !archive.isEnd() );
        toWriteType toRead;
        archive.read( toRead );
        assert( toWrite == toRead );
        assert( archive.isEnd() );
    }

    {
        const TestNonPackable toWrite{1.0, 5, "hello", {1, 2, 3}};
        using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
        static_assert(! hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite );
        assert( !archive.isEnd() );
        toWriteType toRead;
        archive.read( toRead );
        assert( toWrite == toRead );
        assert( archive.isEnd() );
    }

    {
        const TestForceSerializable toWrite{1.0, 5, true, 2};
        using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
//        static_assert(! hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite );
        assert( !archive.isEnd() );
        toWriteType toRead;
        archive.read( toRead );
        assert( toWrite == toRead );
        assert( archive.isEnd() );
    }

    {
        const std::vector<double> toWrite(1'000, 5);
        using toWriteType        = std::remove_cvref_t<decltype( toWrite )>;
//        static_assert(! hub::packable_v<toWriteType> );
        std::cout << std::endl << "------------------ " << TYPE_NAME( toWrite ) << " ---------------------"
                  << std::endl;
        archive.write( toWrite );
        assert( !archive.isEnd() );
        toWriteType toRead;
        archive.read( toRead );
        assert( toWrite == toRead );
        assert( archive.isEnd() );
    }



    return;
}

// #include <catch2/catch_test_macros.hpp>
#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "test_common.hpp"
#include "test_io_common.hpp"

// #include <core/Utils.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/input/InputStreamServer.hpp>
// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
// #include <thread>
// #include <net/ClientSocket.hpp>
// #include <net/ServerSocket.hpp>

struct UserDefined {
    int a;
    bool b;
    double c;
    hub::Size_t dataSize() const { return sizeof( int ); }
    auto toString() const {
        return std::to_string( a ) + ":" + std::to_string( b ) + ":" + std::to_string( c );
    }
    bool operator==( const UserDefined& other ) {
        return a == other.a && b == other.b && c == other.c;
    }
};

TEST_CASE( "InputOutputStream local header test" ) {
    TEST_BEGIN()

    const auto port = GET_RANDOM_PORT;

    {
        // auto header = hub::io::make_header(5);
        // const auto header_ref = hub::io::Header( sizeof( int ) );
        // const auto header_ref = hub::io::make_header(1);
        // hub::output::OutputStream outputStream( {}, port );
        const UserDefined userDefined { 1, true, 2.0 };
        const auto header_ref = hub::io::make_header( userDefined );

        hub::output::OutputStream outputStream( header_ref, port );

        hub::input::InputStream inputStream( port );
        // assert( inputStream.getHeader() == hub::io::Header() );
        assert( inputStream.getHeader() == header_ref );

        const int a_ref = 5;
        static_assert( sizeof( int ) == 4 );
        outputStream.write( 5 );
        // std::cout << "write done" << std::endl;
        int a_read;
        inputStream.read( a_read );
        // std::cout << "read done" << std::endl;
        assert( a_ref == a_read );

        hub::io::Memory memory( inputStream.getHeader().getUserDefined() );
        UserDefined userDefined_read;
        memory.read( userDefined_read );
        assert( userDefined == userDefined_read );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}

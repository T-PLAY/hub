// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputStream server deco reco test" ) {
    // const auto hostname = hub::utils::getHostname();
    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();

    {
        hub::output::OutputStream outputStream( FILE_NAME, port, "127.0.0.1", false );

        hub::input::InputStream inputStream( FILE_NAME, port );

        int a = 5;
        outputStream.write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        server.stop();

        a = 6;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );

        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        a = 7;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );
    }

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

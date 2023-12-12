// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <server/Server.hpp>

TEST_CASE( "InputOutputStream kill server test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    auto* server = new hub::Server( port );
    server->asyncRun();

    // not allowed
    {

        // const hub::io::Header header{sizeof(int)};
        hub::output::OutputStream outputStream( FILE_NAME, port );

        hub::input::InputStream inputStream( FILE_NAME, port );

        int a = 5;
        outputStream.write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        // std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
        delete server;
        // std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );

        a = 6;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );
    }
    std::cout << "[test] inputStream ended" << std::endl;
    std::cout << "[test] outputStream ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}

// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    // const auto hostname = hub::utils::getHostname();
    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();
    // std::cout << "[test] server started" << std::endl;

    {
        hub::output::OutputStream outputStream( FILE_NAME, port );
        // std::cout << "[test] outputStream created" << std::endl;

        hub::input::InputStream inputStream( FILE_NAME, port );
        // std::cout << "[test] inputStream created" << std::endl;

        int a = 5;
        outputStream.write( a );
        int a_read;
        inputStream.read( a_read );
        assert( a == a_read );

        // std::cout << "[test] server ending" << std::endl;
        server.stop();
        // std::cout << "[test] server ended" << std::endl;

        // std::cout << "-----------------------------------------------" << std::endl;

        a = 6;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );

        // std::cout << "[test] server starting" << std::endl;
        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        // std::cout << "[test] server started" << std::endl;

        // std::cout << "-----------------------------------------------" << std::endl;

        a = 7;
        outputStream.write( a );
        inputStream.read( a_read );
        assert( a == a_read );
    }
    // std::cout << "[test] outputStream ended" << std::endl;
    // std::cout << "[test] inputStream ended" << std::endl;

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

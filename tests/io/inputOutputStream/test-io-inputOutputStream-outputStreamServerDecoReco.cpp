// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/output/OutputStream.hpp>
#include <io/Server.hpp>

TEST_CASE( "OutputStream server deco reco test" ) {
    // const auto hostname = hub::utils::getHostname();
    const auto port = GET_RANDOM_PORT;

    hub::Server server( port );
    server.asyncRun();
    std::cout << "[test] server started" << std::endl;

    {
        hub::output::OutputStream outputStream( FILE_NAME, port );

        std::cout << "[test] server ending" << std::endl;
        server.stop();
        std::cout << "[test] server ended" << std::endl;


        std::cout << "[test] server starting" << std::endl;
        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        std::cout << "[test] server started" << std::endl;

    }
    std::cout << "[test] outputStream ended" << std::endl;

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

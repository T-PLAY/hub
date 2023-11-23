// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

// #include <core/Utils.hpp>
// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/input/InputStreamServer.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    //    const auto port     = GET_RANDOM_PORT;
    INIT_SERVER

    //        hub::Server server( port );
    //        server.asyncRun();

    {
        hub::output::OutputStream outputStream( FILE_NAME, SERVER_PORT );

        {
            hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );
            //                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    //        server->exit();
    //        delete server;
    //        std::cout << "[test] server ended" << std::endl;
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

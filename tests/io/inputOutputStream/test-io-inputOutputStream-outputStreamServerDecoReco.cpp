// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <core/Utils.hpp>
#include <impl/server2/Server.hpp>
// #include <impl/server2/io/input/InputStreamServer.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
#include <thread>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

//    auto* server = new hub::Server( port );
//    server->asyncRun();

    hub::Server server( port );
    server.asyncRun();
        std::cout << "[test] server started" << std::endl;
    // not allowed
    {

        hub::output::OutputStream outputStream( "streamName", port );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        //        {
        //            hub::input::InputStream inputStream( "streamName", port );
        //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        //        }

        //        server->exit();
//        delete server;
        server.stop();
        std::cout << "[test] server ended" << std::endl;
//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        server.asyncRun();
//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        std::cout << "[test] server started" << std::endl;
//        delete server;
//        server.stop();
//        std::cout << "[test] server ended" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
    }
    //    delete server;
    //    std::cout << "[test] inputStream ended" << std::endl;
    std::cout << "[test] outputStream ended" << std::endl;

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

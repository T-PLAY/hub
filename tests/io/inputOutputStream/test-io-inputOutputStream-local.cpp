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
    double ratio;

    // not allowed
    {
        //        hub::output::OutputStream outputStream( port );
        std::vector<hub::output::OutputStream> outputStreams;
        outputStreams.emplace_back( port );

        {
            std::vector<hub::input::InputStream> inputStreams;
            inputStreams.emplace_back( port );
            const auto& [durationInMillisecondUni, gigaBytePerSecondUni] =
                inputOutputBench( inputStreams, outputStreams, "unicast" );

            {
                //            hub::input::InputStream inputStream( port );
                //            std::vector<hub::input::InputStream> inputStreams;
                for ( int i = 1; i < 10; ++i ) {
                    inputStreams.emplace_back( port );
                }
                //            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                const auto& [durationInMillisecondMulti, gigaBytePerSecondMulti] =
                    inputOutputBench( inputStreams, outputStreams, "multicast" );
                ratio = 100.0 * gigaBytePerSecondMulti / gigaBytePerSecondUni;
            }
        }
        //        server->exit();
        //        delete server;
        //        std::cout << "[test] server ended" << std::endl;
        //        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    //    delete server;
    //    std::cout << "[test] inputStream ended" << std::endl;
    //    std::cout << "[test] outputStream ended" << std::endl;
    // CHECK_VALUE( ratio, 50, 30, "InputOutputStreamLocal:multicast/unicast", "%" );
    CHECK_DECLINE( ratio, "InputOutputStreamLocal:multicast/unicast", "%" );

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

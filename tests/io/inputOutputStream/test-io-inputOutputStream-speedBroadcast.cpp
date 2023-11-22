// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <core/Utils.hpp>
#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>
#include <impl/server2/Server.hpp>
#include <impl/server2/io/input/InputStreamServer.hpp>
// #include <InputSensor.hpp>
// #include <OutputSensor.hpp>
#include <thread>

TEST_CASE( "InputOutputStream test" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;
    const auto port2    = port + 1;

    const auto delay = 100;

    double durationInMillisecondSinglePair;
    double gigaBytePerSecondSinglePair;

    double durationInMillisecondBroadcast;
    double gigaBytePerSecondBroadcast;

    constexpr auto nInputStream = 5;

    {
        hub::Server server( port );
        server.asyncRun();

        {
            hub::output::OutputStream outputStream( "streamName", "127.0.0.1", port );
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            {
                //                hub::input::InputStream inputStream( "streamName", "127.0.0.1",
                //                port ); std::this_thread::sleep_for( std::chrono::milliseconds(
                //                delay ) );
                std::vector<hub::input::InputStream> inputStreams;
                //                for ( int i = 0; i < nInputStream; ++i ) {
                inputStreams.emplace_back( "streamName", "127.0.0.1", port );

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( outputStream, inputStreams );
                    durationInMillisecondSinglePair = durationInMillisecond;
                    gigaBytePerSecondSinglePair     = gigaBytePerSecond;
                }

                for ( int i = 1; i < nInputStream; ++i ) {
                    inputStreams.emplace_back( "streamName", "127.0.0.1", port );
                    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
                }

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( outputStream, inputStreams );
                    durationInMillisecondBroadcast = durationInMillisecond;
                    gigaBytePerSecondBroadcast     = gigaBytePerSecond;
                }

                std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
            }
            std::cout << "[test] inputStreams ended" << std::endl;

            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        }
        std::cout << "[test] outputStream ended" << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStreamSerial] writing/reading rate: "
              << std::to_string( gigaBytePerSecondSinglePair ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStreamSerial] total time: " << durationInMillisecondSinglePair
              << " ms" << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStreamBroadcast] writing/reading rate: "
              << std::to_string( gigaBytePerSecondBroadcast ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStreamBroadcast] total time: " << durationInMillisecondBroadcast
              << " ms" << std::endl;
    std::cout << std::endl;

    const auto ratio = 100.0 * gigaBytePerSecondBroadcast / gigaBytePerSecondSinglePair;
    CHECK_VALUE( ratio, 60, 30, "InputOutputStreamBroadcast/InputOutputStreamSerial", "%" );

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

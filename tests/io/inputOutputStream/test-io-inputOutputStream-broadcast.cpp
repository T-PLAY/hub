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

    double durationInMillisecondSerial;
    double gigaBytePerSecondSerial;

    double durationInMillisecondBroadcast;
    double gigaBytePerSecondBroadcast;

    constexpr auto nOutputStream = 1;
    constexpr auto nInputStream  = 5;

    {
        hub::Server server( port );
        server.asyncRun();

        {
            std::vector<hub::output::OutputStream> outputStreams;
            outputStreams.emplace_back( "streamName",  port );

            {
                std::vector<hub::input::InputStream> inputStreams;
                inputStreams.emplace_back( "streamName", port );

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( inputStreams, outputStreams );
                    durationInMillisecondSerial = durationInMillisecond;
                    gigaBytePerSecondSerial     = gigaBytePerSecond;
                }

                for ( int i = 1; i < nOutputStream; ++i ) {
                    outputStreams.emplace_back(
                        "streamName" + std::to_string( i ), port );
                }

                for ( int i = 1; i < nInputStream; ++i ) {
                    inputStreams.emplace_back( "streamName", port );
                }

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( inputStreams , outputStreams);
                    durationInMillisecondBroadcast = durationInMillisecond;
                    gigaBytePerSecondBroadcast     = gigaBytePerSecond;
                }
            }
            std::cout << "[test] inputStreams ended" << std::endl;
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStreamSerial] writing/reading rate: "
              << std::to_string( gigaBytePerSecondSerial ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStreamSerial] total time: " << durationInMillisecondSerial << " ms"
              << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStreamBroadcast] writing/reading rate: "
              << std::to_string( gigaBytePerSecondBroadcast ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStreamBroadcast] total time: " << durationInMillisecondBroadcast
              << " ms" << std::endl;
    std::cout << std::endl;

    const auto ratio = 100.0 * gigaBytePerSecondBroadcast / gigaBytePerSecondSerial;
    CHECK_VALUE( ratio, 60, 30, "InputOutputStreamBroadcast/InputOutputStreamSerial", "%" );

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

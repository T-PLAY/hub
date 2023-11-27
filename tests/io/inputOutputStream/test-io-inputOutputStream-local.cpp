// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream local test" ) {
    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    double ratio;

    {
        std::vector<hub::output::OutputStream> outputStreams;
        outputStreams.emplace_back( port );

        {
            std::vector<hub::input::InputStream> inputStreams;
            inputStreams.emplace_back( port );
            const auto& [durationInMillisecondUni, gigaBytePerSecondUni] =
                inputOutputBench( inputStreams, outputStreams, "unicast" );

            {
                for ( int i = 1; i < 10; ++i ) {
                    inputStreams.emplace_back( port );
                }

                const auto& [durationInMillisecondMulti, gigaBytePerSecondMulti] =
                    inputOutputBench( inputStreams, outputStreams, "multicast" );
                ratio = gigaBytePerSecondMulti / gigaBytePerSecondUni;
            }
        }
    }
    CHECK_DECLINE( ratio, "InputOutputStreamLocal:broadcast(1->10)/unicast(1->1)", "/" );

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
}

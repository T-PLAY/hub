
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream local multicast test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    const auto port     = GET_RANDOM_PORT;

    double ratio;

    {
        std::vector<hub::output::OutputStream> outputStreams;
        outputStreams.emplace_back( TEST_IO_HEADER, port );

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
    CHECK_DECLINE( ratio, "InputOutputStreamLocal:Broadcast(1->10)/Unicast(1->1)", "/" );

    TEST_END()
}

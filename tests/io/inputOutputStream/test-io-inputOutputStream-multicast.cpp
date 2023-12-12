// #include <catch2/catch_test_macros.hpp>
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_SOCKET

#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream multicast test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();
    // const auto port     = GET_RANDOM_PORT;

    INIT_SERVER

    double durationInMillisecondSerial;
    double gigaBytePerSecondSerial;

    double durationInMillisecondMulticast;
    double gigaBytePerSecondMulticast;

    constexpr auto nStream = 5;

    {
        // hub::Server server( );
        // server.asyncRun();

        {
            std::vector<hub::output::OutputStream> outputStreams;
            outputStreams.emplace_back( FILE_NAME, SERVER_PORT );

            {
                std::vector<hub::input::InputStream> inputStreams;
                inputStreams.emplace_back( FILE_NAME, SERVER_PORT );

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( inputStreams, outputStreams );
                    durationInMillisecondSerial = durationInMillisecond;
                    gigaBytePerSecondSerial     = gigaBytePerSecond;
                }

                for ( int i = 1; i < nStream; ++i ) {
                    outputStreams.emplace_back(
                        FILE_NAME + std::to_string( i ), SERVER_PORT );
                }

                for ( int i = 1; i < nStream; ++i ) {
                    inputStreams.emplace_back(
                        FILE_NAME + std::to_string( i ), SERVER_PORT );
                }

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( inputStreams, outputStreams );
                    durationInMillisecondMulticast = durationInMillisecond;
                    gigaBytePerSecondMulticast     = gigaBytePerSecond;
                }
            }
            std::cout << "[test] inputStreams ended" << std::endl;
        }
        std::cout << "[test] outputStream ended" << std::endl;
    }
    std::cout << "[test] server ended" << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStream:Serial] writing/reading rate: "
              << std::to_string( gigaBytePerSecondSerial ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStream:Serial] total time: " << durationInMillisecondSerial << " ms"
              << std::endl;

    std::cout << "------------------------------------" << std::endl;
    std::cout << "[InputOutputStream:Multicast] writing/reading rate: "
              << std::to_string( gigaBytePerSecondMulticast ) << " Go/s" << std::endl;
    std::cout << "[InputOutputStream:Multicast] total time: " << durationInMillisecondMulticast
              << " ms" << std::endl;
    std::cout << std::endl;

    const auto ratio = gigaBytePerSecondMulticast / gigaBytePerSecondSerial;
    // CHECK_VALUE( ratio, 100, 50, "InputOutputStream:Multicast/Serial", "%" );
    CHECK_DECLINE( ratio, "InputOutputStream:Multicast(5->5)/Serial(1->1)", "/" );

    // std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;
    TEST_END()
}

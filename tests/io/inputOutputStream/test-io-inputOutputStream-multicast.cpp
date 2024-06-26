
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "InputOutputStream multicast test" ) {
    TEST_BEGIN()

    const auto hostname = hub::utils::getHostname();

    INIT_SERVER

    double durationInMillisecondSerial;
    double gigaBytePerSecondSerial;

    double durationInMillisecondMulticast;
    double gigaBytePerSecondMulticast;

    constexpr auto nStream = 5;

    {

        {
            std::vector<hub::output::OutputStream> outputStreams;
            outputStreams.emplace_back( TEST_IO_HEADER, FILE_NAME, SERVER_PORT );

            {
                std::vector<hub::input::InputStream> inputStreams;
                inputStreams.emplace_back( FILE_NAME, SERVER_PORT );
                CHECK( inputStreams.back().getHeader() == TEST_IO_HEADER );

                {
                    const auto& [durationInMillisecond, gigaBytePerSecond] =
                        inputOutputBench( inputStreams, outputStreams );
                    durationInMillisecondSerial = durationInMillisecond;
                    gigaBytePerSecondSerial     = gigaBytePerSecond;
                }

                for ( int i = 1; i < nStream; ++i ) {
                    outputStreams.emplace_back(
                        TEST_IO_HEADER, FILE_NAME + std::to_string( i ), SERVER_PORT );
                }

                for ( int i = 1; i < nStream; ++i ) {
                    inputStreams.emplace_back( FILE_NAME + std::to_string( i ), SERVER_PORT );
                    CHECK( inputStreams.back().getHeader() == TEST_IO_HEADER );
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

    TEST_END()
}

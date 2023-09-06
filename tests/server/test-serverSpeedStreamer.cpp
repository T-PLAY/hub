#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <client/Streamer.hpp>

// #include <server/Server.hpp>
#include <net/ServerSocket.hpp>

#include <utils/Utils.hpp>

TEST_CASE( "Server speed test : streamer" ) {
    const auto hostname = hub::utils::getHostname();

    constexpr int nAcqs       = 200;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    unsigned char* data       = new unsigned char[dataSize];
    unsigned char* data2      = new unsigned char[dataSize];

    std::vector<hub::Acquisition> acqs( nAcqs );
    for ( int i = 0; i < dataSize; ++i ) {
        data[i] = i % 256;
    }

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        hub::Acquisition acq( iAcq, iAcq );
        acq << hub::Measure( reinterpret_cast<unsigned const char*>( data ),
                                   dataSize,
                                   { { width, height }, hub::Format::BGR8 } );
        acqs.at( iAcq ) = std::move( acq );
    }

    double megaBytesPerSeconds;
    double megaBytesPerSeconds2;

    {
        const std::string ipv4 = "127.0.0.1";
        const int port         = GET_RANDOM_PORT;
        std::cout << "[test][ClientSocket] start streaming" << std::endl;
        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        auto clientServerSocket = serverSocket.waitNewClient();
        //const int packetSize    = 2'000'000; // 2Go network memory buffer
        const int packetSize    = 500'000; // 500Mo network memory buffer MacOS
        const int nPart         = dataSize / packetSize;

        const auto& start = std::chrono::high_resolution_clock::now();
        {
            for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                int uploadSize = 0;

                for ( int i = 0; i < nPart - 1; ++i ) {
                    clientSocket.write( data + uploadSize, packetSize );
                    clientServerSocket.read( data2 + uploadSize, packetSize );

                    uploadSize += packetSize;
                }

                clientSocket.write( data + uploadSize, dataSize - uploadSize );
                clientServerSocket.read( data2 + uploadSize, dataSize - uploadSize );

                assert( !memcmp( data, data2, dataSize ) );
            }
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        std::cout << "[test][ClientSocket] end streaming" << std::endl;
        const auto& duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        const auto& bytes           = dataSize * nAcqs;
        const auto& bytesPerSeconds = 1000.0 * bytes / duration;
        megaBytesPerSeconds         = bytesPerSeconds / 1000'000.0;

        std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
                  << std::endl;
        std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds
                  << " Mo/s" << std::endl;
    }

    {
        //        const int port2 = port + 1;

        std::cout << "[test][InputOutputSensor] ############################### server start"
                  << std::endl;
        //        hub::Server server( port2 );
        //        server.setMaxClients( 2 );
        //        server.asyncRun();
        std::cout << "[test][InputOutputSensor] server end ------------------------------"
                  << std::endl;

        {
            std::cout
                << "[test][InputOutputSensor] ############################### outputStream start"
                << std::endl;

            hub::SensorSpec sensorSpec( "sensorName",
                                        { { { width, height }, hub::Format::BGR8 } } );

            hub::client::Streamer streamer;
            streamer.addStream( FILE_NAME, sensorSpec );
            int iTryConnect = 0;
            while ( !streamer.isConnected() && iTryConnect < 10 ) {
                std::cout << "[test] waiting for streamer started" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTryConnect;
            }
            CHECK( streamer.isConnected() );
            assert(streamer.isConnected());

            hub::InputSensor inputSensor(
                //                hub::io::InputStream( "streamName", hub::net::ClientSocket( ipv4,
                //                port2 ) ) );
                //                hub::input::InputStreamServer( "streamName", ipv4, port2 ) );
                hub::input::InputStream( FILE_NAME ) );

            const auto& sensorSpec2 = inputSensor.getSpec();
            CHECK( sensorSpec == sensorSpec2 );

            std::cout
                << "[test][InputOutputSensor] ############################### send acquisitions"
                << std::endl;
            const auto& start2 = std::chrono::high_resolution_clock::now();
            for ( int i = 0; i < nAcqs; ++i ) {
                const auto& acq = acqs.at( i );
                streamer.newAcquisition( FILE_NAME, acq );
                hub::Acquisition acq2;
                inputSensor >> acq2;
                CHECK( acq2 == acqs.at( i ) );
            }

            const auto& end2 = std::chrono::high_resolution_clock::now();
            const auto& duration2 =
                std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
            const auto& bytes2           = dataSize * nAcqs;
            const auto& bytesPerSeconds2 = 1000.0 * bytes2 / duration2;
            megaBytesPerSeconds2         = bytesPerSeconds2 / 1000'000.0;

            std::cout << "[test][InputOutputSensor] Mega byte wrote : " << bytes2 / 1000'000.0
                      << " Mo" << std::endl;
            std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds2
                      << " Mo/s" << std::endl;
        }
    }

    std::cout << std::endl;

    std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds << " Mo/s"
              << std::endl;
    std::cout << "[test][InputOutputSensor] Mega byte per second : " << megaBytesPerSeconds2
              << " Mo/s" << std::endl;

    std::cout << std::endl;

    const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
    std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %" << std::endl;

//#ifdef WIN32
//#    ifdef DEBUG
//    checkRatio( ratio, 20 );
//#    else
//    checkRatio( ratio, 40 );
//#    endif
//#else
//    if ( hostname == "msi" ) { checkRatio( ratio, 60, 10 ); }
//    else {
//#    ifdef DEBUG
//        checkRatio( ratio, 50, 10 );
//#    else
//        checkRatio( ratio, 40, 10 );
//#    endif
//    }
//#endif

    std::cout << "[test] tested on machine: '" << hostname << "'" << std::endl;

    delete[] data;
    delete[] data2;
}

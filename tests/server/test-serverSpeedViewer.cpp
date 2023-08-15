#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>
#include <client/Viewer.hpp>

#include <server/Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : viewer" ) {
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
        acq << hub::data::Measure( reinterpret_cast<unsigned const char*>( data ),
                                   dataSize,
                                   { { width, height }, hub::Format::BGR8 } );
        acqs.at( iAcq ) = std::move( acq );
    }

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    std::cout << "[test][ClientSocket] start streaming" << std::endl;
    const auto& start = std::chrono::high_resolution_clock::now();
    {
        hub::net::ServerSocket serverSocket( port );
        hub::net::ClientSocket clientSocket( ipv4, port );
        auto clientServerSocket = serverSocket.waitNewClient();
        const int packetSize    = 2'000'000; // 2Go network memory buffer
        const int nPart         = dataSize / packetSize;
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
    const auto& bytes               = dataSize * nAcqs;
    const auto& bytesPerSeconds     = 1000.0 * bytes / duration;
    const auto& megaBytesPerSeconds = bytesPerSeconds / 1000'000.0;

    std::cout << "[test][ClientSocket] Mega byte wrote : " << bytes / 1000'000.0 << " Mo"
              << std::endl;
    std::cout << "[test][ClientSocket] Mega byte per second : " << megaBytesPerSeconds << " Mo/s"
              << std::endl;

    {
        const int port2 = port + 1;

        std::cout << "[test][InputOutputSensor] ############################### server start"
                  << std::endl;
        hub::server::Server server( port2 );
        server.setMaxClients( 3 );
        server.asyncRun();
        std::cout << "[test][InputOutputSensor] server end ------------------------------"
                  << std::endl;

        {
            std::cout
                << "[test][InputOutputSensor] ############################### outputStream start"
                << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                "streamName",
                hub::net::ClientSocket( ipv4, port2 ) );

            std::cout
                << "[test][InputOutputSensor] ############################### inputStream start"
                << std::endl;

            {
                // startConstruction
                bool newStreamerAdded = false;
                auto onNewStreamer    = [&]( const std::string& streamName,
                                          const hub::SensorSpec& sensorSpec ) {
                    std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;

                    CHECK( sensorSpec.getAcquisitionSize() == dataSize );
                    CHECK( sensorSpec.getSensorName() == "sensorName" );

                    const auto& resolutions = sensorSpec.getResolutions();
                    CHECK( resolutions.size() == 1 );
                    CHECK( resolutions[0].first.size() == 2 );
                    CHECK( resolutions[0].first.at( 0 ) == width );
                    CHECK( resolutions[0].first.at( 1 ) == height );
                    CHECK( resolutions[0].second == hub::Format::BGR8 );
                    newStreamerAdded = true;
                    return true;
                };
                auto onDelStreamer = []( const std::string& streamName,
                                         const hub::SensorSpec& sensorSpec ) {
                    std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
                };
                auto onServerNotFound = []( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerNotFound : " << ipv4 << " " << port
                              << std::endl;
                };
                bool serverConnected   = false;
                auto onServerConnected = [&]( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port
                              << std::endl;
                    serverConnected = true;
                };
                auto onServerDisconnected = []( const std::string& ipv4, int port ) {
                    std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port
                              << std::endl;
                };
                int iAcq              = 0;
                auto onNewAcquisition = [&]( const std::string& streamName,
                                             const hub::Acquisition& acq ) {
                    CHECK( acq == acqs.at( iAcq ) );
                    ++iAcq;
                };
                auto onSetProperty = []( const std::string& streamName,
                                         const std::string& objectName,
                                         int property,
                                         const hub::Any& value ) {
                    std::cout << "[example-viewer] onSetProperty " << streamName << std::endl;
                };
                auto onLogMessage = []( const std::string& logMessage ) {
                    std::cout << "[example-viewer] onLogMessage '" << logMessage << "'"
                              << std::endl;
                };

                std::cout << "[Test] ############################### viewer start" << std::endl;
                hub::client::Viewer viewer { onNewStreamer,
                                             onDelStreamer,
                                             onServerNotFound,
                                             onServerConnected,
                                             onServerDisconnected,
                                             onNewAcquisition,
                                             onSetProperty,
                                             ipv4,
                                             port2 };

                while ( !serverConnected ) {
                    std::cout << "[test] waiting for server started" << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                }
                assert( viewer.isConnected() );

                std::cout
                    << "[test][InputOutputSensor] ############################### send acquisitions"
                    << std::endl;
                const auto& start2 = std::chrono::high_resolution_clock::now();
                for ( int i = 0; i < nAcqs; ++i ) {
                    outputSensor << acqs.at( i );
                }

                while ( iAcq != nAcqs ) {
                    std::cout << "[test] waiting for viewer read all acqs" << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                }
                const auto& end2 = std::chrono::high_resolution_clock::now();
                const auto& duration2 =
                    std::chrono::duration_cast<std::chrono::milliseconds>( end2 - start2 ).count();
                const auto& bytes2               = dataSize * nAcqs;
                const auto& bytesPerSeconds2     = 1000.0 * bytes / duration2;
                const auto& megaBytesPerSeconds2 = bytesPerSeconds2 / 1000'000.0;

                std::cout << "[test][InputOutputSensor] Mega byte wrote : " << bytes2 / 1000'000.0
                          << " Mo" << std::endl;
                std::cout << "[test][InputOutputSensor] Mega byte per second : "
                          << megaBytesPerSeconds2 << " Mo/s" << std::endl;

                const auto ratio = 100.0 * megaBytesPerSeconds2 / megaBytesPerSeconds;
                std::cout << "[test][ClientSocket/InputOutputSensor] ratio : " << ratio << " %"
                          << std::endl;
            #ifdef  WIN32
                #ifdef DEBUG
                    CHECK( ratio > 15 );
                #else
                    CHECK( ratio > 35 );
                #endif
            #else
                CHECK( ratio > 35 );
            #endif
            } // end viewer
        }
    }

    delete[] data;
    delete[] data2;
}

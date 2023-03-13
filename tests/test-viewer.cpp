#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

#include <client/Viewer.hpp>

#include <Server.hpp>

// #include <filesystem>
// #include <iostream>

//clang-format off

// #include <Streamer.hpp>

// int main() {
TEST_CASE( "Viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    //    constexpr int port     = 9002;
//    srand( (unsigned)time( NULL ) );
//    const int port = rand() % 65535;
    const int port = getRandomPort();

    // startConstruction
    auto onNewStreamer = [=]( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;
        // accept all stream to run
        return true;
    };
    auto onDelStreamer = []( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
    };
    auto onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const std::string& streamName, const hub::Acquisition& acq ) {
        std::cout << "[example-viewer] onNewAcquisition : " << acq << std::endl;
    };

    std::cout << "[Test] ############################### viewer start" << std::endl;
    hub::client::Viewer viewer{
        onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected, onNewAcquisition };

    viewer.setIpv4( ipv4 );
    viewer.setPort( port );
    CHECK( viewer.getIpv4() == ipv4 );
    CHECK( viewer.getPort() == port );
    CHECK( !viewer.isConnected() );

    viewer.setAutoSync( false );
    // endConstruction

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    {
        std::cout << "[Test] ############################### server start" << std::endl;
        Server server( port );
        server.setMaxClients( 2 );
        //    server.setAcqPing( false );
        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
        CHECK( viewer.isConnected() );

        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;

            const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
            hub::SensorSpec sensorSpec( "sensorName", { resolution } );
            hub::OutputSensor outputSensor(
                sensorSpec,
                hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

            unsigned char data[3] { 1, 2, 3 };
            hub::Acquisition acq =
                std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );
            outputSensor << acq;
            std::cout << "[Test] ############################### outputSensor end" << std::endl;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
    std::cout << "[Test] ############################### server end" << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    //    std::cout << "Starting viewer listening" << std::endl
    //              << "Press any key to terminate" << std::endl;
    //    getchar();

    //    return 0;
}
//clang-format on

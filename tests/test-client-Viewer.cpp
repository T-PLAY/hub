#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

#include <client/Viewer.hpp>

#include <server/Server.hpp>

// #include <filesystem>
// #include <iostream>

//clang-format off

// #include <Streamer.hpp>

// int main() {
TEST_CASE( "Viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    // startConstruction
    auto onNewStreamer = [=]( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;
        // accept all stream to run
        //        return true;
        return false;
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
    hub::client::Viewer viewer {
        onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected, onNewAcquisition };

    viewer.setIpv4( ipv4 );
    viewer.setPort( port );
    CHECK( viewer.getIpv4() == ipv4 );
    CHECK( viewer.getPort() == port );
    CHECK( !viewer.isConnected() );

    viewer.setAutoSync( false );
    // endConstruction
    // todo delay -> 0

    constexpr int delay = 0;

    //    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    {
        std::cout << "[Test] ############################### server start" << std::endl;
        hub::server::Server server( port );
        server.setMaxClients( 2 );
        server.asyncRun();
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        while ( !viewer.isConnected() ) {
            std::cout << "[test] waiting for viewer connected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        }

        CHECK( viewer.isConnected() );

        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;

            const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
            hub::SensorSpec::MetaData metaData;
            metaData["parent"] = "streamName2";
            hub::SensorSpec sensorSpec( "sensorName", { resolution } );
            hub::OutputSensor outputSensor(
                sensorSpec,
//                hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
                "streamName", hub::net::ClientSocket( ipv4, port ) );
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            unsigned char data[3] { 1, 2, 3 };
            hub::Acquisition acq =
                std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );
            for ( int i = 0; i < 10; ++i ) {
                outputSensor << acq;
            }
            std::cout << "[Test] ############################### outputSensor end" << std::endl;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        //        server.detach();
    }
    std::cout << "[Test] ############################### server end" << std::endl;

    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}
//clang-format on

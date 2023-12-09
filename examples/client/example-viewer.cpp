// #include "test_common.hpp"
// #include <catch2/catch_test_macros.hpp>

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

// #include <client/Viewer.hpp>
// #include <server/Server.hpp>

// #include <filesystem>
// #include <iostream>

// #include <Streamer.hpp>
#include <client/Viewer.hpp>
#include <sensor/OutputSensor.hpp>

int main() {
    // TEST_CASE( "Viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    //    const int port         = GET_RANDOM_PORT;
    const int port = 1883;

    hub::client::ViewerHandler viewerHandler;
    // startConstruction
    viewerHandler.onNewStreamer = [=]( const std::string& streamName,
                              const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "\t[example-viewer] onNewStreamer : " << streamName << std::endl;
        // accept all stream to run
        return true;
        //        return false;
    };
    viewerHandler.onDelStreamer = []( const std::string& streamName,
                             const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "\t[example-viewer] onDelStreamer : " << streamName << std::endl;
    };
    viewerHandler.onServerNotFound = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "\t[example-viewer] onServerDisconnected : " << ipv4 << " " << port
                  << std::endl;
    };
    viewerHandler.onNewAcquisition = []( const std::string& streamName,
                                const hub::sensor::Acquisition& acq ) {
        std::cout << "\t[example-viewer] onNewAcquisition : " << acq << std::endl;
    };
    viewerHandler.onSetProperty = []( const std::string& streamName,
                             const std::string& objectName,
                             int property,
                             const hub::Any& value ) {
        std::cout << "\t[example-viewer] onSetProperty " << streamName << std::endl;
    };
    viewerHandler.onLogMessage = []( const std::string& logMessage ) {
        std::cout << "[example-viewer] onLogMessage '" << logMessage << "'" << std::endl;
    };

    {
        std::cout << "\t[example-viewer] creating viewer" << std::endl;
        hub::client::Viewer viewer( FILE_NAME,
                                    std::move(viewerHandler),
                                    ipv4,
                                    port
                                    //                                        ,onLogMessage
        );
        //    hub::client::Viewer viewer {
        //        onNewStreamer, onDelStreamer, onServerNotFound, onServerConnected,
        //        onServerDisconnected, onNewAcquisition, onSetProperty };

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        {
            std::cout << "\t[example-viewer] creating outputSensor" << std::endl;
            // hub::sensor::Resolution res { { 1 }, hub::sensor::Format::BGR8 };
            using res = hub::sensor::format::BGR8;
            // hub::sensor::SensorSpec sensorSpec( "sensorName", { res } );
            hub::sensor::SensorSpec sensorSpec( "sensorName" );
            hub::sensor::OutputSensor outputSensor(
                sensorSpec, hub::output::OutputStream( "streamName", port, ipv4 ) );

            std::this_thread::sleep_for( std::chrono::milliseconds( 3000 ) );

            // unsigned char data[3] { 1, 2, 3 };
            // hub::sensor::Acquisition acq =
                // std::move( hub::sensor::Acquisition( 1, 1 ) << hub::Measure( data, 3, res ) );
            auto acq = outputSensor.acqMsg();
            acq.start() = 1;
            acq.end() = 1;
            auto & bgr8 = acq.get<hub::sensor::format::BGR8&>();
            bgr8.b = 1;
            bgr8.g = 2;
            bgr8.r = 3;
            outputSensor << acq;

            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

            std::cout << "\t----------------------------------------------------------------------"
                      << std::endl;
        }
        std::cout << "\t[example-viewer] outputSensor ended" << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        std::cout << "\t----------------------------------------------------------------------"
                  << std::endl;
    }
    std::cout << "\t[example-viewer] viewer ended" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    std::cout << "\t[example-viewer] ended" << std::endl;

    //    std::cout << "[example-viewer] press key to quit" << std::endl;
    //    getchar();

    //    viewer.setIpv4( ipv4 );
    //    viewer.setPort( port );
    //    CHECK( viewer.getIpv4() == ipv4 );
    //    CHECK( viewer.getPort() == port );
    //    CHECK( !viewer.isConnected() );

    //    viewer.setAutoSync( false );
    //    // endConstruction
    //    // todo delay -> 0

    //    constexpr int delay = 0;

    //    {
    //        std::cout << "[Test] ############################### server start" << std::endl;
    //        hub::Server server( port );
    //        server.setMaxClients( 2 );
    //        server.asyncRun();
    //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    //        while ( !viewer.isConnected() ) {
    //            std::cout << "[test] waiting for viewer connected" << std::endl;
    //            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    //        }

    //        CHECK( viewer.isConnected() );

    //        {
    //            std::cout << "[Test] ############################### outputSensor start" <<
    //            std::endl;

    //            const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    //            hub::SensorSpec::MetaData metaData;
    //            metaData["parent"] = "streamName2";
    //            hub::SensorSpec sensorSpec( "sensorName", { resolution } );
    //            hub::sensor::OutputSensor outputSensor(
    //                sensorSpec,
    ////                "streamName", hub::net::ClientSocket( ipv4, port ) );
    //                "streamName", ipv4, port );
    //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    //            unsigned char data[3] { 1, 2, 3 };
    //            hub::sensor::Acquisition acq =
    //                std::move( hub::sensor::Acquisition( 0, 1 ) << hub::Measure( data, 3,
    //                resolution ) );
    //            for ( int i = 0; i < 10; ++i ) {
    //                outputSensor << acq;
    //            }
    //            std::cout << "[Test] ############################### outputSensor end" <<
    //            std::endl;
    //        }
    //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    //    }
    //    std::cout << "[Test] ############################### server end" << std::endl;

    //    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    std::cout << "\t----------------------------------------------------------------------"
              << std::endl;
}

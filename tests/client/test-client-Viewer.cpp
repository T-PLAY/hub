
#include "io/test_io_common.hpp"
#include "test_common.hpp"

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

#include <client/Viewer.hpp>

#include <sensor/OutputSensor.hpp>

// #include <server/Server.hpp>

// #include <iostream>

// #include <Streamer.hpp>

// int main() {
TEST_CASE( "Viewer" ) {

    INIT_SERVER

    // const std::string ipv4 = "127.0.0.1";
    //    const int port         = GET_RANDOM_PORT;
    //    const int port         = hub::io::s_serverDefaultPort;

    hub::client::ViewerHandler viewerHandler;

    // startConstruction
    viewerHandler.onNewStreamer = [=]( const std::string& streamName,
                                       const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;
        // accept all stream to run
        return false;
    };
    viewerHandler.onDelStreamer = []( const std::string& streamName,
                                      const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
    };
    viewerHandler.onServerNotFound = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onNewAcquisition = []( const std::string& streamName,
                                         const hub::sensor::Acquisition& acq ) {
        std::cout << "[example-viewer] onNewAcquisition : " << acq << std::endl;
    };
    viewerHandler.onSetProperty = []( const std::string& streamName,
                                      const std::string& objectName,
                                      int property,
                                      const hub::Any& value ) {
        std::cout << "[example-viewer] onSetProperty " << streamName << std::endl;
    };
    viewerHandler.onLogMessage = []( const std::string& logMessage ) {
        std::cout << "[example-viewer] onLogMessage '" << logMessage << "'" << std::endl;
    };

    std::cout << "[Test] ############################### viewer start" << std::endl;
    //    hub::client::ViewerServer viewer {
    hub::client::Viewer viewer {
        // ipv4, port,
        FILE_NAME,
        std::move( viewerHandler )
        // onNewStreamer,
        // onDelStreamer,
        // onServerNotFound,
        // onServerConnected,
        // onServerDisconnected,
        // onNewAcquisition,
        // onSetProperty
    };

    //    viewer.setIpv4( ipv4 );
    //    viewer.setPort( port );
    //    CHECK( viewer.getIpv4() == ipv4 );
    //    CHECK( viewer.getPort() == port );
    //    CHECK( !viewer.isConnected() );

    //    viewer.setAutoSync( false );
    // endConstruction
    // todo delay -> 0

    //    constexpr int delay = 0;

    {
        std::cout << "[Test] ############################### server start" << std::endl;
        //        hub::Server server( port );
        //        server.setMaxClients( 2 );
        //        server.asyncRun();
        //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        int iTryConnect = 0;
        while ( !viewer.isConnected() && iTryConnect < 10 ) {
            std::cout << "[test] waiting for viewer connected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTryConnect;
        }
        CHECK( viewer.isConnected() );
        assert( viewer.isConnected() );

        // {
        //     std::cout << "[Test] ############################### outputSensor start" <<
        //     std::endl;

        //     // const auto resolution = hub::sensor::Resolution { { 1 }, hub::sensor::Format::BGR8
        //     }; using Resolution = hub::sensor::format::BGR8; hub::sensor::SensorSpec::MetaData
        //     metaData; metaData["parent"] = "parentName";
        //     // hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );
        //     const hub::sensor::SensorSpec sensorSpec( "sensorName", metaData );
        //     hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec,
        //                                             hub::output::OutputStream( FILE_NAME,
        //                                             SERVER_PORT ) );
        //     //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        //     // unsigned char data[3] { 1, 2, 3 };
        //     // hub::sensor::Acquisition acq = std::move( hub::sensor::Acquisition( 0, 1 )
        //                                               // << hub::Measure( data, 3, resolution )
        //                                               );
        //     auto acq = outputSensor.acq();
        //     // auto * data = acq.data();
        //     auto & bgr8 = acq.get<hub::sensor::format::BGR8&>();
        //     for ( int i = 0; i < 10; ++i ) {
        //         // data[0] = data[1] = data[2] = i;
        //         bgr8.b = i;
        //         bgr8.g = i;
        //         bgr8.r = i;
        //         outputSensor << acq;
        //     }
        //     std::cout << "[Test] ############################### outputSensor end" << std::endl;
        // }

        //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    }
    std::cout << "[Test] ############################### server end" << std::endl;

    //    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

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

    // INIT_SERVER
    const int port = GET_RANDOM_PORT;

    std::mutex mtxPrint;

    using Resolution      = hub::sensor::format::BGR8;
    const auto resolution = hub::make_matrix<Resolution>();

    hub::sensor::SensorSpec::MetaData metaData;
    metaData["parent"] = "parentName";

    const hub::sensor::SensorSpec sensorSpec_ref( "sensorName", resolution, metaData );
    std::cout << "sensorSpec : " << sensorSpec_ref << std::endl;

    {

        hub::client::ViewerHandler viewerHandler;

        int nNewStreamer        = 0;
        int nDelStreamer        = 0;
        int nServerNotFound     = 0;
        int nServerConnected    = 0;
        int nServerDisconnected = 0;
        int nNewAcquisition     = 0;

        // startConstruction
        viewerHandler.onNewStreamer = [&]( const std::string& streamName,
                                           const hub::sensor::SensorSpec& sensorSpec ) {
            std::cout << "[test-client-Viewer] onNewStreamer : " << streamName << ", " << sensorSpec
                      << std::endl;
            // accept all stream to run
            assert( sensorSpec_ref == sensorSpec );
            ++nNewStreamer;
            return true;
        };
        viewerHandler.onDelStreamer = [&]( const std::string& streamName,
                                           const hub::sensor::SensorSpec& sensorSpec ) {
            std::cout << "[test-client-Viewer] onDelStreamer : " << streamName << std::endl;
            assert( sensorSpec_ref == sensorSpec );
            ++nDelStreamer;
        };
        viewerHandler.onServerNotFound = [&]( const std::string& ipv4, int port ) {
            std::cout << "[test-client-Viewer] onServerNotFound : " << ipv4 << " " << port
                      << std::endl;
            ++nServerNotFound;
        };
        viewerHandler.onServerConnected = [&]( const std::string& ipv4, int port ) {
            std::cout << "[test-client-Viewer] onServerConnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerConnected;
        };
        viewerHandler.onServerDisconnected = [&]( const std::string& ipv4, int port ) {
            std::cout << "[test-client-Viewer] onServerDisconnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerDisconnected;
        };
        viewerHandler.onNewAcquisition = [&]( const std::string& streamName,
                                              const hub::sensor::Acquisition& acq ) {
            mtxPrint.lock();
            std::cout << "[test-client-Viewer] onNewAcquisition : " << acq << std::endl;
            mtxPrint.unlock();
            ++nNewAcquisition;
        };
        viewerHandler.onSetProperty = []( const std::string& streamName,
                                          const std::string& objectName,
                                          int property,
                                          const hub::Any& value ) {
            std::cout << "[test-client-Viewer] onSetProperty " << streamName << std::endl;
        };
        viewerHandler.onLogMessage = []( const std::string& logMessage ) {
            std::cout << "[test-client-Viewer] onLogMessage '" << logMessage << "'" << std::endl;
        };

        std::cout << "[test] ############################### viewer start" << std::endl;
        hub::client::Viewer viewer { FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port };

        std::cout << "[test] ############################### server start" << std::endl;
        assert( nServerDisconnected == 0 );
        assert( !viewer.isConnected() );
        while ( nServerNotFound < 2 ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
            std::cout << "[test] waiting for server not found" << std::endl;
        }
        int iTry = 0;

        {
            hub::Server server( port );
            server.asyncRun();

            while ( !viewer.isConnected() && iTry < 10 ) {
                std::cout << "[test] waiting for viewer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( viewer.isConnected() );
            assert( nServerConnected == 1 );

            {
                std::cout << "[Test] ############################### outputSensor start"
                          << std::endl;

                hub::sensor::OutputSensorT<Resolution> outputSensor(
                    sensorSpec_ref, hub::output::OutputStream( FILE_NAME, port ) );

                iTry = 0;
                while ( viewer.nStreaming() != 1 && iTry < 10 ) {
                    std::cout << "[test] waiting for outputStream connected" << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( viewer.nStreaming() == 1 );
                assert( nNewStreamer == 1 );
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                // hub::sensor::AcquisitionT<Resolution> acq;
                auto acq    = outputSensor.acq();
                auto& start = acq.start();
                auto& end   = acq.end();
                // auto * data = acq.data();
                auto& bgr8 = acq.get<hub::sensor::format::BGR8&>();
                for ( int i = 0; i < 10; ++i ) {
                    start  = i;
                    end    = i;
                    bgr8.b = i;
                    bgr8.g = i;
                    bgr8.r = i;
                    outputSensor << acq;
                    mtxPrint.lock();
                    std::cout << "[test] write acq : " << acq << std::endl;
                    mtxPrint.unlock();
                }

                iTry = 0;
                while ( nNewAcquisition < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for acq received : " << nNewAcquisition
                              << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( nNewAcquisition == 10 );

            } // end outputSensor
            std::cout << "[Test] ############################### outputSensor end" << std::endl;

            iTry = 0;
            while ( viewer.nStreaming() != 0 && iTry < 10 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( viewer.nStreaming() == 0 );
            assert( nDelStreamer == 1 );
            assert( nServerDisconnected == 0 );

        } // end server
        std::cout << "[test] ############################### server end" << std::endl;

        iTry = 0;
        while ( viewer.isConnected() && iTry < 20 ) {
            std::cout << "[test] waiting for viewer disconnected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTry;
        }
        assert( !viewer.isConnected() );
        assert( nServerDisconnected == 1 );

    } // end viewer
    std::cout << "[test] ############################### viewer end" << std::endl;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
}

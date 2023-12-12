#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "io/test_io_common.hpp"
#include "test_common.hpp"

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

// #include <core/io/Memory.hpp>
#include <client/Viewer.hpp>

#include <io/output/OutputStream.hpp>
// #include <sensor/OutputSensor.hpp>

// #include <server/Server.hpp>

// #include <iostream>

// #include <Streamer.hpp>

// int main() {
TEST_CASE( "Viewer" ) {
    TEST_BEGIN()

    // INIT_SERVER
    const int port = GET_RANDOM_PORT;

    // std::mutex mtxPrint;

    // using Resolution      = hub::sensor::format::BGR8;
    // const auto resolution = hub::make_matrix<Resolution>();

    // hub::sensor::SensorSpec::MetaData metaData;
    // metaData["parent"] = "parentName";

    // const hub::sensor::SensorSpec sensorSpec_ref( "sensorName", resolution, metaData );
    // std::cout << "sensorSpec : " << sensorSpec_ref << std::endl;
    // const header_ref
    const hub::Datas_t header_ref { 'g', 'o', 't' };
    std::cout << "[test] header_ref : " << header_ref << std::endl;
    // hub::io::Memory memory;

    // const auto& [data_ref, dataSize_ref] = generateTestData();

    {

        hub::client::ViewerHandler viewerHandler;

        int nNewStreamer                     = 0;
        int nDelStreamer                     = 0;
        int nServerNotFound                  = 0;
        int nServerConnected                 = 0;
        std::atomic<int> nServerDisconnected = 0;
        // int nNewAcquisition                  = 0;
        int nNewData = 0;

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

        viewerHandler.onNewStream = [&]( const std::string& streamName,
                                         // const hub::sensor::SensorSpec& sensorSpec ) {
                                         const hub::Datas_t& header ) {
            std::cout << "[test-client-Viewer] onNewStream : " << streamName << ", " << header
                      << std::endl;
            assert( header_ref == header );
            ++nNewStreamer;
            return true;
        };
        viewerHandler.onNewData = [&]( const std::string& streamName,
                                       hub::input::InputStream& inputStream ) {
            assert( inputStream.getHeader() == header_ref );
            int a;
            inputStream.read( a );
            // mtxPrint.lock();
            std::cout << "[test-client-Viewer] onNewData : " << a << std::endl;
            // mtxPrint.unlock();
            assert( a == nNewData );
            ++nNewData;
        };
        // viewerHandler.onNewAcquisition = [&]( const std::string& streamName,
        //                                       const hub::sensor::Acquisition& acq ) {
        //     mtxPrint.lock();
        //     std::cout << "[test-client-Viewer] onNewAcquisition : " << acq << std::endl;
        //     mtxPrint.unlock();
        //     ++nNewAcquisition;
        // };
        viewerHandler.onDelStream = [&]( const std::string& streamName ) {
            // const hub::sensor::SensorSpec& sensorSpec ) {
            // const hub::Datas_t& retainedData ) {
            std::cout << "[test-client-Viewer] onDelStream : " << streamName << std::endl;
            // assert( header_ref == retainedData );
            ++nDelStreamer;
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

        // std::cout << "[test] ############################### viewer start" << std::endl;
        CONSTRUCT_BEGIN( "Viewer" );
        hub::client::Viewer viewer { FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port };
        CONSTRUCT_END( "Viewer" );

        // std::cout << "[test] ############################### server start" << std::endl;
        assert( nServerDisconnected == 0 );
        assert( !viewer.isConnected() );
        while ( nServerNotFound == 0 ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[test] waiting for server not found" << std::endl;
        }
        int iTry = 0;

        {
            CONSTRUCT_BEGIN( "Server" );
            hub::Server server( port );
            CONSTRUCT_END( "Server" );
            server.asyncRun();

            while ( !viewer.isConnected() && iTry < 20 ) {
                std::cout << "[test] waiting for viewer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 20 );
            assert( viewer.isConnected() );
            assert( nServerConnected == 1 );

            {
                // std::cout << "[test] ############################### outputStream start "
                // "###########################################"
                // << std::endl;

                // hub::sensor::OutputSensorT<Resolution> outputStream(
                // sensorSpec_ref, hub::output::OutputStream( FILE_NAME, port ) );
                assert( viewer.nStream() == 0 );
                assert( viewer.nStreaming() == 0 );
                CONSTRUCT_BEGIN( "OutputStream" );
                hub::output::OutputStream outputStream( FILE_NAME, port, "127.0.0.1", header_ref );
                CONSTRUCT_END( "OutputStream" );
                assert( viewer.nStream() == 1 );
                assert( viewer.nStreaming() == 1 );
                // outputStream.setRetain( true );
                // outputStream.write(header_ref);
                // outputStream.write( header_ref.data(), header_ref.size() );
                // outputStream.setRetain( false );

                // iTry = 0;
                // while ( viewer.nStreaming() != 1 && iTry < 10 ) {
                //     std::cout << "[test] waiting for outputStream connected" << std::endl;
                //     std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                //     ++iTry;
                // }
                // assert( iTry != 10 );
                // assert( viewer.nStreaming() == 1 );
                // assert( nNewStreamer == 1 );
                // std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                // hub::sensor::AcquisitionT<Resolution> acq;
                // auto acq    = outputSensor.acqMsg();
                // auto& start = acq.start();
                // auto& end   = acq.end();
                // // auto * data = acq.data();
                // auto& bgr8 = acq.get<hub::sensor::format::BGR8&>();
                for ( int i = 0; i < 10; ++i ) {
                    outputStream.write( i );
                    //     start  = i;
                    //     end    = i;
                    //     bgr8.b = i;
                    //     bgr8.g = i;
                    //     bgr8.r = i;
                    //     outputSensor << acq;
                    //     mtxPrint.lock();
                    //     std::cout << "[test] write acq : " << acq << std::endl;
                    //     mtxPrint.unlock();
                }
                // outputStream.write(data_ref, dataSize_ref);

                iTry = 0;
                while ( nNewData < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for acq received : " << nNewData << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( iTry != 10 );
                assert( nNewData == 10 );

                // std::cout << "[test] -------------------------------------------------> "
                //              "~OutputStream() ..."
                //           << std::endl;
                DESTRUCT_BEGIN( "OutputStream" );
            } // end outputStream
            DESTRUCT_END( "OutputStream" );
            // std::cout << "[test] -------------------------------------------------> "
            //              "~OutputStream() done"
            //           << std::endl;

            iTry = 0;
            while ( viewer.nStreaming() != 0 && iTry < 10 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 10 );
            assert( viewer.nStreaming() == 0 );
            assert( nDelStreamer == 1 );
            assert( nServerDisconnected == 0 );

            // std::cout << "[test] ############################### server end" << std::endl;
            DESTRUCT_BEGIN( "Server" );
        } // end server
        DESTRUCT_END( "Server" );

        iTry = 0;
        // while ( viewer.isConnected() && iTry < 20 ) {
        while ( nServerDisconnected == 0 && iTry < 20 ) {
            std::cout << "[test] waiting for viewer disconnected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTry;
        }
        assert( iTry != 20 );
        assert( !viewer.isConnected() );
        std::cout << "[test] nServerDisconnected : " << nServerDisconnected << std::endl;
        assert( nServerDisconnected == 1 );

        DESTRUCT_BEGIN( "Viewer" );
    } // end viewer
    DESTRUCT_END( "Viewer" );
    // std::cout << "[test] ############################### viewer end" << std::endl;
    // std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    // todo fix : SIGPIPE Exception
    TEST_END()
}

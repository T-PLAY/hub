
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <client/Viewer.hpp>

#include <sensor/OutputSensor.hpp>

TEST_CASE( "Viewer" ) {
    TEST_BEGIN()

    const int port = GET_RANDOM_PORT;

    using Resolution = hub::format::BGR8;
    hub::MetaData metaData;
    metaData["user"] = "gauthier";
    const hub::sensor::SensorSpec sensorSpec_ref {
        "sensorName", hub::make_matrix<Resolution>(), metaData };

    std::mutex mtxPrint;

    {

        hub::client::ViewerHandler viewerHandler;

        int nNewStreamer                     = 0;
        int nDelStreamer                     = 0;
        std::atomic<int> nServerNotFound     = 0;
        int nServerConnected                 = 0;
        std::atomic<int> nServerDisconnected = 0;
        int nNewAcq                          = 0;

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
        viewerHandler.onNewSensor = [&]( const std::string& streamName,
                                         const hub::sensor::SensorSpec& sensorSpec ) {
            std::cout << "[test-client-Viewer] onNewStream : " << streamName << ", " << sensorSpec
                      << std::endl;
            assert( streamName == FILE_NAME );
            assert( sensorSpec_ref == sensorSpec );
            ++nNewStreamer;
            return true;
        };
        hub::io::Memory memory;
        viewerHandler.onNewAcq = [&]( const std::string& streamName,
                                      const hub::sensor::Acquisition& acq ) {
            assert( streamName == FILE_NAME );
            assert( acq.getResolution() == hub::make_matrix<Resolution>() );

            assert( acq.getOffset<hub::format::BGR8>( 0 ) == 16 );
            const auto& bgr8  = acq.get<const hub::format::BGR8&>();
            const auto& start = acq.getStart();
            const auto& end   = acq.getEnd();
            assert( start == nNewAcq );
            assert( end == nNewAcq );
            assert( bgr8.b == nNewAcq );
            assert( bgr8.g == nNewAcq );
            assert( bgr8.r == nNewAcq );
            mtxPrint.lock();
            std::cout << "[test-client-Viewer] onNewAcq : " << acq << std::endl;
            mtxPrint.unlock();
            ++nNewAcq;
        };
        viewerHandler.onDelStream = [&]( const std::string& streamName ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-Viewer] onDelStream : " << streamName << std::endl;
            ++nDelStreamer;
        };
        viewerHandler.onSetProperty = []( const std::string& streamName,
                                          const std::string& objectName,
                                          int property,
                                          const hub::Any& value ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-Viewer] onSetProperty " << streamName << std::endl;
        };
        viewerHandler.onLogMessage = []( const std::string& logMessage ) {
            std::cout << "[test-client-Viewer] onLogMessage '" << logMessage << "'" << std::endl;
        };

        CONSTRUCT_BEGIN( "Viewer" );
        hub::client::Viewer viewer { FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port };
        CONSTRUCT_END( "Viewer" );

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
                assert( viewer.nStream() == 0 );
                assert( viewer.nStreaming() == 0 );
                CONSTRUCT_BEGIN( "OutputSensor" );
                hub::sensor::OutputSensor outputSensor( sensorSpec_ref, FILE_NAME, port );
                CONSTRUCT_END( "OutputSensor" );
                assert( viewer.nStream() == 1 );
                assert( viewer.nStreaming() == 1 );
                auto acq    = outputSensor.acqMsg();
                auto& bgr8  = acq.get<hub::format::BGR8&>();
                auto& start = acq.start();
                auto& end   = acq.end();

                for ( int i = 0; i < 10; ++i ) {
                    start  = i;
                    end    = i;
                    bgr8.r = i;
                    bgr8.g = i;
                    bgr8.b = i;
                    outputSensor << acq;
                    mtxPrint.lock();
                    std::cout << "[test] write acq : " << acq << std::endl;
                    mtxPrint.unlock();
                }

                iTry = 0;
                while ( nNewAcq < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for 10 acqs received : " << nNewAcq << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( iTry != 10 );
                assert( nNewAcq == 10 );

                DESTRUCT_BEGIN( "OutputStream" );
            } // end outputStream
            DESTRUCT_END( "OutputStream" );

            iTry = 0;
            while ( viewer.nStream() != 0 && iTry < 10 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 10 );
            assert( viewer.nStream() == 0 );
            assert( viewer.nStreaming() == 0 );
            assert( nDelStreamer == 1 );
            assert( nServerDisconnected == 0 );

            DESTRUCT_BEGIN( "Server" );
        } // end server
        DESTRUCT_END( "Server" );

        iTry = 0;
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

    TEST_END()
}

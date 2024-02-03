
#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <native/Native.hpp>
#include <server/Server.hpp>

/// \file

TEST_CASE( "Native Viewer stream" ) {
    TEST_BEGIN()

    const int port = GET_RANDOM_PORT;

    static const hub::io::Header header_ref { sizeof( int ) };

    {

        static int nNewStreamer                     = 0;
        static int nDelStreamer                     = 0;
        static std::atomic<int> nServerNotFound     = 0;
        static int nServerConnected                 = 0;
        static std::atomic<int> nServerDisconnected = 0;
        static int nNewData                         = 0;

        auto onServerNotFound = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerNotFound : " << ipv4 << " " << port
                      << std::endl;
            ++nServerNotFound;
        };
        auto onServerConnected = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerConnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerConnected;
        };
        auto onServerDisconnected = []( const char* ipv4, int port ) {
            std::cout << "[test-client-NativeViewer] onServerDisconnected : " << ipv4 << " " << port
                      << std::endl;
            ++nServerDisconnected;
        };
        auto onNewStream = []( const char* streamName, const hub::io::Header* header ) {
            std::cout << "[test-client-NativeViewer] onNewStream : " << streamName << ", " << header
                      << std::endl;
            assert( streamName == FILE_NAME );
            assert( header_ref == *header );
            ++nNewStreamer;
            return true;
        };
#ifndef HUB_NON_BUILD_SENSOR
        auto onNewSensor = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
            assert( false );
            std::cout << "[test-client-NativeViewer] onNewStream : " << streamName << ", "
                      << sensorSpec << std::endl;
            assert( streamName == FILE_NAME );
            ++nNewStreamer;
            return true;
        };
#endif

        auto onNewData = []( const char* streamName, const hub::Datas_t* datas ) {
            assert( streamName == FILE_NAME );
            int a;
            memcpy( &a, datas->data(), datas->size() );
            assert( a == nNewData );
            std::cout << "[test-client-NativeViewer] onNewData : " << *datas << std::endl;
            ++nNewData;
        };

#ifndef HUB_NON_BUILD_SENSOR
        auto onNewAcq = []( const char* streamName, const hub::sensor::Acquisition* acq ) {
            assert( false );
        };
#endif

        auto onDelStream = []( const char* streamName ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-NativeViewer] onDelStream : " << streamName << std::endl;
            ++nDelStreamer;
        };
        auto onSetProperty = []( const char* streamName,
                                 const char* objectName,
                                 int property,
                                 const hub::Any* value ) {
            assert( streamName == FILE_NAME );
            std::cout << "[test-client-NativeViewer] onSetProperty " << streamName << std::endl;
        };
        auto onLogMessage = []( const char* logMessage ) {
            std::cout << "[test-client-NativeViewer] onLogMessage '" << logMessage << "'"
                      << std::endl;
        };

        auto* viewerHandler = hub::native::createViewerHandler( onServerNotFound,
                                                                onServerConnected,
                                                                onServerDisconnected,
                                                                onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                                                                onNewSensor,
#endif
                                                                onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                                                                onNewAcq,
#endif
                                                                onDelStream,
                                                                onSetProperty,
                                                                onLogMessage );

        CONSTRUCT_BEGIN( "NativeViewer" );
        auto* viewer =
            hub::native::createViewer( FILE_NAME.c_str(), viewerHandler, "127.0.0.1", port );
        CONSTRUCT_END( "NativeViewer" );

        assert( nServerDisconnected == 0 );
        assert( !hub::native::viewer_isConnected( viewer ) );
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

            while ( !hub::native::viewer_isConnected( viewer ) && iTry < 20 ) {
                std::cout << "[test] waiting for viewer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 20 );
            assert( hub::native::viewer_isConnected( viewer ) );
            assert( nServerConnected == 1 );

            {
                assert( hub::native::viewer_nStream( viewer ) == 0 );
                assert( hub::native::viewer_nStreaming( viewer ) == 0 );
                CONSTRUCT_BEGIN( "OutputStream" );
                auto* outputStream = hub::native::createOutputStream(
                    &header_ref, FILE_NAME.c_str(), port, "127.0.0.1" );
                CONSTRUCT_END( "OutputStream" );
                assert( hub::native::viewer_nStream( viewer ) == 1 );
                assert( hub::native::viewer_nStreaming( viewer ) == 1 );

                for ( int i = 0; i < 10; ++i ) {
                    hub::native::outputStream_write_int( outputStream, i );
                }

                iTry = 0;
                while ( nNewData < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for 10 acqs received : " << nNewData << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                assert( iTry != 10 );
                assert( nNewData == 10 );

                hub::native::freeOutputStream( outputStream );

                DESTRUCT_BEGIN( "OutputStream" );
            } // end outputStream
            DESTRUCT_END( "OutputStream" );

            iTry = 0;
            while ( hub::native::viewer_nStream( viewer ) != 0 && iTry < 10 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            assert( iTry != 10 );
            assert( hub::native::viewer_nStream( viewer ) == 0 );
            assert( hub::native::viewer_nStreaming( viewer ) == 0 );
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
        assert( !hub::native::viewer_isConnected( viewer ) );
        std::cout << "[test] nServerDisconnected : " << nServerDisconnected << std::endl;
        assert( nServerDisconnected == 1 );

        hub::native::freeViewer( viewer );

        DESTRUCT_BEGIN( "NativeViewer" );
    } // end viewer
    DESTRUCT_END( "NativeViewer" );

    TEST_END()
}

// TEST_CASE( "Native test" ) {

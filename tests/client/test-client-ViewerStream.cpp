
#include "io/test_io_common.hpp"
#include "test_common.hpp"

#include <client/Viewer.hpp>

#include <core/io/Memory.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "Viewer stream" ) {
    TEST_BEGIN()

    const int port = GET_RANDOM_PORT;

    const hub::io::Header header_ref { sizeof( int ) };

    {

        hub::client::ViewerHandler viewerHandler;

        std::atomic<int> nNewStreamer        = 0;
        std::atomic<int> nDelStreamer        = 0;
        std::atomic<int> nServerNotFound     = 0;
        std::atomic<int> nServerConnected    = 0;
        std::atomic<int> nServerDisconnected = 0;
        std::atomic<int> nNewData            = 0;

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
                                         const hub::io::Header& header ) {
            std::cout << "[test-client-Viewer] onNewStream : " << streamName << ", " << header
                      << std::endl;
            CHECK( streamName == FILE_NAME );
            CHECK( header_ref == header );
            ++nNewStreamer;
            return true;
        };
        hub::io::Memory memory;
        viewerHandler.onNewData = [&]( const std::string& streamName, const hub::Datas_t& datas ) {
            CHECK( streamName == FILE_NAME );
            int a;
            memory.write( datas.data(), datas.size() );
            memory.read( a );
            CHECK( a == nNewData );
            std::cout << "[test-client-Viewer] onNewData : " << datas << std::endl;
            ++nNewData;
        };
        viewerHandler.onDelStream = [&]( const std::string& streamName ) {
            CHECK( streamName == FILE_NAME );
            std::cout << "[test-client-Viewer] onDelStream : " << streamName << std::endl;
            ++nDelStreamer;
        };
        viewerHandler.onSetProperty = []( const std::string& streamName,
                                          const std::string& objectName,
                                          int property,
                                          const hub::Any& value ) {
            CHECK( streamName == FILE_NAME );
            std::cout << "[test-client-Viewer] onSetProperty " << streamName << std::endl;
        };
        viewerHandler.onLogMessage = []( const std::string& logMessage ) {
            std::cout << "[test-client-Viewer] onLogMessage '" << logMessage << "'" << std::endl;
        };

        CONSTRUCT_BEGIN( "Viewer" );
        hub::client::Viewer viewer { FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port };
        CONSTRUCT_END( "Viewer" );

        CHECK( nServerDisconnected == 0 );
        CHECK( !viewer.isConnected() );
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

            while ( ( !viewer.isConnected() || nServerConnected == 0 ) && iTry < 20 ) {
                std::cout << "[test] waiting for viewer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            CHECK( iTry < 20 );
            CHECK( viewer.isConnected() );
            CHECK( nServerConnected == 1 );

            {
                CHECK( viewer.nStream() == 0 );
                CHECK( viewer.nStreaming() == 0 );
                CONSTRUCT_BEGIN( "OutputStream" );
                hub::output::OutputStream outputStream( header_ref, FILE_NAME, port, "127.0.0.1" );
                CONSTRUCT_END( "OutputStream" );
                iTry = 0;
                while ( viewer.nStream() == 0 && iTry < 20 ) {
                    std::cout << "[test] waiting for viwer nStream == 1 : " << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                CHECK( iTry < 20 );
                CHECK( viewer.nStream() == 1 );
                CHECK( viewer.nStreaming() == 1 );

                for ( int i = 0; i < 10; ++i ) {
                    outputStream.write( i );
                }

                iTry = 0;
                while ( nNewData < 10 && iTry < 10 ) {
                    std::cout << "[test] waiting for 10 acqs received : " << nNewData << std::endl;
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    ++iTry;
                }
                CHECK( iTry != 10 );
                CHECK( nNewData == 10 );

                DESTRUCT_BEGIN( "OutputStream" );
            } // end outputStream
            DESTRUCT_END( "OutputStream" );

            iTry = 0;
            while ( ( viewer.nStream() != 0 || nDelStreamer == 0 ) && iTry < 20 ) {
                std::cout << "[test] waiting for outputStream disconnected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ++iTry;
            }
            CHECK( iTry != 20 );
            CHECK( viewer.nStream() == 0 );
            CHECK( viewer.nStreaming() == 0 );
            CHECK( nDelStreamer == 1 );
            CHECK( nServerDisconnected == 0 );

            DESTRUCT_BEGIN( "Server" );
        } // end server
        DESTRUCT_END( "Server" );

        iTry = 0;
        while ( nServerDisconnected == 0 && iTry < 20 ) {
            std::cout << "[test] waiting for viewer disconnected" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            ++iTry;
        }
        CHECK( iTry != 20 );
        CHECK( !viewer.isConnected() );
        std::cout << "[test] nServerDisconnected : " << nServerDisconnected << std::endl;
        CHECK( nServerDisconnected == 1 );

        DESTRUCT_BEGIN( "Viewer" );
    } // end viewer
    DESTRUCT_END( "Viewer" );

    TEST_END()
}

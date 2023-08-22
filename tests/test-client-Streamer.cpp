#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <client/Streamer.hpp>
//#include <server/Server.hpp>

#include <io/Stream.hpp>

TEST_CASE( "Streamer" ) {
//    const int port         = GET_RANDOM_PORT;
    const std::string ipv4 = "127.0.0.1";
    const int port         = hub::io::s_serverDefaultPort;

    constexpr int delay = 0;

    {
        hub::client::Streamer streamer( ipv4, port );
        std::cout << "[Test] ############################### streamer start" << std::endl;

        const hub::Resolution resolution( { { 1 }, hub::Format::BGR8 } );
        const hub::SensorSpec sensorSpec( "sensorName", { resolution } );

        unsigned char data[3] { 1, 2, 3 };
        hub::Acquisition acq =
            std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );
        std::vector<hub::Acquisition> acqs;
        acqs.push_back( acq.clone() );

        streamer.addStream( "streamName", sensorSpec, std::move( acqs ) );
        std::cout << "[Test] ############################### streamer add stream" << std::endl;

        assert( !streamer.isConnected() );
        CHECK( !streamer.isConnected() );

        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        {

//            hub::Server server( port );
//            std::cout << "[Test] ############################### server start" << std::endl;
//            server.setMaxClients( 1 );
//            server.asyncRun();

            while ( !streamer.isConnected() ) {
                std::cout << "[test] waiting for streamer connected" << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            }
            assert( streamer.isConnected() );
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            CHECK( streamer.isConnected() );

            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        }
        std::cout << "[Test] ############################### server end" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        streamer.newAcquisition( "streamName", acq );

//        while ( streamer.isConnected() ) {
//            std::cout << "[test] waiting for streamer disconnected" << std::endl;
//            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
//        }
//        assert( !streamer.isConnected() );
//        CHECK( !streamer.isConnected() );
    }
    std::cout << "[Test] ############################### streamer end" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

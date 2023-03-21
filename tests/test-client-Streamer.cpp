#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <server/Server.hpp>
#include <client/Streamer.hpp>

TEST_CASE( "Streamer" ) {
    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort(__FILE_NAME__);

    constexpr int delay = 100;

    {
        hub::client::Streamer streamer( ipv4, port );

        const hub::Resolution resolution( { { 1 }, hub::Format::BGR8 } );
        const hub::SensorSpec sensorSpec( "sensorName", { resolution } );

        unsigned char data[3] { 1, 2, 3 };
        hub::Acquisition acq =
            std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );

        streamer.addStream( "streamName", sensorSpec );
        CHECK( !streamer.isConnected() );

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        {

            std::cout << "[Test] ############################### server start" << std::endl;
            hub::server::Server server( port );
            server.setMaxClients( 1 );
            server.asyncRun();
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            CHECK( !streamer.isConnected() );
            streamer.newAcquisition( "streamName", acq );
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            CHECK( streamer.isConnected() );
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

            server.detach();
            std::cout << "[Test] ############################### server end" << std::endl;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        CHECK( streamer.isConnected() );
    }
    std::cout << "[Test] ############################### streamer end" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

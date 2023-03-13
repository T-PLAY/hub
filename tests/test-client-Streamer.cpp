#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <Server.hpp>
#include <client/Streamer.hpp>

// int main() {
TEST_CASE( "Streamer" ) {
    const std::string ipv4 = "127.0.0.1";
//    srand( (unsigned)time( NULL ) );
//    const int port = rand() % 65535;
    const int port = getRandomPort();

//    {
//        // startConstruction
//        // link to local server at port 4042
//        hub::client::Streamer streamer( ipv4, port );
//        // endConstruction

//        const hub::Resolution resolution( { { 1 }, hub::Format::BGR8 } );
//        const hub::SensorSpec sensorSpec( "sensorName", { resolution } );

//        streamer.addStream( "hello", sensorSpec );

//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//        {

//            std::cout << "[Test] ############################### server start" << std::endl;
//            Server server( port );
//            server.setMaxClients( 1 );
//            server.asyncRun();
//            std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

//            unsigned char data[3] { 1, 2, 3 };
//            hub::Acquisition acq =
//                std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );

//            streamer.newAcquisition( "hello", std::move( acq ) );
//            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//            std::cout << "[Test] ############################### server end" << std::endl;
//        }
//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
//    }
//    std::cout << "[Test] ############################### streamer end" << std::endl;
//    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
}

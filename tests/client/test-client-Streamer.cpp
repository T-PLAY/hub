#include "test_common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <client/Streamer.hpp>
#include <server/Server.hpp>

#include <io/Stream.hpp>

TEST_CASE( "Streamer" ) {

    // hub::sensor::SensorSpec::MetaData metaData;
    // metaData["parent"] = "parentName";

    // using Resolution      = hub::sensor::format::BGR8;
    // const auto resolution = hub::make_matrix<Resolution>();

    // const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );

    // // std::vector<hub::sensor::AcquisitionT<Resolution>> acqs;
    // std::vector<hub::sensor::Acquisition> acqs;

    // hub::sensor::Acquisition acq = hub::sensor::make_acquisition( resolution );
    // auto& start                  = acq.start();
    // auto& end                    = acq.end();
    // auto& bgr8                   = acq.get<hub::sensor::format::BGR8&>();
    // for ( int i = 0; i < 10; ++i ) {
    //     start  = i;
    //     end    = i;
    //     bgr8.b = i;
    //     bgr8.g = i;
    //     bgr8.r = i;
    //     acqs.push_back( acq.clone() );
    // }

    // ////////////////////////////////

    // const int port         = GET_RANDOM_PORT;
    // const std::string ipv4 = "127.0.0.1";

    // hub::client::Streamer streamer( ipv4, port );
    // assert( !streamer.isConnected() );

    // streamer.addStream( FILE_NAME, sensorSpec, std::move( acqs ) );

    // {
    //     hub::Server server( port );
    //     server.asyncRun();

    //     int iTry = 0;
    //     while ( !streamer.isConnected() && iTry < 10 ) {
    //         std::cout << "[test] waiting for streamer connected" << std::endl;
    //         std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //         ++iTry;
    //     }
    //     CHECK( streamer.isConnected() );
    //     assert( streamer.isConnected() );

    //     streamer.newAcquisition( FILE_NAME, acq );

    // } // end server
    // std::cout << "[test] server ended" << std::endl;

    // while ( streamer.isConnected() ) {
    //     std::cout << "[test] waiting for streamer disconnected" << std::endl;
    //     std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    //     // streamer.newAcquisition( FILE_NAME, acq );
    // }
    // assert( !streamer.isConnected() );
    // CHECK( !streamer.isConnected() );

    // // constexpr int delay = 0;

    // //     {
    // //         //        hub::client::Streamer streamer( ipv4, port );
    // //         hub::client::Streamer streamer;
    // //         std::cout << "[Test] ############################### streamer start" << std::endl;

    // //         const hub::sensor::Resolution resolution( { { 1 }, hub::sensor::Format::BGR8 } );
    // //         const hub::sensor::SensorSpec sensorSpec( "sensorName", { resolution } );

    // //         unsigned char data[3] { 1, 2, 3 };
    // //         hub::sensor::Acquisition acq =
    // //             std::move( hub::sensor::Acquisition( 0, 1 ) << hub::Measure( data, 3, resolution
    // //             ) );
    // //         std::vector<hub::sensor::Acquisition> acqs;
    // //         acqs.push_back( acq.clone() );

    // //         streamer.addStream( FILE_NAME, sensorSpec, std::move( acqs ) );
    // //         std::cout << "[Test] ############################### streamer add stream" <<
    // //         std::endl;

    // //         assert( !streamer.isConnected() );
    // //         CHECK( !streamer.isConnected() );

    // // //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    // //         {

    // //             //            hub::Server server( port );
    // //             //            std::cout << "[Test] ############################### server start"
    // //             <<
    // //             //            std::endl; server.setMaxClients( 1 ); server.asyncRun();

    // //             int iTryConnect = 0;
    // //             while ( !streamer.isConnected() && iTryConnect < 10 ) {
    // // //        while (! streamer.isConnected()) {
    // //                 std::cout << "[test] waiting for streamer connected" << std::endl;
    // //                 std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    // //                 ++iTryConnect;
    // //             }
    // //             CHECK( streamer.isConnected() );
    // //             assert( streamer.isConnected() );
    // // //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    // // //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    // //         }
    // //         std::cout << "[Test] ############################### server end" << std::endl;
    // // //        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

    // //         streamer.newAcquisition( FILE_NAME, acq );

    // //         //        while ( streamer.isConnected() ) {
    // //         //            std::cout << "[test] waiting for streamer disconnected" << std::endl;
    // //         //            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    // //         //        }
    // //         //        assert( !streamer.isConnected() );
    // //         //        CHECK( !streamer.isConnected() );
    // //     }
    // //     std::cout << "[Test] ############################### streamer end" << std::endl;
    // //    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

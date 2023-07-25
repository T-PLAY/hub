#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test : inputSensorLogoutWithBusyOutputSensor" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    constexpr int delay = 0;

    {
        hub::OutputSensor outputSensor(
            hub::SensorSpec( "sensorName", { { { 1 }, hub::Format::BGR8 } } ),
            "streamName",
            hub::net::ClientSocket( ipv4, port ) );
        std::cout << "[test] outputSensor created" << std::endl;

        std::thread thread;
        bool killThread = false;
        thread          = std::thread( [&outputSensor, &killThread]() {
            try {
                int iAcq = 0;
                while ( !killThread ) {
                    unsigned char data[3] = { 0, 1, 2 };
                    outputSensor << ( hub::Acquisition( iAcq, iAcq ) << hub::data::Measure(
                                          data, 3, { { 1 }, hub::Format::BGR8 } ) );
                    ++iAcq;
                }
            }
            catch ( std::exception& ex ) {
                std::cout << "[test] catch exception : " << ex.what() << std::endl;
            }
        } );

        {

            hub::io::InputStream inputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
            hub::InputSensor inputSensor( std::move( inputStream ) );
            assert( !inputSensor.getSpec().isEmpty() );

            for ( int i = 0; i < 10; ++i ) {
                hub::Acquisition acq;
                inputSensor >> acq;
                std::cout << "[test] receive acq: " << acq << std::endl;
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        }
        std::cout << "[test] end inputSensor" << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        std::cout << "[test] kill thread" << std::endl;
        killThread = true;
        assert( thread.joinable() );
        thread.join();
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
    }
    std::cout << "[test] end outputSensor" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

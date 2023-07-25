#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <server/Server.hpp>

TEST_CASE( "OutputSensor with closed server" ) {
    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    constexpr int delay = 0;

    {

        const hub::Resolution resolution( { { 1 }, hub::Format::BGR8 } );
        const hub::SensorSpec sensorSpec( "sensorName", { resolution } );

        unsigned char data[3] { 1, 2, 3 };
        hub::Acquisition acq =
            std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data, 3, resolution ) );
        std::vector<hub::Acquisition> acqs;
        acqs.push_back( acq.clone() );

        hub::OutputSensor* outputSensor = nullptr;

        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        {

            hub::server::Server server( port );
            std::cout << "[Test] ############################### server start" << std::endl;
            server.setMaxClients( 1 );
            server.asyncRun();

            outputSensor = new hub::OutputSensor(
                sensorSpec, "streamName", hub::net::ClientSocket( ipv4, port ) );

            std::cout << "[Test] ############################### output sensor start" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
            *outputSensor << acq;
            std::cout << "[Test] ############################### sended acq" << std::endl;
            std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
        }
        std::cout << "[Test] ############################### server end" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        delete outputSensor;
        std::cout << "[Test] ############################### output sensor end" << std::endl;
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );
}

#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

//#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test" ) {

//    const std::string ipv4 = "127.0.0.1";
//    const int port         = GET_RANDOM_PORT;

//    hub::Server server( port );
//    server.setMaxClients( 2 );
//    server.asyncRun();

    {
        std::unique_ptr<hub::sensor::InputSensor> inputSensor;

        std::thread thread;

        {
            hub::sensor::OutputSensor outputSensor(
                hub::sensor::SensorSpec( "sensorName", { { { 1 }, hub::sensor::Format::BGR8 } } ),
                hub::output::OutputStream(FILE_NAME)
//                "streamName",
//                hub::net::ClientSocket( ipv4, port ) );
//                ipv4, port
                );
            std::cout << "[test] outputSensor created" << std::endl;

            inputSensor = std::make_unique<hub::sensor::InputSensor>(
//                hub::io::InputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
//                hub::input::InputStreamServer( "streamName", ipv4, port ) );
                hub::input::InputStream( FILE_NAME ) );
            std::cout << "[test] inputSensor created" << std::endl;

            thread = std::thread( [&inputSensor]() {
                try {
                    while ( true ) {
                        hub::sensor::Acquisition acq;
                        *inputSensor >> acq;
                        std::cout << "[test] receive acq: " << acq << std::endl;
                    }
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test] catch exception : " << ex.what() << std::endl;
                }
            } );

            std::cout << "inputSensor2 created" << std::endl;
        }
        std::cout << "[test] end outputSensor" << std::endl;

        assert( thread.joinable() );
        thread.join();
    }
    std::cout << "[test] end inputSensors" << std::endl;
}

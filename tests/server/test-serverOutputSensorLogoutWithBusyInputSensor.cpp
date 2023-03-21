#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort( __FILE_NAME__ );

    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    {
        std::unique_ptr<hub::InputSensor> inputSensor;
//        std::unique_ptr<hub::InputSensor> inputSensor2;

        std::thread thread;
//        std::thread thread2;

        {
            hub::OutputSensor outputSensor(
                hub::SensorSpec( "sensorName", { { { 1 }, hub::Format::BGR8 } } ),
                hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
            std::cout << "[test] outputSensor created" << std::endl;

//            for (int i = 0; i <10; ++i) {
//                        unsigned char data[3] = { 0, 1, 2 };
//                        outputSensor
//                            << ( hub::Acquisition( i, i )
//                                 << hub::data::Measure( data, 3, { { 1 }, hub::Format::BGR8 } ) );
//            }


//            hub::OutputSensor outputSensor2(
//                hub::SensorSpec( "sensorName2", { { { 1 }, hub::Format::BGR8 } } ),
//                hub::io::OutputStream( "streamName2", hub::net::ClientSocket( ipv4, port ) ) );
//            std::cout << "outputSensor2 created" << std::endl;

            inputSensor = std::make_unique<hub::InputSensor>(
                hub::io::InputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
            std::cout << "[test] inputSensor created" << std::endl;

            thread = std::thread( [&inputSensor]() {
                try {
                    while ( true ) {
                        hub::Acquisition acq;
                        *inputSensor >> acq;
                        std::cout << "[test] receive acq: " << acq << std::endl;
                    }
                }
                catch ( std::exception& ex ) {
                    std::cout << "[test] catch exception : " << ex.what() << std::endl;
                }
            } );

//            for (int i = 0; i <10; ++i) {
//                        unsigned char data[3] = { 0, 1, 2 };
//                        outputSensor
//                            << ( hub::Acquisition( i, i )
//                                 << hub::data::Measure( data, 3, { { 1 }, hub::Format::BGR8 } ) );
//            }


            //        hub::InputSensor inputSensor2(
//            inputSensor2 = std::make_unique<hub::InputSensor>(
//                hub::io::InputStream( "streamName2", hub::net::ClientSocket( ipv4, port ) ) );
            std::cout << "inputSensor2 created" << std::endl;
        }
        std::cout << "[test] end outputSensor" << std::endl;

        assert(thread.joinable());
        thread.join();

        //        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
    std::cout << "[test] end inputSensors" << std::endl;


    //    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
}

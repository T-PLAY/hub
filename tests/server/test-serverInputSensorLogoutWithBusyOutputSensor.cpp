#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamViewerClient.hpp>

TEST_CASE( "Server test : inputSensorLogoutWithBusyOutputSensor" ) {
//int main() {

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();

    constexpr int delay = 0;

    {
        hub::OutputSensor outputSensor(
            hub::SensorSpec( "sensorName", { { { 1 }, hub::Format::BGR8 } } ),
//            hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );
            "streamName", hub::net::ClientSocket( ipv4, port ) );
        std::cout << "[test] outputSensor created" << std::endl;
        //        std::unique_ptr<hub::InputSensor> inputSensor;
        //        std::unique_ptr<hub::InputSensor> inputSensor2;

        std::thread thread;
        bool killThread = false;
        thread = std::thread( [&outputSensor, &killThread]() {
            try {
                int iAcq = 0;
                while ( ! killThread ) {
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
        //        std::thread thread2;

        {

            //            hub::OutputSensor outputSensor2(
            //                hub::SensorSpec( "sensorName2", { { { 1 }, hub::Format::BGR8 } } ),
            //                hub::io::OutputStream( "streamName2", hub::net::ClientSocket( ipv4,
            //                port ) ) );
            //            std::cout << "outputSensor2 created" << std::endl;
            hub::io::InputStream inputStream( "streamName", hub::net::ClientSocket( ipv4, port ) );
            hub::InputSensor inputSensor( std::move( inputStream ) );
//            hub::io::Input & input = inputSensor.getInput();
//            assert(!input.getSensorSpec().isEmpty());
            assert(! inputSensor.getSpec().isEmpty());
//            std::cout << "[test] inputSensor created" << std::endl;

//                    unsigned char data[3] = { 0, 1, 2 };
//                    outputSensor << ( hub::Acquisition( 0, 0 ) << hub::data::Measure(
//                                          data, 3, { { 1 }, hub::Format::BGR8 } ) );

            for ( int i = 0; i < 10; ++i ) {
                hub::Acquisition acq;
                inputSensor >> acq;
                std::cout << "[test] receive acq: " << acq << std::endl;
            }
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

//            std::cout << "inputSensor2 created" << std::endl;
        }
        std::cout << "[test] end inputSensor" << std::endl;

        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        std::cout << "[test] kill thread" << std::endl;
        killThread = true;
        assert( thread.joinable() );
        thread.join();
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

        //        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
    std::cout << "[test] end outputSensor" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( delay ) );

//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
}

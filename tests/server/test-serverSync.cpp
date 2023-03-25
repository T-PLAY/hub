#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <InputSensor.hpp>
//#include <io/InputSyncStream.hpp>
#include <OutputSensor.hpp>

#include <server/Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : sync" ) {

    const std::string ipv4 = "127.0.0.1";
//    srand( (unsigned)time( NULL ) );
//    const int port = rand() % 65535;
    const int port = GET_RANDOM_PORT;


    constexpr int nAcqs    = 5;
    constexpr int dataSize = 9;


    std::cout << "ref_acqs:" << std::endl;
    std::vector<hub::Acquisition> acqs;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 10;
        }
        acqs.emplace_back( iAcq * 10, iAcq * 10 );
        acqs.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                            dataSize,
                                            { { 3 }, hub::Format::BGR8 } );
        std::cout << acqs.back() << std::endl;
    }

    std::cout << "ref2_acqs:" << std::endl;
    std::vector<hub::Acquisition> acqs2;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs2.emplace_back( iAcq * 2, iAcq * 2 );
        acqs2.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                           dataSize,
                                           { { 3 }, hub::Format::BGR8 } );
        std::cout << acqs2.back() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::server::Server server( port );
    server.setMaxClients( 6 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            hub::SensorSpec { "sensorName", { { { 3 }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec = outputSensor.getSpec();
        CHECK( outputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec.getSensorName() == "sensorName" );
        CHECK( outputSensorSpec.getResolutions().size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::OutputSensor outputSensor2(
            hub::SensorSpec { "sensorName2", { { { 3 }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "stream2", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec2 = outputSensor2.getSpec();
        CHECK( outputSensorSpec2.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec2.getSensorName() == "sensorName2" );
        CHECK( outputSensorSpec2.getResolutions().size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::InputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.getSpec();
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream2 start" << std::endl;
        hub::InputSensor inputSensor2(
            hub::io::InputStream( "stream2", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec2 = inputSensor2.getSpec();
        std::cout << "[Test] inputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream3 start" << std::endl;
        hub::InputSensor inputSensor3(
            hub::io::InputSyncStream( "stream", "stream2", ipv4, port) );

        const auto& inputSensorSpec3 = inputSensor3.getSpec();
        std::cout << "[Test] inputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### send acquisitions" << std::endl;
        for ( const auto& acq : acqs ) {
            outputSensor << acq;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        std::cout << "[Test] ############################### send acquisitions 2" << std::endl;
        for ( const auto& acq2 : acqs2 ) {
            outputSensor2 << acq2;
        }
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        std::cout << "[Test] ############################### compare " << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            hub::Acquisition acq;
            inputSensor >> acq;
//            auto acq = inputSensor.getAcq();
            std::cout << "[Test] acq = " << acq << std::endl;
            CHECK( acq == acqs[iAcq] );
        }
        std::cout << "[Test] ############################### compare2 " << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
            hub::Acquisition acq;
            inputSensor2 >> acq;
//            auto acq = inputSensor2.getAcq();
            std::cout << "[Test] acq2 = " << acq << std::endl;
            CHECK( acq == acqs2[iAcq] );
        }
        std::cout << "[Test] ############################### compare3 " << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            hub::Acquisition acq;
            inputSensor3 >> acq;
//            auto acq = inputSensor3.getAcq();
            std::cout << "[Test] acq3 = " << acq << std::endl;
            hub::Acquisition acqSync = acqs.at(iAcq).clone();
            acqSync << acqs2[iAcq * 5].getMeasures();
            CHECK( acq == acqSync);
        }
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

//    server.stop();
}

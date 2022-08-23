#include <catch2/catch_test_macros.hpp>

#include <IO/Client.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <Net/Server.hpp>
#include <OutputSensor.hpp>

#include <filesystem>

TEST_CASE( "Server test : sync" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 6001;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 5;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs.emplace_back( iAcq * 2, iAcq * 2, data, dataSize );
    }
    std::vector<hub::Acquisition> acqs2;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10;
        }
        acqs2.emplace_back( iAcq2 * 10, iAcq2 * 10, data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::net::Server server( port );
    server.setMaxClients( 5 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { 3 } },
            hub::io::Streamer( "stream", hub::net::ClientSocket( ipv4, port ) ) );

//        hub::io::Streamer(hub::net::ClientSocket(ipv4, port));

        auto& outputSensorSpec = outputSensor.spec;
        CHECK( outputSensorSpec.acquisitonSize == dataSize );
        CHECK( outputSensorSpec.sensorName == "sensorName" );
        CHECK( outputSensorSpec.dims.size() == 1 );
        CHECK( outputSensorSpec.dims.at( 0 ) == 3 );
        CHECK( outputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::OutputSensor outputSensor2(
            { "sensorName2", hub::SensorSpec::Format::BGR8, { 3 } },
            hub::io::Streamer( "master", hub::net::ClientSocket( ipv4, port ) ) );

        auto& outputSensorSpec2 = outputSensor2.spec;
        CHECK( outputSensorSpec2.acquisitonSize == dataSize );
        CHECK( outputSensorSpec2.sensorName == "sensorName2" );
        CHECK( outputSensorSpec2.dims.size() == 1 );
        CHECK( outputSensorSpec2.dims.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::StreamViewer( "stream", "master", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.spec;
        CHECK( inputSensorSpec.acquisitonSize == dataSize );
        CHECK( inputSensorSpec.sensorName == "sensorName" );
        CHECK( inputSensorSpec.dims.size() == 1 );
        CHECK( inputSensorSpec.dims.at( 0 ) == 3 );
        CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream2 start" << std::endl;
        hub::InputSensor inputSensor2(
            hub::io::StreamViewer( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec2 = inputSensor2.spec;
        CHECK( inputSensorSpec2.acquisitonSize == dataSize );
        CHECK( inputSensorSpec2.sensorName == "sensorName" );
        CHECK( inputSensorSpec2.dims.size() == 1 );
        CHECK( inputSensorSpec2.dims.at( 0 ) == 3 );
        CHECK( inputSensorSpec2.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] inputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream3 start" << std::endl;
        hub::InputSensor inputSensor3(
            hub::io::StreamViewer( "master", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec3 = inputSensor3.spec;
        CHECK( inputSensorSpec3.acquisitonSize == dataSize );
        CHECK( inputSensorSpec3.sensorName == "sensorName2" );
        CHECK( inputSensorSpec3.dims.size() == 1 );
        CHECK( inputSensorSpec3.dims.at( 0 ) == 3 );
        CHECK( inputSensorSpec3.format == hub::SensorSpec::Format::BGR8 );
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
            auto acq = inputSensor.getAcquisition();
            std::cout << "[Test] acq = " << acq << std::endl;
            CHECK( acq == acqs[iAcq * 5] );
        }
        std::cout << "[Test] ############################### compare2 " << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
            auto acq = inputSensor2.getAcquisition();
            std::cout << "[Test] acq2 = " << acq << std::endl;
            CHECK( acq == acqs[iAcq] );
        }
        std::cout << "[Test] ############################### compare3 " << std::endl;
        for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
            auto acq = inputSensor3.getAcquisition();
            std::cout << "[Test] acq3 = " << acq << std::endl;
            CHECK( acq == acqs2[iAcq] );
        }
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

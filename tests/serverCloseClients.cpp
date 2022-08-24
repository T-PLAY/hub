#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include <IO/Stream.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>


TEST_CASE( "Server test : close clients" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 7000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 2;
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
    Server server( port );
    server.setMaxClients( 8 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { 3 } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

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
            hub::io::OutputStream( "master", hub::net::ClientSocket( ipv4, port ) ) );

        auto& outputSensorSpec2 = outputSensor2.spec;
        CHECK( outputSensorSpec2.acquisitonSize == dataSize );
        CHECK( outputSensorSpec2.sensorName == "sensorName2" );
        CHECK( outputSensorSpec2.dims.size() == 1 );
        CHECK( outputSensorSpec2.dims.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        for ( int i = 0; i < 2; ++i ) {
            std::cout << "[Test] ############################### inputSensor(stream, master)"
                      << std::endl;
            {
                hub::InputSensor inputSensor( hub::io::InputStream(
                    "stream", "master", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.spec;
                CHECK( inputSensorSpec.acquisitonSize == dataSize );
                CHECK( inputSensorSpec.sensorName == "sensorName" );
                CHECK( inputSensorSpec.dims.size() == 1 );
                CHECK( inputSensorSpec.dims.at( 0 ) == 3 );
                CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq : acqs ) {
                    outputSensor << acq;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                std::cout << "[Test] ############################### send acquisitions 2"
                          << std::endl;
                for ( const auto& acq2 : acqs2 ) {
                    outputSensor2 << acq2;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                std::cout << "[Test] ############################### compare " << std::endl;
                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                    //            std::cout << "[Test] compare acq " << iAcq << std::endl;
                    auto acq = inputSensor.getAcquisition();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    //            CHECK( acq == acqs[iAcq] );
                    CHECK( acq == acqs[iAcq * 5] );
                }
            }
            outputSensor << acqs.front();
            outputSensor2 << acqs2.front();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### inputSensor(stream,)" << std::endl;
            {
                hub::InputSensor inputSensor(
                    hub::io::InputStream( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.spec;
                CHECK( inputSensorSpec.acquisitonSize == dataSize );
                CHECK( inputSensorSpec.sensorName == "sensorName" );
                CHECK( inputSensorSpec.dims.size() == 1 );
                CHECK( inputSensorSpec.dims.at( 0 ) == 3 );
                CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq : acqs ) {
                    outputSensor << acq;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                std::cout << "[Test] ############################### compare " << std::endl;
                for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
                    auto acq = inputSensor.getAcquisition();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    CHECK( acq == acqs[iAcq] );
                }
            }
            outputSensor << acqs.front();
            outputSensor2 << acqs2.front();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### inputSensor(master,)" << std::endl;
            {
                hub::InputSensor inputSensor(
                    hub::io::InputStream( "master", "", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.spec;
                CHECK( inputSensorSpec.acquisitonSize == dataSize );
                CHECK( inputSensorSpec.sensorName == "sensorName2" );
                CHECK( inputSensorSpec.dims.size() == 1 );
                CHECK( inputSensorSpec.dims.at( 0 ) == 3 );
                CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq2 : acqs2 ) {
                    outputSensor2 << acq2;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                std::cout << "[Test] ############################### compare " << std::endl;
                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                    auto acq = inputSensor.getAcquisition();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    CHECK( acq == acqs2[iAcq] );
                }
            }
            outputSensor << acqs.front();
            outputSensor2 << acqs2.front();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        }
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

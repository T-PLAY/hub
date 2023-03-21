#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <server/Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : speed test" ) {

    const std::string ipv4 = "127.0.0.1";
//    srand( (unsigned)time( NULL ) );
//    const int port = rand() % 65535;
    const int port = getRandomPort(__FILE_NAME__);

    constexpr int nAcqs       = 100;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    unsigned char* data       = new unsigned char[dataSize];

    std::vector<hub::Acquisition> acqs(nAcqs);

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq;
        }
        hub::Acquisition acq( iAcq, iAcq );
        acq << hub::data::Measure( reinterpret_cast<unsigned const char*>( data ),
                                   dataSize,
                                   { { width, height }, hub::Format::BGR8 } );
        acqs.at(iAcq) = std::move(acq);
    }
    delete[] data;

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::server::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec( "sensorName", { { { width, height }, hub::Format::BGR8 } } ),
                hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::InputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.getSpec();
        CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( inputSensorSpec.getSensorName() == "sensorName" );
        CHECK( inputSensorSpec.getResolutions().size() == 1 );
        CHECK( inputSensorSpec.getResolutions()[0].first.size() == 2 );
        CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == width );
        CHECK( inputSensorSpec.getResolutions()[0].first.at( 1 ) == height );
        CHECK( inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### send acquisitions" << std::endl;
        const auto& start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < nAcqs; ++i ) {
            outputSensor << acqs.at(i);
            auto acq = inputSensor.getAcq();
            CHECK(acq == acqs.at(i));
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        const auto& duration =
            std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
        const auto& bytes               = dataSize * nAcqs;
        const auto& bytesPerSeconds     = 1000 * bytes / duration;
        const auto& megaBytesPerSeconds = bytesPerSeconds / 1000'000;

        std::cout << "Mega byte wrote : " << bytes / 1000'000 << " Mo" << std::endl;
        std::cout << "Mega byte per second : " << megaBytesPerSeconds << " Mo/s" << std::endl;
        CHECK( megaBytesPerSeconds >= 800 ); // server IRIT
        // CHECK( megaBytesPerSeconds >= 2700 ); // home linux
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

//    server.stop();
}

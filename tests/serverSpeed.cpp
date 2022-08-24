#include <catch2/catch_test_macros.hpp>

#include <IO/NetClient.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <Net/Server.hpp>
#include <OutputSensor.hpp>

#include <filesystem>

TEST_CASE( "Server test : speed test" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 8000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 100;
    constexpr int width    = 1920;
    constexpr int height   = 1080;
    constexpr size_t dataSize = width * height * 3;
    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq;
        }
        acqs.emplace_back( iAcq, iAcq, data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::net::Server server( port );
    server.setMaxClients( 2 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { width, height } },
            hub::io::Streamer( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::StreamViewer( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.spec;
        CHECK( inputSensorSpec.acquisitonSize == dataSize );
        CHECK( inputSensorSpec.sensorName == "sensorName" );
        CHECK( inputSensorSpec.dims.size() == 2 );
        CHECK( inputSensorSpec.dims.at( 0 ) == width );
        CHECK( inputSensorSpec.dims.at( 1 ) == height );
        CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### send acquisitions" << std::endl;
        const auto& start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < nAcqs; ++i ) {
            outputSensor << acqs[i];
            auto acq = inputSensor.getAcquisition();
            CHECK( acq == acqs[i] );
        }
        const auto& end = std::chrono::high_resolution_clock::now();
        const auto& duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        const auto& bytes = dataSize * nAcqs;
        const auto& bytesPerSeconds = 1000 * bytes / duration;
        const auto& megaBytesPerSeconds = bytesPerSeconds / 1000'000;

        std::cout << "Mega byte wrote : " << bytes / 1000'000 << " Mo" << std::endl;
        std::cout << "Mega byte per second : " << megaBytesPerSeconds << " Mo/s" << std::endl;
        CHECK(megaBytesPerSeconds >= 900);
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

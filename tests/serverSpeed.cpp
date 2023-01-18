#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : speed test" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 7000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs       = 100;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
    constexpr size_t dataSize = width * height * 3;
    unsigned char* data       = new unsigned char[dataSize];

    for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq;
        }
        acqs.emplace_back( iAcq, iAcq ) << hub::Measure(
            (unsigned const char*)data, dataSize, { { width, height }, hub::Format::BGR8 } );
    }
    delete[] data;

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 2 );
    //    server.setAcqPing( false );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", { { { width, height }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::InputStream( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.m_spec;
        CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( inputSensorSpec.getSensorName() == "sensorName" );
        CHECK( inputSensorSpec.getResolutions().size() == 1 );
        CHECK( inputSensorSpec.getResolutions()[0].first.size() == 2 );
        CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == width );
        CHECK( inputSensorSpec.getResolutions()[0].first.at( 1 ) == height );
        CHECK( inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### send acquisitions" << std::endl;
        //        for ( int i = 0; i < nAcqs; ++i ) {
        //            outputSensor << acqs[i];
        //        }
        const auto& start = std::chrono::high_resolution_clock::now();
        for ( int i = 0; i < nAcqs; ++i ) {
            outputSensor << acqs[i];
            auto acq = inputSensor.getAcquisition();
            //            CHECK( acq == acqs[i] );
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
        // CHECK( megaBytesPerSeconds >= 2000 ); // home linux
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

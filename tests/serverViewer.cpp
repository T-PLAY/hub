#include <catch2/catch_test_macros.hpp>

#include <IO/NetClient.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <Net/Server.hpp>
#include <OutputSensor.hpp>

#include <filesystem>

TEST_CASE( "Server test : viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 9000;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs       = 100;
    constexpr int width       = 1920;
    constexpr int height      = 1080;
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
    server.setMaxClients( 3 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {

        auto onNewStreamer = [ipv4, port]( const std::string& sensorName,
                                           const hub::SensorSpec& sensorSpec ) {
            std::cout << "[Test] ############################### onNewStreamer" << std::endl;
            {
                hub::InputSensor inputSensor(
                    hub::io::StreamViewer( sensorName, "", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.spec;
                CHECK( inputSensorSpec.acquisitonSize == dataSize );
                CHECK( inputSensorSpec.sensorName == "sensorName" );
                CHECK( inputSensorSpec.dims.size() == 2 );
                CHECK( inputSensorSpec.dims.at( 0 ) == width );
                CHECK( inputSensorSpec.dims.at( 1 ) == height );
                CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### onNewStreamer end" << std::endl;
        };
        auto onDelStreamer = []( const std::string& sensorName ) {
            std::cout << "[Test] ############################### onDelStreamer" << std::endl;
        };
        auto viewer =
            hub::io::Viewer( hub::net::ClientSocket( ipv4, port ), onNewStreamer, onDelStreamer );
        std::cout << "[Test] ############################### viewer created" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;
            hub::OutputSensor outputSensor(
                { "sensorName", hub::SensorSpec::Format::BGR8, { width, height } },
                hub::io::Streamer( "stream", hub::net::ClientSocket( ipv4, port ) ) );
//            outputSensor << acqs[0];
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

//            {
//                hub::InputSensor inputSensor(
//                    hub::io::StreamViewer( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

//                const auto& inputSensorSpec = inputSensor.spec;
//                CHECK( inputSensorSpec.acquisitonSize == dataSize );
//                CHECK( inputSensorSpec.sensorName == "sensorName" );
//                CHECK( inputSensorSpec.dims.size() == 2 );
//                CHECK( inputSensorSpec.dims.at( 0 ) == width );
//                CHECK( inputSensorSpec.dims.at( 1 ) == height );
//                CHECK( inputSensorSpec.format == hub::SensorSpec::Format::BGR8 );
//            }
//            outputSensor << acqs[0];
//            outputSensor << acqs[0];
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            std::cout << "[Test] ############################### inputSensor end" << std::endl;
        }
        std::cout << "[Test] ############################### outputSensor end" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "[Test] ############################### viewer end" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    server.stop();
}

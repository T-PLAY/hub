#include "test-common.hpp"
#include <catch2/catch_test_macros.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <client/Viewer.hpp>

#include <server/Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = GET_RANDOM_PORT;

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
        acqs.emplace_back( iAcq, iAcq );
        acqs.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                           dataSize,
                                           { { width, height }, hub::Format::BGR8 } );
    }
    delete[] data;

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::server::Server server( port );
    server.setMaxClients( 4 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {

        auto onNewStreamer = [=]( const std::string& sensorName,
                                  const hub::SensorSpec& sensorSpec ) {
            std::cout << "[Test] ############################### onNewStreamer" << std::endl;
            {
                hub::InputSensor inputSensor(
                    hub::io::InputStream( sensorName, hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.getSpec();
                CHECK( inputSensorSpec.getAcquisitionSize() == dataSize );
                CHECK( inputSensorSpec.getSensorName() == "sensorName" );
                CHECK( inputSensorSpec.getResolutions().size() == 1 );
                CHECK( inputSensorSpec.getResolutions()[0].first.size() == 2 );
                CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == width );
                CHECK( inputSensorSpec.getResolutions()[0].first.at( 1 ) == height );
                CHECK( inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### onNewStreamer end" << std::endl;
            return true;
        };
        auto onDelStreamer = []( const std::string& sensorName,
                                 const hub::SensorSpec& sensorSpec ) {
            std::cout << "[Test] ############################### onDelStreamer" << std::endl;
        };
        auto onServerNotFound = []( const std::string& ipv4, int port ) {
            std::cout << "[example-viewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
        };
        auto onServerConnected = []( const std::string& ipv4, int port ) {
            std::cout << "[Test] ############################### onServerConnected" << std::endl;
        };
        auto onServerDisconnected = []( const std::string& ipv4, int port ) {
            std::cout << "[Test] ############################### onServerDisconnected" << std::endl;
        };
        auto onNewAcquisition = []( const std::string& sensorName, const hub::Acquisition& acq ) {
            std::cout << "[Test] ############################### onNewAcquisition" << std::endl;
        };
        auto onSetProperty = []( const std::string& streamName,
                                 const std::string& objectName,
                                 int property,
                                 const hub::Any& value ) {
            std::cout << "[example-viewer] onSetProperty " << streamName << std::endl;
        };
        hub::client::Viewer viewer { onNewStreamer,
                                     onDelStreamer,
                                     onServerNotFound,
                                     onServerConnected,
                                     onServerDisconnected,
                                     onNewAcquisition,
                                     onSetProperty,
                                     ipv4,
                                     port };
        std::cout << "[Test] ############################### viewer created" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;
            hub::OutputSensor outputSensor(
                hub::SensorSpec { "sensorName", { { { width, height }, hub::Format::BGR8 } } },
                "stream",
                hub::net::ClientSocket( ipv4, port ) );
            std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
        }
        std::cout << "[Test] ############################### outputSensor end" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "[Test] ############################### viewer end" << std::endl;
}

#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>
#include <Viewer.hpp>

#include <Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : viewer" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 9002;

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
        acqs.emplace_back( iAcq, iAcq );
        acqs.back() << hub::Measure( data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 3 );
    server.setAcqPing(false);
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {

        auto onNewStreamer = [=]( const std::string& sensorName,
                                           const hub::SensorSpec& sensorSpec ) {
            std::cout << "[Test] ############################### onNewStreamer" << std::endl;
            {
                hub::InputSensor inputSensor(
                    hub::io::InputStream( sensorName, "", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
                CHECK( inputSensorSpec.m_sensorName == "sensorName" );
                CHECK( inputSensorSpec.m_resolutions.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.size() == 2 );
                CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == width );
                CHECK( inputSensorSpec.m_resolutions[0].first.at( 1 ) == height );
                CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
            }
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### onNewStreamer end" << std::endl;
        };
        auto onDelStreamer = []( const std::string& sensorName,
                                 const hub::SensorSpec& sensorSpec ) {
            std::cout << "[Test] ############################### onDelStreamer" << std::endl;
        };
        auto onServerConnected = []() {
            std::cout << "[Test] ############################### onServerConnected" << std::endl;
        };
        auto onServerDisconnected = []() {
            std::cout << "[Test] ############################### onServerDisconnected" << std::endl;
        };
        auto viewer = hub::Viewer( ipv4, port, onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected );
        std::cout << "[Test] ############################### viewer created" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        {
            std::cout << "[Test] ############################### outputSensor start" << std::endl;
            hub::OutputSensor outputSensor(
                { "sensorName", { { { width, height }, hub::SensorSpec::Format::BGR8 } } },
                hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );
            //            outputSensor << acqs[0];
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

            //            {
            //                hub::InputSensor inputSensor(
            //                    hub::io::StreamViewer( "stream", "", hub::net::ClientSocket( ipv4,
            //                    port ) ) );

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
            //            std::cout << "[Test] ############################### inputSensor end" <<
            //            std::endl;
        }
        std::cout << "[Test] ############################### outputSensor end" << std::endl;
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "[Test] ############################### viewer end" << std::endl;
    std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );

    server.stop();
}

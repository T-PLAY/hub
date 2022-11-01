#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

TEST_CASE( "Server test : close clients" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 6002;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 2;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs.emplace_back( iAcq * 2, iAcq * 2 );
        acqs.back() << hub::Measure( data, dataSize );
    }
    std::vector<hub::Acquisition> acqs2;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10;
        }
        acqs2.push_back( hub::Acquisition( iAcq2 * 10, iAcq2 * 10 ) );
        acqs2.back() << hub::Measure( data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 8 );
    server.setAcqPing( false );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", { { { 3 }, hub::SensorSpec::Format::BGR8 } } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );


        auto& outputSensorSpec = outputSensor.m_spec;
        CHECK( outputSensorSpec.m_acquisitionSize == dataSize );
        CHECK( outputSensorSpec.m_sensorName == "sensorName" );
        CHECK( outputSensorSpec.m_resolutions.size() == 1 );
        CHECK( outputSensorSpec.m_resolutions[0].first.size() == 1 );
        CHECK( outputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::OutputSensor outputSensor2(
            { "sensorName2", { { { 3 }, hub::SensorSpec::Format::BGR8 } } },
            hub::io::OutputStream( "master", hub::net::ClientSocket( ipv4, port ) ) );

        auto& outputSensorSpec2 = outputSensor2.m_spec;
        CHECK( outputSensorSpec2.m_acquisitionSize == dataSize );
        CHECK( outputSensorSpec2.m_sensorName == "sensorName2" );
        CHECK( outputSensorSpec2.m_resolutions.size() == 1 );
        CHECK( outputSensorSpec2.m_resolutions[0].first.size() == 1 );
        CHECK( outputSensorSpec2.m_resolutions[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        for ( int i = 0; i < 2; ++i ) {
            std::cout << "[Test] ############################### inputSensor(stream, master)"
                      << std::endl;
            {
                hub::InputSensor inputSensor( hub::io::InputStream(
                    "stream", "master", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
                CHECK( inputSensorSpec.m_sensorName == "sensorName" );
                CHECK( inputSensorSpec.m_resolutions.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
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
                    auto acq = inputSensor.getAcquisition();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    assert( acq == acqs[iAcq * 5] );
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

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
                CHECK( inputSensorSpec.m_sensorName == "sensorName" );
                CHECK( inputSensorSpec.m_resolutions.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
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

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
                CHECK( inputSensorSpec.m_sensorName == "sensorName2" );
                CHECK( inputSensorSpec.m_resolutions.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.size() == 1 );
                CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
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
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

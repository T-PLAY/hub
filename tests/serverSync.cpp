#include <catch2/catch_test_macros.hpp>

#include <IO/Stream.hpp>
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

#include <filesystem>

TEST_CASE( "Server test : sync" ) {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 8001;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 5;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs.emplace_back( iAcq * 2, iAcq * 2);
        acqs.back() << hub::Measure(data, dataSize );
    }
    std::vector<hub::Acquisition> acqs2;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10;
        }
        acqs2.emplace_back( iAcq2 * 10, iAcq2 * 10);
        acqs2.back() << hub::Measure(data, dataSize );
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 5 );
    server.asyncRun();
    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            { "sensorName", {{{3}, hub::SensorSpec::Format::BGR8}} },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

//        hub::io::OutputStream(hub::net::ClientSocket(ipv4, port));

        auto& outputSensorSpec = outputSensor.m_spec;
        CHECK( outputSensorSpec.m_acquisitionSize == dataSize );
        CHECK( outputSensorSpec.m_sensorName == "sensorName" );
            CHECK( outputSensorSpec.m_resolutions.size() == 1);
            CHECK( outputSensorSpec.m_resolutions[0].first.size() == 1 );
            CHECK( outputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( outputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::OutputSensor outputSensor2(
            { "sensorName2", {{{3}, hub::SensorSpec::Format::BGR8}} },
            hub::io::OutputStream( "master", hub::net::ClientSocket( ipv4, port ) ) );

        auto& outputSensorSpec2 = outputSensor2.m_spec;
        CHECK( outputSensorSpec2.m_acquisitionSize == dataSize );
        CHECK( outputSensorSpec2.m_sensorName == "sensorName2" );
            CHECK( outputSensorSpec2.m_resolutions.size() == 1);
            CHECK( outputSensorSpec2.m_resolutions[0].first.size() == 1 );
            CHECK( outputSensorSpec2.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( outputSensorSpec2.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream start" << std::endl;
        hub::InputSensor inputSensor(
            hub::io::InputStream( "stream", "master", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec = inputSensor.m_spec;
        CHECK( inputSensorSpec.m_acquisitionSize == dataSize );
        CHECK( inputSensorSpec.m_sensorName == "sensorName" );
            CHECK( inputSensorSpec.m_resolutions.size() == 1);
            CHECK( inputSensorSpec.m_resolutions[0].first.size() == 1 );
            CHECK( inputSensorSpec.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( inputSensorSpec.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] inputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream2 start" << std::endl;
        hub::InputSensor inputSensor2(
            hub::io::InputStream( "stream", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec2 = inputSensor2.m_spec;
        CHECK( inputSensorSpec2.m_acquisitionSize == dataSize );
        CHECK( inputSensorSpec2.m_sensorName == "sensorName" );
            CHECK( inputSensorSpec2.m_resolutions.size() == 1);
            CHECK( inputSensorSpec2.m_resolutions[0].first.size() == 1 );
            CHECK( inputSensorSpec2.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( inputSensorSpec2.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
        std::cout << "[Test] inputStream2 end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### inputStream3 start" << std::endl;
        hub::InputSensor inputSensor3(
            hub::io::InputStream( "master", "", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& inputSensorSpec3 = inputSensor3.m_spec;
        CHECK( inputSensorSpec3.m_acquisitionSize == dataSize );
        CHECK( inputSensorSpec3.m_sensorName == "sensorName2" );
            CHECK( inputSensorSpec3.m_resolutions.size() == 1);
            CHECK( inputSensorSpec3.m_resolutions[0].first.size() == 1 );
            CHECK( inputSensorSpec3.m_resolutions[0].first.at( 0 ) == 3 );
            CHECK( inputSensorSpec3.m_resolutions[0].second == hub::SensorSpec::Format::BGR8 );
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

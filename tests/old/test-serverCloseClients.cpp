#include <catch2/catch_test_macros.hpp>

#include <filesystem>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Server.hpp>

TEST_CASE( "Server test : close clients" ) {

    const std::string ipv4 = "127.0.0.1";
    srand( (unsigned)time( NULL ) );
    const int port = rand() % 65535;

    std::vector<hub::Acquisition> acqs;
    constexpr int nAcqs    = 2;
    constexpr int dataSize = 9;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs.emplace_back( iAcq * 2, iAcq * 2 );
        acqs.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                           dataSize,
                                           { { 3 }, hub::Format::BGR8 } );
    }
    std::vector<hub::Acquisition> acqs2;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10;
        }
        acqs2.push_back( hub::Acquisition( iAcq2 * 10, iAcq2 * 10 ) );
        acqs2.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                            dataSize,
                                            { { 3 }, hub::Format::BGR8 } );
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
            hub::SensorSpec { "sensorName", { { { 3 }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec = outputSensor.m_spec;
        CHECK( outputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec.getSensorName() == "sensorName" );
        CHECK( outputSensorSpec.getResolutions().size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::OutputSensor outputSensor2(
            hub::SensorSpec { "sensorName2", { { { 3 }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "master", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec2 = outputSensor2.m_spec;
        CHECK( outputSensorSpec2.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec2.getSensorName() == "sensorName2" );
        CHECK( outputSensorSpec2.getResolutions().size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        for ( int i = 0; i < 2; ++i ) {
            std::cout << "[Test] ############################### inputSensor(stream, master)"
                      << std::endl;
            {
                hub::InputSensor inputSensor( hub::io::InputStream(
                    "stream", "master", hub::net::ClientSocket( ipv4, port ) ) );

                const auto& inputSensorSpec = inputSensor.m_spec;
                CHECK( inputSensorSpec.getAcquisitionSize() == dataSize * 2 );
                CHECK( inputSensorSpec.getSensorName() == "sensorName + sensorName2" );
                CHECK( inputSensorSpec.getResolutions().size() == 2 );
                CHECK( inputSensorSpec.getResolutions()[0].first.size() == 1 );
                CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.getResolutions()[0].second == hub::Format::BGR8 );
                CHECK( inputSensorSpec.getResolutions()[1].first.size() == 1 );
                CHECK( inputSensorSpec.getResolutions()[1].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.getResolutions()[1].second == hub::Format::BGR8 );
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq : acqs ) {
                    outputSensor << acq;
                    std::cout << "send acq : " << acq << std::endl;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                std::cout << "[Test] ############################### send acquisitions 2"
                          << std::endl;
                for ( const auto& acq2 : acqs2 ) {
                    outputSensor2 << acq2;
                    std::cout << "send acq2 : " << acq2 << std::endl;
                }
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                std::cout << "[Test] ############################### compare " << std::endl;
                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                    auto acq = inputSensor.getAcquisition();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    std::cout << "ref acq : " << acqs[iAcq * 5] << std::endl;
                    assert( acq.m_start == 10 * iAcq );
                    CHECK( acq.m_start == 10 * iAcq );
                }
            }
            outputSensor << acqs.front();
            outputSensor2 << acqs2.front();
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            std::cout << "[Test] ############################### inputSensor(stream,)" << std::endl;
//            continue;
        }
    }
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    server.stop();
}

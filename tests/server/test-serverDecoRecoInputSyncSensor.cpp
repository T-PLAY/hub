#include <catch2/catch_test_macros.hpp>
#include "test-common.hpp"

#include <filesystem>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <server/Server.hpp>

TEST_CASE( "Server test : close clients" ) {

    const std::string ipv4 = "127.0.0.1";
//    srand( (unsigned)time( NULL ) );
//    const int port = rand() % 65535;
    const int port = GET_RANDOM_PORT;


    constexpr int nInput = 2;
    constexpr int nAcqs    = 2;
    constexpr int dataSize = 9;

    std::cout << "ref acqs2 : " << std::endl;
    std::vector<hub::Acquisition> acqs;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10;
        }
        acqs.push_back( hub::Acquisition( iAcq2 * 10, iAcq2 * 10 ) );
        acqs.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                            dataSize,
                                            { { 3 }, hub::Format::BGR8 } );
        std::cout << acqs.back() << std::endl;
    }

    std::vector<hub::Acquisition> acqs2;
    std::cout << "ref acqs2 : " << std::endl;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        acqs2.emplace_back( iAcq * 2, iAcq * 2 );
        acqs2.back() << hub::data::Measure( reinterpret_cast<const unsigned char*>( data ),
                                           dataSize,
                                           { { 3 }, hub::Format::BGR8 } );
        std::cout << acqs2.back() << std::endl;
    }

    std::cout << "[Test] ############################### server start" << std::endl;
    hub::server::Server server( port );
    server.setMaxClients( 2 + 2 * nInput );
    server.asyncRun();
//    std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::OutputSensor outputSensor(
            hub::SensorSpec { "sensorName", { { { 3 }, hub::Format::BGR8 } } },
            hub::io::OutputStream( "stream", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec = outputSensor.getSpec();
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
            hub::io::OutputStream( "stream2", hub::net::ClientSocket( ipv4, port ) ) );

        const auto& outputSensorSpec2 = outputSensor2.getSpec();
        CHECK( outputSensorSpec2.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec2.getSensorName() == "sensorName2" );
        CHECK( outputSensorSpec2.getResolutions().size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.getResolutions()[0].second == hub::Format::BGR8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        for ( int i = 0; i < nInput; ++i ) {
            std::cout << "[Test] ############################### inputSensor(stream, stream2)"
                      << std::endl;
            {
                hub::InputSensor inputSensor( hub::io::InputSyncStream(
                    "stream", "stream2", ipv4, port ) );

                const auto& inputSensorSpec = inputSensor.getSpec();
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
//                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                std::cout << "[Test] ############################### send acquisitions 2"
                          << std::endl;
                for ( const auto& acq2 : acqs2 ) {
                    outputSensor2 << acq2;
                    std::cout << "send acq2 : " << acq2 << std::endl;
                }
//                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

                std::cout << "[Test] ############################### compare " << std::endl;
                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                    hub::Acquisition acq;
                    inputSensor >> acq;
//                    auto acq = inputSensor.getAcq();
                    std::cout << "[Test] acq = " << acq << std::endl;
                    std::cout << "ref acq : " << acqs2[iAcq * 5] << std::endl;
                    assert( acq.getStart() == 10 * iAcq );
                    CHECK( acq.getStart() == 10 * iAcq );
                }
            }
//            outputSensor << acqs.front();
//            outputSensor2 << acqs2.front();
//            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//            std::cout << "[Test] ############################### inputSensor(stream,)" << std::endl;
//            continue;
        }
    }
//    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    std::cout << "[test] done" << std::endl;
//    server.stop();
}

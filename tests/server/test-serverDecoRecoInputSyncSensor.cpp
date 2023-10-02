#include "test_common.hpp"
#include "sensor/test_sensor_utils.hpp"


#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

//#include <server/Server.hpp>

TEST_CASE( "Server test : close clients" ) {

//    const std::string ipv4 = "127.0.0.1";
//    const int port         = GET_RANDOM_PORT;

    constexpr int nInput   = 2;
    constexpr int nAcqs    = 2;
    constexpr int dataSize = 9;

    std::cout << "ref acqs : " << std::endl;
    std::vector<hub::sensor::Acquisition> ref_acqs;
    int ref_offset = 4;
    for ( int iAcq2 = 0; iAcq2 < nAcqs; ++iAcq2 ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq2 * 10 + ref_offset;
        }
        ref_acqs.push_back( hub::sensor::Acquisition( iAcq2 * 10 + ref_offset, iAcq2 * 10 + ref_offset ) );
        ref_acqs.back() << hub::Measure( reinterpret_cast<const unsigned char*>( data ),
                                           dataSize,
                                           { { 3 }, hub::sensor::Format::BGR8 } );
        std::cout << ref_acqs.back() << std::endl;
    }

    std::vector<hub::sensor::Acquisition> ref_acqs2;
    std::cout << "ref acqs2 : " << std::endl;
    for ( int iAcq = 0; iAcq < nAcqs * 5; ++iAcq ) {
        unsigned char data[dataSize];
        for ( int i = 0; i < dataSize; ++i ) {
            data[i] = iAcq * 2;
        }
        ref_acqs2.emplace_back( iAcq * 2, iAcq * 2 );
        ref_acqs2.back() << hub::Measure( reinterpret_cast<const unsigned char*>( data ),
                                            dataSize,
                                            { { 3 }, hub::sensor::Format::RGB8 } );
        std::cout << ref_acqs2.back() << std::endl;
    }


    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    return;

    std::cout << "[Test] ############################### server start" << std::endl;
//    hub::Server server( port );
//    server.setMaxClients( 2 + 2 * nInput );
//    server.asyncRun();
    std::cout << "[Test] server end ------------------------------" << std::endl;

    {
        std::cout << "[Test] ############################### outputStream start" << std::endl;
        hub::sensor::OutputSensor outputSensor(
            hub::sensor::SensorSpec { "sensorName", { { { 3 }, hub::sensor::Format::BGR8 } } },
            hub::output::OutputStream(FILE_NAME)
//            "stream",
//            hub::net::ClientSocket( ipv4, port ) );
//            ipv4, port
            );

        const auto& outputSensorSpec = outputSensor.getSpec();
        CHECK( outputSensorSpec.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec.getSensorName() == "sensorName" );
        CHECK( outputSensorSpec.getResolutions().size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec.getResolutions()[0].second == hub::sensor::Format::BGR8 );
        std::cout << "[Test] outputStream end ---------------------------------" << std::endl;

        std::cout << "[Test] ############################### outputStream2 start" << std::endl;
        hub::sensor::OutputSensor outputSensor2(
            hub::sensor::SensorSpec { "sensorName2", { { { 3 }, hub::sensor::Format::RGB8 } } },
            hub::output::OutputStream(FILE_NAME + "2")
//            "stream2",
//            hub::net::ClientSocket( ipv4, port ) );
//            ipv4, port
            );

        const auto& outputSensorSpec2 = outputSensor2.getSpec();
        CHECK( outputSensorSpec2.getAcquisitionSize() == dataSize );
        CHECK( outputSensorSpec2.getSensorName() == "sensorName2" );
        CHECK( outputSensorSpec2.getResolutions().size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.size() == 1 );
        CHECK( outputSensorSpec2.getResolutions()[0].first.at( 0 ) == 3 );
        CHECK( outputSensorSpec2.getResolutions()[0].second == hub::sensor::Format::RGB8 );
        std::cout << "[Test] outputStream2 end ---------------------------------" << std::endl;

        for ( int i = 0; i < nInput; ++i ) {
            std::cout << "[Test] ############################### inputSensor(stream, stream2)"
                      << std::endl;
            {
                hub::sensor::InputSensor inputSensor(
//                    hub::input::InputSyncStream( "stream", "stream2", ipv4, port ) );
                    hub::input::InputSyncStream( FILE_NAME, FILE_NAME +  "2" ) );

                const auto& inputSensorSpec = inputSensor.getSpec();
                CHECK( inputSensorSpec.getAcquisitionSize() == dataSize * 2 );
                CHECK( inputSensorSpec.getSensorName() == "sensorName + sensorName2" );
                CHECK( inputSensorSpec.getResolutions().size() == 2 );
                CHECK( inputSensorSpec.getResolutions()[0].first.size() == 1 );
                CHECK( inputSensorSpec.getResolutions()[0].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.getResolutions()[0].second == hub::sensor::Format::BGR8 );
                CHECK( inputSensorSpec.getResolutions()[1].first.size() == 1 );
                CHECK( inputSensorSpec.getResolutions()[1].first.at( 0 ) == 3 );
                CHECK( inputSensorSpec.getResolutions()[1].second == hub::sensor::Format::RGB8 );
                std::cout << "[Test] inputStream end ---------------------------------"
                          << std::endl;

                std::cout << "[Test] ############################### send acquisitions"
                          << std::endl;
                for ( const auto& acq : ref_acqs ) {
                    outputSensor << acq;
                    std::cout << "send acq : " << acq << std::endl;
                }
                std::cout << "[Test] ############################### send acquisitions 2"
                          << std::endl;
                for ( const auto& acq2 : ref_acqs2 ) {
                    outputSensor2 << acq2;
                    std::cout << "send acq2 : " << acq2 << std::endl;
                }

                std::cout << "[Test] ############################### compare " << std::endl;
//                for ( int iAcq = 0; iAcq < nAcqs; ++iAcq ) {
                for ( int iAcq = 0; iAcq < ref_sync_acqs.size(); ++iAcq ) {
                    hub::sensor::Acquisition acq;
                    inputSensor >> acq;
//                    std::cout << "synchronized acq = " << acq << std::endl;
//                    std::cout << "ref synched acq = " << ref_sync_acqs.at(iAcq) << std::endl;
//                    assert( acq.getStart() == 10 * iAcq );
//                    CHECK( acq.getStart() == 10 * iAcq );
                    CHECK(acq == ref_sync_acqs.at(iAcq));
                }
            }
        }
    }

    std::cout << "[test] done" << std::endl;
}

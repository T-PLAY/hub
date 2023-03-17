#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <server/Server.hpp>
#include <server/StreamerClient.hpp>

#include <io/Memory.hpp>


TEST_CASE( "StreamerClient test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort();

    hub::server::Server server( port );
    server.setMaxClients( 3 );
    server.asyncRun();

    const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
    const hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::Acquisition> ref_acqs;
    constexpr int ref_nAcqs = 10;
    const int ref_dataSize  = hub::res::computeAcquisitionSize( ref_resolution );

    unsigned char * data = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
//        unsigned char data[ref_dataSize];
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = rand();
        }
        ref_acqs.emplace_back( 5 + iAcq + 1, 5 + iAcq + 2 );
        ref_acqs.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
    }
    delete [] data;
    hub::OutputSensor outputSensor(
        ref_sensorSpec, hub::io::OutputStream( "streamName", hub::net::ClientSocket( ipv4, port ) ) );

    //////////////////////

    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution } );
    std::vector<hub::Acquisition> ref_acqs2;
    constexpr int ref_nAcqs2 = 20;
    const int ref_dataSize2  = hub::res::computeAcquisitionSize( ref_resolution );
    unsigned char * data2 = new unsigned char[ref_dataSize2];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
//        unsigned char data[ref_dataSize2];
        for ( int i = 0; i < ref_dataSize2; ++i ) {
            data2[i] = rand();
        }
        ref_acqs2.emplace_back( iAcq + 1, iAcq + 2 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution );
    }
    delete [] data2;
    hub::OutputSensor outputSensor2(
        ref_sensorSpec2, hub::io::OutputStream( "streamName2", hub::net::ClientSocket( ipv4, port ) ) );

    //////////////////////

    std::vector<hub::Acquisition> sync_acqs;
    for (const auto & acq : ref_acqs) {
        int iMinDist = 0;
        int minDist = computeDist(acq, ref_acqs2.front());
//        for (const auto & acq2 : ref_acqs2) {
        for (int iAcq2 = 1; iAcq2 < ref_acqs2.size(); ++iAcq2) {
            const auto & acq2 = ref_acqs2.at(iAcq2);
            const auto dist = computeDist(acq, acq2);
            if (dist < minDist) {
                minDist = dist;
                iMinDist = iAcq2;
            }
        }

        const auto & acq2 = ref_acqs2.at(iMinDist);
//        sync_acqs.emplace_back(acq.m_start, acq.m_end);
        sync_acqs.push_back(acq.clone());
        auto & syncAcq = sync_acqs.back();
        syncAcq << acq2.getMeasures().front().clone();

        CHECK(syncAcq.getStart() == acq.getStart());
        CHECK(syncAcq.getEnd() == acq.getEnd());
        CHECK(syncAcq.getMeasures().size() == 2);

        CHECK(acq.getMeasures().size() == 1);
        const auto & measure = acq.getMeasures().front();
        CHECK(syncAcq.getMeasures().at(0) == measure);

        CHECK(acq2.getMeasures().size() == 1);
        const auto & measure2 = acq2.getMeasures().front();
        CHECK(syncAcq.getMeasures().at(1) == measure2);
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    hub::InputSensor inputSensor(
        hub::io::InputStream( "streamName", "streamName2", hub::net::ClientSocket( ipv4, port ) ) );

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    for (const auto & acq : ref_acqs) {
        outputSensor << acq;
    }
    for (const auto & acq2 : ref_acqs2) {
        outputSensor2 << acq2;
    }

    std::vector<hub::Acquisition> acqs;
    for (int i = 0; i <ref_nAcqs; ++i) {
        auto acq2 = inputSensor.getAcquisition();
        acqs.push_back(std::move(acq2));
    }
//    auto acqs = inputSensor.getAllAcquisitions();
    CHECK( ref_acqs.size() == acqs.size() );

    for ( int i = 0; i < sync_acqs.size(); ++i ) {
        const auto & syncAcq = sync_acqs.at(i);
        const auto & acq = acqs.at(i);
        std::cout << "checking: " << syncAcq << " == " << acq << std::endl;
        CHECK( ref_acqs.at( i ) == acqs.at( i ) );
    }
}

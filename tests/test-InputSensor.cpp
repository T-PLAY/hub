#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

// #include <server/Server.hpp>

// #include <io/Interface.hpp>
#include <io/Memory.hpp>

TEST_CASE( "InputSensor test" ) {

    const std::string ipv4 = "127.0.0.1";
    const int port         = getRandomPort();

    //    {
    const int ref_offset    = 0;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 5;
    constexpr int ref_nAcqs2 = 10;

    const int ref3_offset    = 0;
    constexpr int ref_nAcqs3 = 15;

    std::cout << "ref_acqs" << std::endl;
    const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
    const hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::Acquisition> ref_acqs;
    const int ref_dataSize = hub::res::computeAcquisitionSize( ref_resolution );
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        unsigned char data[ref_dataSize];
        for ( int i = 0; i < ref_dataSize; ++i ) {
//            data[i] = rand();
            data[i] = ref_offset + iAcq + 1;
        }
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
        ref_acqs.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    std::vector<char> buff;
    hub::io::Memory<decltype( buff )> memory( buff );
    hub::OutputSensor outputSensor( ref_sensorSpec, std::move( memory ) );
    //    }

    std::cout << std::endl;

    //////////////////////

    //    {
    std::cout << "ref2_acqs" << std::endl;
    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        unsigned char data[ref_dataSize2];
        for ( int i = 0; i < ref_dataSize2; ++i ) {
//            data[i] = rand();
            data[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize2, ref_resolution2 );
        std::cout << ref_acqs2.back() << std::endl;
    }
    std::vector<char> buff2;
    hub::io::Memory<decltype( buff2 )> memory2( buff2 );
    hub::OutputSensor outputSensor2( ref_sensorSpec2, std::move( memory2 ) );
    //    }
    std::cout << std::endl;

    //////////////////////

    //    {
    const hub::Resolution ref_resolution3( { { 1 }, hub::Format::BGR8 } );
    const hub::SensorSpec ref_sensorSpec3( "sensorName2", { ref_resolution3 } );
    std::vector<hub::Acquisition> ref_acqs3;
    const int ref_dataSize3 = hub::res::computeAcquisitionSize( ref_resolution3 );
    for ( int iAcq = 0; iAcq < ref_nAcqs3; ++iAcq ) {
        unsigned char data[ref_dataSize3];
        for ( int i = 0; i < ref_dataSize3; ++i ) {
            data[i] = rand();
        }
        ref_acqs3.emplace_back( ref3_offset + iAcq + 1, ref3_offset + iAcq + 2 );
        ref_acqs3.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize3, ref_resolution3 );
    }
    std::vector<char> buff3;
    hub::io::Memory<decltype( buff3 )> memory3( buff3 );
    hub::OutputSensor outputSensor3( ref_sensorSpec3, std::move( memory3 ) );
    //    }

    //////////////////////

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs;
    std::vector<int> min_dists( ref_acqs2.size(), 999999 );
    std::vector<int> iMin_dists( ref_acqs2.size(), -1 );

    int iAcq = 0;
    for ( const auto& acq : ref_acqs ) {
        int iMinDist = 0;
        int minDist  = computeDist( acq, ref_acqs2.front() );
        //        for (const auto & acq2 : ref_acqs2) {
        for ( int iAcq2 = 1; iAcq2 < ref_acqs2.size(); ++iAcq2 ) {
            const auto& acq2 = ref_acqs2.at( iAcq2 );
            const auto dist  = computeDist( acq, acq2 );
            if ( dist <= minDist ) {
                minDist  = dist;
                iMinDist = iAcq2;
            }
        }

        if ( minDist < min_dists.at( iMinDist ) ) {
            min_dists[iMinDist]  = minDist;
            iMin_dists[iMinDist] = iAcq;
        }
        ++iAcq;
    }

    for ( int i = 0; i < ref_acqs2.size(); ++i ) {
        if ( iMin_dists[i] != -1 ) {
            const auto& acq = ref_acqs.at( iMin_dists.at( i ) );
            //            ref_sync_acqs.push_back(ref_acqs.at(iMin_dists[i]));
            const auto& acq2 = ref_acqs2.at( i );
//            ref_sync_acqs.emplace_back( acq.getStart(), acq.getEnd() );
            ref_sync_acqs.push_back( acq.clone() );

            auto& syncAcq = ref_sync_acqs.back();
            syncAcq << acq2.getMeasures();

            std::cout << ref_sync_acqs.back() << std::endl;

            CHECK( syncAcq.getStart() == acq.getStart() );
            CHECK( syncAcq.getEnd() == acq.getEnd() );
            CHECK( syncAcq.getMeasures().size() == 2 );

            CHECK( acq.getMeasures().size() == 1 );
            const auto& measure = acq.getMeasures().front();
            CHECK( syncAcq.getMeasures().at( 0 ) == measure );

            CHECK( acq2.getMeasures().size() == 1 );
            const auto& measure2 = acq2.getMeasures().front();
            CHECK( syncAcq.getMeasures().at( 1 ) == measure2 );
        }
    }
    std::cout << std::endl;
//    assert(ref_acqs.size() == 5);

//    for (const auto & acq : ref_sync_acqs) {
//        std::cout << acq << std::endl;
//    }
//    std::cout << std::endl;

    //        const auto & acq2 = ref_acqs2.at(iMinDist);
    //        ref_sync_acqs.emplace_back(acq.m_start, acq.m_end);
    //        ref_sync_acqs.push_back(acq.clone());
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for ( const auto& acq : ref_acqs ) {
        outputSensor << acq;
    }
    for ( const auto& acq2 : ref_acqs2 ) {
        outputSensor2 << acq2;
    }
    for ( const auto& acq3 : ref_acqs3 ) {
        outputSensor3 << acq3;
    }

    hub::io::Memory<decltype( buff )> memoryIn( buff );
    hub::InputSensor inputSensor( std::move( memoryIn ) );
    hub::io::Input& input = inputSensor.getInput();

    hub::io::Memory<decltype( buff2 )> memoryIn2( buff2 );
    hub::InputSensor inputSensor2( std::move( memoryIn2 ) );
    hub::io::Input& input2 = inputSensor2.getInput();

    hub::io::Memory<decltype( buff3 )> memoryIn3( buff3 );
    hub::InputSensor inputSensor3( std::move( memoryIn3 ) );
    hub::io::Input& input3 = inputSensor3.getInput();

    hub::Acquisition acq;
    std::cout << "sync acqs" << std::endl;
    std::vector<hub::Acquisition> sync_acqs;
//    int iAcq2 = 0;
    while ( !input.isEnd() && !input2.isEnd() ) {
//    while ( !input.isEnd()) {
//    while (true) {

        inputSensor >> inputSensor2 >> acq;
//        if (acq.isEmpty())
//            break;
        std::cout << acq << std::endl;
        sync_acqs.push_back(std::move(acq));
//        CHECK( acq == ref_sync_acqs.at( iAcq2 ) );
//        ++iAcq2;
    }

    std::cout << std::endl;

    assert(sync_acqs.size() == ref_sync_acqs.size());
    for (int i = 0; i <sync_acqs.size(); ++i) {
        const auto & acq = sync_acqs.at(i);
//        std::cout << acq << std::endl;

        CHECK( acq == ref_sync_acqs.at( i ) );
    }

    //    while (! input.isEnd()) {
    //        inputSensor >> acq;
    //        sync_acqs.push_back(std::move(acq));
    //    }
    //    CHECK(sync_acqs.size() == ref_acqs.size());
    //    for ( int i = 0; i < sync_acqs.size(); ++i ) {
    //        CHECK(sync_acqs.at(i) == ref_acqs.at(i));
    //    }

    //    std::vector<hub::Acquisition> acqs2;
    //    while (! input2.isEnd()) {
    //        inputSensor2 >> acq;
    //        acqs2.push_back(std::move(acq));
    //    }
    //    CHECK(acqs2.size() == ref_acqs2.size());
    //    for ( int i = 0; i < acqs2.size(); ++i ) {
    //        CHECK(acqs2.at(i) == ref_acqs2.at(i));
    //    }

    //    auto inputSyncSensor = inputSensor >> inputSensor2;
    //    auto & input4 = inputSyncSensor.getInput();
//    int iAcq2 = 0;
    //    assert(input4.isOpen());
    //    assert(! input4.isEnd());
//    std::cout << std::endl;

    //    while (! input.isEnd() && ! input2.isEnd() && ! input3.isEnd()) {
    //        inputSensor >> inputSensor2 >> inputSensor3 >> acq;
    //    }

    //    std::vector<hub::Acquisition> sync_acqs;
    //    for (int i = 0; i <ref_nAcqs; ++i) {
    //        auto acq2 = inputSensor.getAcquisition();
    //        sync_acqs.push_back(std::move(acq2));
    //    }
    //    auto sync_acqs = inputSensor.getAllAcquisitions();
    //    CHECK( ref_acqs.size() == sync_acqs.size() );

    //    for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
    //        const auto & syncAcq = ref_sync_acqs.at(i);
    //        const auto & acq = sync_acqs.at(i);
    //        std::cout << "checking: " << syncAcq << " == " << acq << std::endl;
    //        CHECK( ref_acqs.at( i ) == sync_acqs.at( i ) );
    //    }

    ///////////////////////////////////////////////////////////////////////////////////////////

    //    hub::io::CyclicBuff buff;

    //    const auto resolution = hub::Resolution { { 1 }, hub::Format::BGR8 };
    //    hub::SensorSpec sensorSpec( "sensorName", { resolution } );
    //    unsigned char data[3] { 1, 2, 3 };
    //    hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( buff ) );

    //    auto acq = std::move( hub::Acquisition { 0, 1 } << hub::data::Measure { data, 3,
    //    resolution } ); outputSensor << acq; outputSensor << acq; outputSensor << acq;

    //    hub::InputSensor inputSensor { hub::io::Ram( buff ) };

    //    auto acq2 = inputSensor.getAcquisition();
    //    CHECK( acq == acq2 );

    //    const auto& input = inputSensor.getInput();
    //    CHECK( !input.isEnd() );

    //    auto sync_acqs = inputSensor.getAllAcquisitions();
    //    CHECK( sync_acqs.size() == 2 );
    //    CHECK( sync_acqs[0] == acq );
    //    CHECK( sync_acqs[1] == acq );

    //    CHECK( input.isEnd() );

    //    const hub::Resolution resolution2( { 1 }, hub::Format::BGR8 );
    //    const hub::SensorSpec sensorSpec2( "hello", { resolution2 } );
    //    const std::string streamName = "streamName";
    //    const std::string ipv4       = "127.0.0.1";
    //    const int port               = getRandomPort();

    //    hub::server::Server server( port );
    //    server.setMaxClients( 2 );
    //    server.asyncRun();

    //    hub::OutputSensor* outputSensor2 = new hub::OutputSensor(
    //        sensorSpec2, hub::io::OutputStream( streamName, hub::net::ClientSocket( ipv4, port ) )
    //        );

    //    unsigned char data2[3] = { 0, 1, 2 };
    //    const hub::Acquisition acq3 =
    //        std::move( hub::Acquisition( 0, 1 ) << hub::data::Measure( data2, 3, resolution2 ) );
    //    *outputSensor2 << acq3;

    //    hub::InputSensor inputSensor2(
    //        hub::io::InputStream( streamName, "", hub::net::ClientSocket( ipv4, port ) ) );

    //    auto acq4 = inputSensor2.getAcquisition();
    //    CHECK( acq3 == acq4 );

    //    delete outputSensor2;

    //    try {

    //        auto acqs2 = inputSensor2.getAllAcquisitions();
    //        CHECK( false );
    //    }
    //    catch ( std::exception& ex ) {
    //        std::cout << "[test] catch execption : " << ex.what() << std::endl;
    //        CHECK( true );
    //    }
}

#include <catch2/catch_test_macros.hpp>

#include "test-common.hpp"

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

// #include <server/Server.hpp>

// #include <io/Interface.hpp>
#include <io/Memory.hpp>

TEST_CASE( "InputSensor test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 0;
    constexpr int ref_nAcqs2 = 10;

    const int ref3_offset    = 0;
    constexpr int ref_nAcqs3 = 15;

    std::cout << "ref_acqs" << std::endl;
    const hub::Resolution ref_resolution( { { 1 }, hub::Format::Y8 } );
    const hub::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::Acquisition> ref_acqs;
    const int ref_dataSize = hub::res::computeAcquisitionSize( ref_resolution );
    unsigned char* data    = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
        ref_acqs.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    delete[] data;
    std::vector<char> buff;
    hub::io::Memory<decltype( buff )> memory( buff );
    hub::OutputSensor outputSensor( ref_sensorSpec, std::move( memory ) );

    std::cout << std::endl;

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
    unsigned char* data2    = new unsigned char[ref_dataSize2];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize2; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 2 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );
        std::cout << ref_acqs2.back() << std::endl;
    }
    delete[] data2;
    std::vector<char> buff2;
    hub::io::Memory<decltype( buff2 )> memory2( buff2 );
    hub::OutputSensor outputSensor2( ref_sensorSpec2, std::move( memory2 ) );
    std::cout << std::endl;

    //////////////////////

    const hub::Resolution ref_resolution3( { { 1 }, hub::Format::BGR8 } );
    const hub::SensorSpec ref_sensorSpec3( "sensorName2", { ref_resolution3 } );
    std::vector<hub::Acquisition> ref_acqs3;
    const int ref_dataSize3 = hub::res::computeAcquisitionSize( ref_resolution3 );
    unsigned char* data3    = new unsigned char[ref_dataSize3];
    for ( int iAcq = 0; iAcq < ref_nAcqs3; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize3; ++i ) {
            data3[i] = rand();
        }
        ref_acqs3.emplace_back( ref3_offset + iAcq + 1, ref3_offset + iAcq + 2 );
        ref_acqs3.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data3 ), ref_dataSize3, ref_resolution3 );
    }
    delete[] data3;

    std::vector<char> buff3;
    hub::io::Memory<decltype( buff3 )> memory3( buff3 );
    hub::OutputSensor outputSensor3( ref_sensorSpec3, std::move( memory3 ) );

    //////////////////////

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs;
    std::vector<int> min_dists( ref_acqs2.size(), 999999 );
    std::vector<int> iMin_dists( ref_acqs2.size(), -1 );

    int iAcq = 0;
    for ( const auto& acq : ref_acqs ) {
        int iMinDist = 0;
        int minDist  = computeDist( acq, ref_acqs2.front() );
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
            const auto& acq  = ref_acqs.at( iMin_dists.at( i ) );
            const auto& acq2 = ref_acqs2.at( i );
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
    const hub::io::Input& input = inputSensor.getInput();

    hub::io::Memory<decltype( buff2 )> memoryIn2( buff2 );
    hub::InputSensor inputSensor2( std::move( memoryIn2 ) );
    const hub::io::Input& input2 = inputSensor2.getInput();

    hub::Acquisition acq;
    std::cout << "sync acqs" << std::endl;
    std::vector<hub::Acquisition> sync_acqs;
    while ( !input.isEnd() && !input2.isEnd() ) {

        inputSensor >> inputSensor2 >> acq;
        std::cout << acq << std::endl;
        sync_acqs.push_back( std::move( acq ) );
    }

    std::cout << std::endl;

    assert( sync_acqs.size() == ref_sync_acqs.size() );
    for ( int i = 0; i < sync_acqs.size(); ++i ) {
        const auto& acq2 = sync_acqs.at( i );

        CHECK( acq2 == ref_sync_acqs.at( i ) );
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
}

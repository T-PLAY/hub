#include <catch2/catch_test_macros.hpp>

#include "test_common.hpp"

#include <set>

#include <io/input/InputSyncStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>

#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "InputSyncStream test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 0;
    constexpr int ref_nAcqs2 = 10;

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

    std::cout << std::endl;

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::Resolution ref_resolution2( { { 1 }, hub::Format::DOF6 } );
    const hub::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::res::computeAcquisitionSize( ref_resolution2 );
    float data2[7];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < 7; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 1 );
        ref_acqs2.back() << hub::data::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );

        std::cout << ref_acqs2.back() << std::endl;
    }
    std::cout << std::endl;

    //////////////////////

    std::cout << "ref_sync_acqs" << std::endl;
    std::vector<hub::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

    {
        hub::output::OutputStream outputStream( __FILE_NAME__ );
        InputStream inputStream( __FILE_NAME__ );

        hub::output::OutputStream outputStream2( __FILE_NAME__ "2" );
        InputStream inputStream2( __FILE_NAME__ "2" );

        std::vector<hub::Acquisition> sync_acqs = synchronize( std::move(outputStream),
                                                               ref_sensorSpec,
                                                               ref_acqs,
                                                               std::move(outputStream2),
                                                               ref_sensorSpec2,
                                                               ref_acqs2,
                                                               std::move(inputStream),
                                                               std::move(inputStream2), true );

        assert( sync_acqs.size() == ref_sync_acqs.size() );
        for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
            const auto& sync_acq = sync_acqs.at( i );
            CHECK( sync_acq == ref_sync_acqs.at( i ) );
        }
    }
    std::cout << "end output streams" << std::endl;
}

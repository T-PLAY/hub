#include "test_common.hpp"
#include "sensor/test_sensor_utils.hpp"

#include <set>

#include <io/input/InputSyncStream.hpp>
#include <io/output/OutputStream.hpp>
// #include <server/Server.hpp>

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputSyncStream test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    const int ref2_offset    = 0;
    constexpr int ref_nAcqs2 = 10;

    std::cout << "ref_acqs" << std::endl;
    const hub::sensor::Resolution ref_resolution( { { 1 }, hub::sensor::Format::Y8 } );
    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    std::vector<hub::sensor::Acquisition> ref_acqs;
    const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution );
    unsigned char* data    = new unsigned char[ref_dataSize];
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        ref_acqs.emplace_back( ref_offset + iAcq + 1, ref_offset + iAcq + 2 );
        ref_acqs.back() << hub::Measure(
            reinterpret_cast<const unsigned char*>( data ), ref_dataSize, ref_resolution );
        std::cout << ref_acqs.back() << std::endl;
    }
    delete[] data;

    std::cout << std::endl;

    //////////////////////

    std::cout << "ref2_acqs" << std::endl;
    const hub::sensor::Resolution ref_resolution2( { { 1 }, hub::sensor::Format::DOF6 } );
    const hub::sensor::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    std::vector<hub::sensor::Acquisition> ref_acqs2;
    const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize( ref_resolution2 );
    float data2[7];
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < 7; ++i ) {
            data2[i] = ref2_offset + iAcq + 1;
        }
        ref_acqs2.emplace_back( ref2_offset + iAcq + 1, ref2_offset + iAcq + 1 );
        ref_acqs2.back() << hub::Measure(
            reinterpret_cast<const unsigned char*>( data2 ), ref_dataSize2, ref_resolution2 );

        std::cout << ref_acqs2.back() << std::endl;
    }
    std::cout << std::endl;

    //////////////////////

        std::cout << "ref_sync_acqs" << std::endl;
        std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

#ifdef HUB_BUILD_SERVER
    {
        hub::sensor::OutputSensor outputSensor(ref_sensorSpec, hub::output::OutputStream( FILE_NAME ));
        hub::sensor::OutputSensor outputSensor2(ref_sensorSpec2, hub::output::OutputStream( FILE_NAME "2" ));

        hub::sensor::InputSensor inputSyncSensor(hub::input::InputSyncStream(FILE_NAME, FILE_NAME "2"));

        checkSynchronize( outputSensor.getOutput(),
                          ref_acqs,
                          outputSensor2.getOutput(),
                          ref_acqs2,
                          ref_sync_acqs,
                          false,
                          inputSyncSensor.getInput()
                          );

    }
#else
    {
        std::cout << "outputStream start" << std::endl;
        hub::output::OutputStream outputStream( FILE_NAME );
        std::cout << "outputStream end" << std::endl;

//        std::cout << "inputStream start" << std::endl;
//        hub::input::InputStream inputStream( FILE_NAME );
//        std::cout << "inputStream end" << std::endl;

        //        std::cout << "outputStream2 start" << std::endl;
        hub::output::OutputStream outputStream2( FILE_NAME "2" );
        //        std::cout << "outputStream2 end" << std::endl;

//        //        std::cout << "inputStream2 start" << std::endl;
//        hub::input::InputStream inputStream2( FILE_NAME "2" );
//        //        std::cout << "inputStream2 end" << std::endl;

        hub::input::InputSyncStream inputSyncStream(FILE_NAME, FILE_NAME "2");

        // todo sync stream
//        checkSynchronize( std::move( outputStream ),
//                          ref_sensorSpec,
//                          ref_acqs,
//                          std::move( outputStream2 ),
//                          ref_sensorSpec2,
//                          ref_acqs2,
////                          std::move( inputStream ),
////                          std::move( inputStream2 ),
//                          std::move( inputSyncStream ),
//                          true );

//        while ()

        //        assert( sync_acqs.size() == ref_sync_acqs.size() );
        //        for ( int i = 0; i < ref_sync_acqs.size(); ++i ) {
        //            const auto& sync_acq = sync_acqs.at( i );
        //            CHECK( sync_acq == ref_sync_acqs.at( i ) );
        //            std::cout << "receive sync acq : " << sync_acq << std::endl;
        //        }
    }
#endif
    std::cout << "end output streams" << std::endl;
}

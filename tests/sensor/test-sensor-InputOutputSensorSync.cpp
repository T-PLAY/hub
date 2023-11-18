#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"

// #include <initializer_list>
// #include <span>
// #include <vector>

#include "core/test_core_common.hpp"
#include "sensor/test_sensor_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputOutputSensorSync test" ) {

    const int ref_offset    = 5;
    constexpr int ref_nAcqs = 10;

    std::cout << "ref_acqs" << std::endl;
    const auto ref_resolution = hub::make_matrix<hub::sensor::format::Y8>();
    const hub::sensor::SensorSpec ref_sensorSpec( "sensorName" );
    hub::sensor::Acquisition acq = hub::sensor::make_acquisition( ref_resolution );

    std::vector<hub::sensor::Acquisition> ref_acqs;
    auto ref_dataSize = acq.size() - 2 * sizeof( hub::sensor::Clock );
    auto* data        = acq.data() + 2 * sizeof( hub::sensor::Clock );
    for ( int iAcq = 0; iAcq < ref_nAcqs; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize; ++i ) {
            data[i] = ref_offset + iAcq + 1;
        }
        acq.start() = ref_offset + iAcq + 1;
        acq.end()   = ref_offset + iAcq + 2;
        ref_acqs.push_back( acq.clone() );
        std::cout << ref_acqs.back() << std::endl;
    }
    std::cout << std::endl;

    //////////////////////

    const int ref_offset2    = 0;
    constexpr int ref_nAcqs2 = 10;

    std::cout << "ref2_acqs" << std::endl;
    const auto ref_resolution2 = hub::make_matrix<hub::sensor::format::Z16>();
    const hub::sensor::SensorSpec ref_sensorSpec2( "sensorName" );
    hub::sensor::Acquisition acq2 = hub::sensor::make_acquisition( ref_resolution2 );

    std::vector<hub::sensor::Acquisition> ref_acqs2;
    auto ref_dataSize2 = acq2.size() - 2 * sizeof( hub::sensor::Clock );
    auto* data2        = acq2.data() + 2 * sizeof( hub::sensor::Clock );
    for ( int iAcq = 0; iAcq < ref_nAcqs2; ++iAcq ) {
        for ( int i = 0; i < ref_dataSize2; ++i ) {
            data2[i] = ref_offset2 + iAcq + 1;
        }
        acq2.start() = ref_offset2 + iAcq + 1;
        acq2.end()   = ref_offset2 + iAcq + 1;
        ref_acqs2.push_back( acq2.clone() );
        std::cout << ref_acqs2.back() << std::endl;
    }
    std::cout << std::endl;


    //    //////////////////////

//        std::cout << "ref_sync_acqs" << std::endl;
        std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2
        );

    //    {
    //        hub::sensor::OutputSensor outputSensor( ref_sensorSpec,
    //                                                hub::output::OutputStream( FILE_NAME ) );
    //        hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2,
    //                                                 hub::output::OutputStream( FILE_NAME + "2" )
    //                                                 );

    //        hub::sensor::InputSensor inputSyncSensor(
    //            hub::input::InputSyncStream( FILE_NAME, FILE_NAME + "2" ) );

    //        checkSynchronize( outputSensor.getOutput(),
    //                          ref_acqs,
    //                          outputSensor2.getOutput(),
    //                          ref_acqs2,
    //                          ref_sync_acqs,
    //                          false,
    //                          inputSyncSensor.getInput() );
    //    }

    //    hub::io::Archive archive;

    //    struct UserType {
    //        int a;
    //        bool b;
    //        static constexpr auto name() { return "UserType"; };
    //    };
    //    static_assert( sizeof( UserType ) == 8 );

    //    // init outputSensor
    //    hub::sensor::SensorSpec::MetaData metaData;
    //    metaData["name"] = "gauthier";
    //    hub::sensor::SensorSpec sensorSpec( "sensorName", metaData );
    //    using UserResolution = UserType;
    //    hub::sensor::OutputSensorT<UserResolution> outputSensor( sensorSpec, archive );

    //    // init output acq
    //    hub::sensor::OutputSensorT<UserResolution>::Acq acq;
    ////    hub::sensor::AcquisitionT<UserResolution> acq; // same as above
    //    acq.getStart() = 4;
    //    acq.getEnd() = 3;
    //    auto& userType = acq.get<UserType&>();
    //    userType.a     = 2;
    //    userType.b     = true;

    //    // init inputSensor
    //    hub::sensor::InputSensor inputSensor( archive );
    //    CHECK( archive.isEnd() );
    //    CHECK( outputSensor.getSpec() == inputSensor.getSpec() );

    //    // write output acq
    //    outputSensor << acq;

    //    // read input acq
    //    auto acq_read = inputSensor.acq();
    //    inputSensor >> acq_read;
    //    CHECK( acq == acq_read );
}

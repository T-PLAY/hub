//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

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

    //    //////////////////////

//    hub::io::Archive archive;
//    hub::io::Archive archive2;
//    inputOutputBench(archive, archive2);
    inputOutputBench<hub::io::Archive>();
    return;

//    //        std::cout << "ref_sync_acqs" << std::endl;
//    std::vector<hub::sensor::Acquisition> ref_sync_acqs = computeSyncAcqs( ref_acqs, ref_acqs2 );

//    //    {
////    hub::sensor::OutputSensorT<UserResolution> outputSensor( sensorSpec, archive );
//    hub::sensor::OutputSensor outputSensor( ref_sensorSpec, archive);
//    hub::sensor::OutputSensor outputSensor2( ref_sensorSpec2, archive2);

//    hub::sensor::InputSensor inputSensorSync(archive, archive2);
////    std::cout << "ref_sensorSpec: " << ref_sensorSpec << std::endl;
//    std::cout << "outputSensor spec: " << outputSensor.getSpec() << std::endl;
//    std::cout << "outputSensor2 spec: " << outputSensor2.getSpec() << std::endl;
//    std::cout << "inputSensor spec: " << inputSensorSync.getSpec() << std::endl;

//    for (const auto & acq : ref_acqs) {
//        outputSensor << acq;
//    }
//    for (const auto & acq2 : ref_acqs2) {
//        outputSensor2 << acq2;
//    }

//    std::vector<hub::sensor::Acquisition> acqs_read;
//    auto read_acq = inputSensorSync.acq();
//    std::cout << "read_acq: " << read_acq << std::endl;
//    std::cout << "read_acq_res: " << read_acq.getResolution() << std::endl;

//    while (! archive.isEnd() && !archive2.isEnd()) {
//        inputSensorSync >> read_acq;
////        std::cout << "read sync acq: " << read_acq << std::endl;
////        acqs_read.push_back(std::move(read_acq));
//        acqs_read.push_back(read_acq.clone());
//    }

//    for (const auto & syncAcq: acqs_read) {
//        std::cout << syncAcq << std::endl;
//    }

//    assert(ref_sync_acqs == acqs_read);


//                                                    hub::output::OutputStream( FILE_NAME ) );
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

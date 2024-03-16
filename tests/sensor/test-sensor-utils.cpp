
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

#include <sensor/SensorUtils.hpp>

TEST_CASE( "Sensor utils test" ) {
    TEST_BEGIN()

    const std::string filename = FILE_NAME + "." + HUB_EXTENSION;
    CHECK( FILE_NAME == "test-sensor-utils.cpp" );

    hub::MetaData metaData;
    metaData["parent"] = "sensorName2";
    using Resolution   = hub::format::BGR8;
    const hub::sensor::SensorSpec refSensorSpec( "sensorName", Resolution(), metaData );

    using Resolution2 = hub::format::Y8;
    const hub::sensor::SensorSpec refSensorSpec2( "sensorName2", Resolution2() );

    using Resolution3 = hub::format::Z16;
    const hub::sensor::SensorSpec refSensorSpec3( "sensorName3", Resolution3() );

    using Resolution4  = int;
    metaData["parent"] = "sensorName3";
    const hub::sensor::SensorSpec refSensorSpec4( "sensorName4", Resolution4(), metaData );

    using Resolution5 = bool;
    const hub::sensor::SensorSpec refSensorSpec5( "sensorName5", Resolution5() );

    const auto refAcqs      = test::sensor::generateRefAcqs( 5, 10, refSensorSpec );
    const auto refAcqs2     = test::sensor::generateRefAcqs( 0, 20, refSensorSpec2 );
    const auto refSyncAcqs  = test::sensor::computeSyncAcqs( refAcqs, refAcqs2 );
    const auto refAcqs3     = test::sensor::generateRefAcqs( 0, 5, refSensorSpec3 );
    const auto refAcqs4     = test::sensor::generateRefAcqs( 0, 10, refSensorSpec4 );
    const auto refSyncAcqs2 = test::sensor::computeSyncAcqs( refAcqs4, refAcqs3 );
    const auto refAcqs5     = test::sensor::generateRefAcqs( 5, 5, refSensorSpec5 );

    const std::string rootDir = "test-sensor-utils/";
    std::filesystem::remove_all( rootDir );
    std::filesystem::create_directory( rootDir );

    {
        hub::sensor::OutputSensor outputSensor(
            refSensorSpec,
            hub::output::OutputFile( hub::io::make_header( refSensorSpec ),
                                     rootDir + refSensorSpec.getSensorName() + "." +
                                         HUB_EXTENSION ) );
        outputSensor.fill( refAcqs );
        CHECK( outputSensor.getSpec() == refSensorSpec );
        hub::sensor::OutputSensor outputSensor2(
            refSensorSpec2,
            hub::output::OutputFile( hub::io::make_header( refSensorSpec2 ),
                                     rootDir + refSensorSpec2.getSensorName() + "." +
                                         HUB_EXTENSION ) );
        outputSensor2.fill( refAcqs2 );
        CHECK( outputSensor2.getSpec() == refSensorSpec2 );
        hub::sensor::OutputSensor outputSensor3(
            refSensorSpec3,
            hub::output::OutputFile( hub::io::make_header( refSensorSpec3 ),
                                     rootDir + refSensorSpec3.getSensorName() + "." +
                                         HUB_EXTENSION ) );
        outputSensor3.fill( refAcqs3 );

        hub::sensor::OutputSensor outputSensor4(
            refSensorSpec4,
            hub::output::OutputFile( hub::io::make_header( refSensorSpec4 ),
                                     rootDir + refSensorSpec4.getSensorName() + "." +
                                         HUB_EXTENSION ) );
        outputSensor4.fill( refAcqs4 );

        hub::sensor::OutputSensor outputSensor5(
            refSensorSpec5,
            hub::output::OutputFile( hub::io::make_header( refSensorSpec5 ),
                                     rootDir + refSensorSpec5.getSensorName() + "." +
                                         HUB_EXTENSION ) );
        outputSensor5.fill( refAcqs5 );
    }

    hub::sensor::utils::synchronizePath( rootDir );
    std::cout << "[test-sensor-utils] synchronization done" << std::endl;
    std::cout << "------------------------------------------------------------------------"
              << std::endl;

    const auto syncDir = rootDir + "sync/";

    auto inputFiles = hub::sensor::utils::getInputFiles( syncDir );
    for ( auto& inputFile : inputFiles ) {
        std::cout << "input file : " << inputFile.getFilePath() << std::endl;

        hub::sensor::InputSensor inputSensor { inputFile };
        const auto& sensorSpec = inputSensor.getSpec();
        std::cout << sensorSpec << std::endl;
        if ( sensorSpec == refSensorSpec + refSensorSpec2 ) {
            const auto& syncAcqs = inputSensor.getAllAcquisitions();
            CHECK( refSyncAcqs == syncAcqs );
        }
        else if ( sensorSpec == refSensorSpec4 + refSensorSpec3 ) {
            const auto& syncAcqs = inputSensor.getAllAcquisitions();
            CHECK( refSyncAcqs2 == syncAcqs );
        }
    }

    TEST_END()
}

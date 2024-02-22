
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

#include <sensor/SensorUtils.hpp>


TEST_CASE("Sensor utils test")
{
    TEST_BEGIN()

    const std::string filename = FILE_NAME + ".hub";
    assert(FILE_NAME == "test-sensor-utils.cpp");


    hub::MetaData metaData;
    metaData["parent"] = "sensorName2";
    using Resolution = hub::format::BGR8;
    const hub::sensor::SensorSpec refSensorSpec(
        "sensorName", Resolution(), metaData);

    using Resolution2 = hub::format::Y8;
    const hub::sensor::SensorSpec refSensorSpec2(
        "sensorName2", Resolution2());

    using Resolution3 = hub::format::Z16;
    const hub::sensor::SensorSpec refSensorSpec3(
        "sensorName3", Resolution3());

    using Resolution4 = int;
    metaData["parent"] = "sensorName3";
    const hub::sensor::SensorSpec refSensorSpec4(
        "sensorName4", Resolution4(), metaData);

    using Resolution5 = bool;
    const hub::sensor::SensorSpec refSensorSpec5(
        "sensorName5", Resolution5());

//    hub::io::Archive archive(hub::io::make_header(refSensorSpec));
//    hub::io::Archive archive2(hub::io::make_header(refSensorSpec2));

    const auto refAcqs = test::sensor::generateRefAcqs(5, 10, refSensorSpec);
//    std::cout << "refAcqs" << std::endl;
//    for (const auto & refAcq : refAcqs) {
//        std::cout << refAcq << std::endl;
//    }

    const auto refAcqs2 = test::sensor::generateRefAcqs(0, 20, refSensorSpec2);
//    std::cout << "refAcqs2" << std::endl;
//    for (const auto & refAcq2 : refAcqs2) {
//        std::cout << refAcq2 << std::endl;
//    }

    const auto refSyncAcqs = test::sensor::computeSyncAcqs(refAcqs, refAcqs2);
//    std::cout << "refSyncAcqs" << std::endl;
//    for (const auto & refSyncAcq : refSyncAcqs) {
//        std::cout << refSyncAcq << std::endl;
//    }
    const auto refAcqs3 = test::sensor::generateRefAcqs(0, 5, refSensorSpec3);
    const auto refAcqs4 = test::sensor::generateRefAcqs(0, 10, refSensorSpec4);
    const auto refSyncAcqs2 = test::sensor::computeSyncAcqs(refAcqs4, refAcqs3);
    const auto refAcqs5 = test::sensor::generateRefAcqs(5, 5, refSensorSpec5);

    const std::string rootDir = "test-sensor-utils/";
    std::filesystem::create_directory(rootDir);

//    hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile> outputSensor(sensorSpec, rootDir + sensorSpec.getSensorName() + ".hub");
//    hub::sensor::OutputSensor outputSensor(sensorSpec, archive);
    {
        hub::sensor::OutputSensor outputSensor(refSensorSpec, hub::output::OutputFile(hub::io::make_header(refSensorSpec), rootDir + refSensorSpec.getSensorName() + ".hub"));
        outputSensor.fill(refAcqs);
        assert(outputSensor.getSpec() == refSensorSpec);
//        for (const auto & acq : refAcqs) {
//            outputSensor << acq;
//        }
    //    hub::sensor::OutputSensor outputSensor2(refSensorSpec2, archive2);
        hub::sensor::OutputSensor outputSensor2(refSensorSpec2, hub::output::OutputFile(hub::io::make_header(refSensorSpec2), rootDir + refSensorSpec2.getSensorName() + ".hub"));
        outputSensor2.fill(refAcqs2);
        assert(outputSensor2.getSpec() == refSensorSpec2);
//        for (const auto & acq : refAcqs2) {
//            outputSensor2 << acq;
//        }
        hub::sensor::OutputSensor outputSensor3(refSensorSpec3, hub::output::OutputFile(hub::io::make_header(refSensorSpec3), rootDir + refSensorSpec3.getSensorName() + ".hub"));
        outputSensor3.fill(refAcqs3);

        hub::sensor::OutputSensor outputSensor4(refSensorSpec4, hub::output::OutputFile(hub::io::make_header(refSensorSpec4), rootDir + refSensorSpec4.getSensorName() + ".hub"));
        outputSensor4.fill(refAcqs4);

        hub::sensor::OutputSensor outputSensor5(refSensorSpec5, hub::output::OutputFile(hub::io::make_header(refSensorSpec5), rootDir + refSensorSpec5.getSensorName() + ".hub"));
        outputSensor5.fill(refAcqs5);
    }


//    hub::sensor::InputSensor inputSensor(archive);
//    assert(inputSensor.getSpec() == sensorSpec);

//    hub::sensor::InputSensor inputSensor2(archive2);
//    assert(inputSensor2.getSpec() == refSensorSpec2);

//    std::vector<hub::sensor::InputSensor*> inputSensors{&inputSensor, &inputSensor2};

    hub::sensor::utils::synchronizePath(rootDir);

    const auto syncDir = rootDir + "sync/";

    auto inputFiles = hub::sensor::utils::getInputFiles(syncDir);
    for (auto & inputFile : inputFiles) {
        hub::sensor::InputSensor inputSensor{inputFile};
        const auto & sensorSpec = inputSensor.getSpec();
        if (sensorSpec == refSensorSpec + refSensorSpec2) {
            const auto & syncAcqs = inputSensor.getAllAcquisitions();
            CHECK(refSyncAcqs == syncAcqs);
        }
        else if (sensorSpec == refSensorSpec4 + refSensorSpec3) {
            const auto & syncAcqs = inputSensor.getAllAcquisitions();
            CHECK(refSyncAcqs2 == syncAcqs);
        }
    }


//    auto specAcqs = hub::sensor::utils::synchonizeInputs(archive, archive2);
//    std::cout << "spec acqs : " << specAcqs << std::endl;
//    for (const auto & specAcq : specAcqs) {
//        std::cout << specAcq.m_sensorSpec << std::endl;
//        for (const auto & acq : specAcq.m_acqs) {
//            std::cout << acq << std::endl;
//        }
//        assert(specAcq.m_acqs == refSyncAcqs);
//        assert(specAcq.m_sensorSpec == sensorSpec + sensorSpec2);
//    }

//    test::sensor::checkSynchronize(archive, sensorSpec, refAcqs, archive2, sensorSpec2, refAcqs2, archive, archive2);



    TEST_END()
}

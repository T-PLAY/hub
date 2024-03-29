
#include <filesystem>

#include "test_common.hpp"
// #include "io/test_io_common.hpp"
#include "test_sensor_common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "sensor compat os and arch test" ) {
    // multi os/platform compatibility
    // able to load hub file through differents os and platform
    // test if raw stream data is same using different architectures like x64 x86 arm64,
    // using different os like linux, windows, macos
    TEST_BEGIN()

    // INIT_SERVER

    const std::string meshPath = HUB_PROJECT_DIR "data/assets/";
    // const hub::data::Mesh mesh( meshPath + "Bunny" );
    const hub::data::Mesh mesh( meshPath + "sensor" );

    hub::MetaData metaData;
    metaData["asset"] = mesh;
    metaData["parent"] = "Stanford";

    using Resolution   = hub::MatrixTs<int, bool, hub::MatrixXD<char, 10>>;
    // using Resolution   = int;
    using OutputSensor = hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile>;
    // using OutputSensor = hub::sensor::OutputSensorT<Resolution>;

    using Acquisition  = OutputSensor::Acquisition;
    std::vector<Acquisition> refAcqs;

    const hub::sensor::SensorSpec refSensorSpec( FILE_NAME, Resolution(), metaData );

    const std::string filePath = HUB_PROJECT_DIR "data/files/compatMultiOsArch.hub";

    Acquisition acq;
    auto [start, end] = acq.clocks();
    auto& intRef      = acq.get<int&>();
    auto& boolRef     = acq.get<bool&>();
    auto* charPtr      = acq.get<char*>();

    for ( int i = 0; i < 10; ++i ) {
        start   = i;
        end     = i;
        intRef  = i;
        boolRef = i % 2;
        // intRef2 = i;
        for ( int j = 0; j < 10; ++j ) {
            charPtr[j] = i;
        }
        refAcqs.push_back( acq );
    }

    // std::cout << "-------------------------------- outputSensor -------------------------- " << std::endl;

    // OutputSensor
    // Testing unique multi os/arch file
    if (! std::filesystem::exists(filePath))
    {
        std::cout << "--------------> generating ref file <----------------------" << std::endl;
        OutputSensor outputSensor( refSensorSpec, filePath );
        // OutputSensor outputSensor( refSensorSpec, FILE_NAME, SERVER_PORT );
        for (const auto & acq : refAcqs) {
            outputSensor << acq;
        }
    }

    // std::cout << std::endl;
    // std::cout << "-------------------------------- inputSensor -------------------------- " << std::endl;

    // InputSensor
    {
        hub::sensor::InputSensor inputSensor(hub::input::InputFile{filePath});
        // hub::sensor::InputSensor inputSensor(hub::input::InputStream(FILE_NAME, SERVER_PORT));
        const auto & sensorSpec = inputSensor.getSpec();
        assert(sensorSpec == refSensorSpec);

        auto acqs = inputSensor.getAllAcquisitions();
        for (int i =0; i < acqs.size(); ++i) {
            // std::cout << refAcqs.at(i) << std::endl;
            assert(refAcqs.at(i) == acqs.at(i));
        }
    }

    TEST_END()
}

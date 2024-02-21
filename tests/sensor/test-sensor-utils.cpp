
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE("Sensor utils test")
{
    TEST_BEGIN()

//    const auto file = __FILE__;
//    std::cout << "max \\ : " << std::string( __FILE__ ).find_last_of( '\\' ) << std::endl;
//    std::cout << "max / : " << std::string( "/" __FILE__ ).find_last_of( '/' ) << std::endl;

    const std::string filename = FILE_NAME + ".hub";
    assert(FILE_NAME == "test-sensor-utils.cpp");

    hub::MetaData metaData;
    metaData["parent"] = "parentName";
    using Resolution = hub::format::BGR8;
    const hub::sensor::SensorSpec sensorSpec(
        "sensorName", hub::make_matrix<Resolution>(), metaData);

    {
        hub::sensor::OutputSensorT<Resolution, hub::output::OutputFile> outputSensor(sensorSpec, filename);
        outputSensor.getOutput().setRetain(true);

        auto acq = outputSensor.acqMsg();
        auto& start = acq.start();
        auto& end = acq.end();
        auto& bgr8 = acq.get<hub::format::BGR8&>();

        for (int i = 0; i < 20; ++i) {
            start = i;
            end = i;
            bgr8.b = i;
            bgr8.g = i;
            bgr8.r = i;
            outputSensor << acq;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////

    hub::sensor::InputSensor inputSensor(hub::input::InputFile { filename });

    assert(sensorSpec == inputSensor.getSpec());
    assert(inputSensor.getSpec().getResolution().hasType<hub::format::BGR8>());

    auto acq_read = inputSensor.acqMsg();
    auto& start_read = acq_read.start();
    auto& end_read = acq_read.end();
    auto& bgr8_read = acq_read.get<hub::format::BGR8&>();

    for (int i = 0; i < 20; ++i) {
//        std::cout << "read acq " << i << std::endl;
        inputSensor >> acq_read;
        CHECK(start_read == i);
        CHECK(end_read == i);
        CHECK(bgr8_read.b == i);
        CHECK(bgr8_read.g == i);
        CHECK(bgr8_read.r == i);
    }

    TEST_END()
}

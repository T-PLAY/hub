
#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

TEST_CASE( "SensorSpec test" ) {

    hub::SensorSpec::MetaData metaData;
    hub::SensorSpec::Format format = hub::SensorSpec::Format::BGR8;
    hub::SensorSpec::Dims dims     = { 1 };
    hub::SensorSpec::Resolutions resolutions { { dims, format } };
    hub::SensorSpec sensorSpec {
        "sensorName", { { dims, hub::SensorSpec::Format::BGR8 } }, metaData };

    CHECK(sensorSpec.getSensorName() == "sensorName");
    CHECK(sensorSpec.getAcquisitionSize() == 3);
}

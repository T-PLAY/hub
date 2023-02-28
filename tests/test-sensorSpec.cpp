
#include <catch2/catch_test_macros.hpp>

#include <IO/File.hpp>
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>
//#include <Streamer.hpp>

TEST_CASE( "SensorSpec test" ) {

//    hub::Streamer streamer;
//    streamer.isConnected();

    hub::SensorSpec::MetaData metaData;
//    hub::Format format = hub::Format::BGR8;
    hub::Dims dims     = { 1 };
//    hub::Resolutions resolutions { { dims, format } };
    hub::SensorSpec sensorSpec { "sensorName", { { dims, hub::Format::BGR8 } }, metaData };

    CHECK( sensorSpec.getSensorName() == "sensorName" );
    CHECK( sensorSpec.getAcquisitionSize() == 3 );

}

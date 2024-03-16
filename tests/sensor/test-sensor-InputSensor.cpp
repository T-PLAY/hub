
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputSensor test" ) {
    TEST_BEGIN()

    INIT_SERVER

    hub::MetaData metaData;
    metaData["parent"] = "parentName";

    using Resolution = hub::format::BGR8;

    const hub::sensor::SensorSpec sensorSpec(
        "sensorName", hub::make_matrix<Resolution>(), metaData );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME, SERVER_PORT );
    std::cout << "[test] outputSensor inited" << std::endl;

    auto acq = outputSensor.acqMsg();
    outputSensor.getOutput().setRetain( true );

    auto& start = acq.start();
    auto& end   = acq.end();
    auto& bgr8  = acq.get<hub::format::BGR8&>();
    for ( int i = 0; i < 20; ++i ) {
        start  = i;
        end    = i;
        bgr8.b = i;
        bgr8.g = i;
        bgr8.r = i;
        outputSensor << acq;
    }

    //////////////////////////////////////////////////////////////////////////////////

    hub::sensor::InputSensor inputSensor( hub::input::InputStream( FILE_NAME, SERVER_PORT ) );
    std::cout << "[test] inputSensor inited" << std::endl;

    CHECK( outputSensor.getSpec() == inputSensor.getSpec() );
    CHECK( inputSensor.getSpec().getResolution().hasType<hub::format::BGR8>() );

    auto acq_read    = inputSensor.acqMsg();
    auto& start_read = acq_read.start();
    auto& end_read   = acq_read.end();
    auto& bgr8_read  = acq_read.get<hub::format::BGR8&>();
    for ( int i = 0; i < 20; ++i ) {
        std::cout << "read acq " << i << std::endl;
        inputSensor >> acq_read;
        CHECK( start_read == i );
        CHECK( end_read == i );
        CHECK( bgr8_read.b == i );
        CHECK( bgr8_read.g == i );
        CHECK( bgr8_read.r == i );
    }

    TEST_END()
}

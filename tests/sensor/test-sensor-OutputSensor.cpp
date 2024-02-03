
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "OutputSensor test" ) {
    TEST_BEGIN()

    INIT_SERVER

    hub::MetaData metaData;
    metaData["parent"] = "parentName";

    auto resolution = hub::make_matrix<hub::format::BGR8>();
    assert( resolution.size() != 0 );
    assert( resolution.nType() > 0 );

    const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );
    assert( sensorSpec.getResolution().size() != 0 );
    assert( sensorSpec.getResolution().nType() > 0 );

    hub::sensor::OutputSensor outputSensor( sensorSpec, FILE_NAME, SERVER_PORT );

    assert( outputSensor.getSpec() == sensorSpec );

    auto acq = outputSensor.acqMsg();

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

    TEST_END()
}

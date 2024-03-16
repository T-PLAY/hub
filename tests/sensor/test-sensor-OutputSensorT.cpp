
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "OutputSensorT test" ) {
    TEST_BEGIN()

    INIT_SERVER

    {
        hub::MetaData metaData;
        metaData["parent"] = "parentName";

        using Resolution = hub::format::BGR8;
        const hub::sensor::SensorSpec sensorSpec(
            "sensorName", hub::make_matrix<Resolution>(), metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME, SERVER_PORT );

        auto acq = outputSensor.acqMsg();

        auto& start = acq.start();
        auto& end   = acq.end();
        auto& bgr8  = acq.get<hub::format::BGR8&>();
        for ( int i = 0; i < 100; ++i ) {
            start  = i;
            end    = i;
            bgr8.b = i;
            bgr8.g = i;
            bgr8.r = i;
            outputSensor << acq;
        }
    }

    // Todo fix native

    TEST_END()
}

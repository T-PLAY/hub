
#include "io/test_io_common.hpp"
#include "test_common.hpp"
#include "test_sensor_common.hpp"

#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputOutputSensorStream test" ) {
    TEST_BEGIN()

    INIT_SERVER

    test::sensor::inputOutputSensorBench<hub::input::InputStream, hub::output::OutputStream>(
        FILE_NAME, SERVER_PORT );

    TEST_END()
}


#include "io/test_io_common.hpp"
#include "test_common.hpp"
#include "test_sensor_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "InputOutputSensorArchive test" ) {
    TEST_BEGIN()

    test::sensor::inputOutputSensorAsyncBench<hub::input::InputFile, hub::output::OutputFile>(
        FILE_NAME + "." + HUB_EXTENSION );

    TEST_END()
}

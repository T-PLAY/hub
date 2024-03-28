
#include "io/test_io_common.hpp"
#include "test_common.hpp"
#include "test_sensor_common.hpp"

#include <io/input/InputStream.hpp>
#include <io/output/OutputStream.hpp>

TEST_CASE( "sensor : InputOutputSensorSync" ) {
    TEST_BEGIN()

    static_assert( !hub::Endable<hub::io::InputOutputSocket> );
    static_assert( !hub::Endable<hub::input::InputStream> );

    INIT_SERVER

    using Resolution = hub::format::Y8;
    const hub::sensor::SensorSpec sensorSpec( "sensorName", hub::make_matrix<Resolution>() );
    const auto acqs = test::sensor::generateRefAcqs( 0, 10, sensorSpec );
    hub::output::OutputStream outputStream(
        hub::io::make_header( sensorSpec ), FILE_NAME, SERVER_PORT );

    // Todo avoid sleep on macos, concurrency bug when 2 outputStream login in same time (only llvm on macos)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    using Resolution2 = hub::format::Z16;
    const hub::sensor::SensorSpec sensorSpec2( "sensorName2", hub::make_matrix<Resolution2>() );
    const auto acqs2 = test::sensor::generateRefAcqs( 5, 10, sensorSpec2 );
    hub::output::OutputStream outputStream2(
        hub::io::make_header( sensorSpec2 ), FILE_NAME + "2", SERVER_PORT );

    hub::input::InputStream inputStream( FILE_NAME, SERVER_PORT );

    hub::input::InputStream inputStream2( FILE_NAME + "2", SERVER_PORT );

    test::sensor::checkSynchronize( outputStream,
                                    sensorSpec,
                                    acqs,
                                    outputStream2,
                                    sensorSpec2,
                                    acqs2,
                                    inputStream,
                                    inputStream2 );

    using Resolution3 = hub::format::Y8;
    const hub::sensor::SensorSpec sensorSpec3( "sensorName3", hub::make_matrix<Resolution3>() );
    const auto acqs3 = test::sensor::generateRefAcqs( 10, 10, sensorSpec3 );
    hub::output::OutputStream outputStream3(
        hub::io::make_header( sensorSpec3 ), FILE_NAME + "3", SERVER_PORT );

    hub::input::InputStream inputStream3( FILE_NAME + "3", SERVER_PORT );

    test::sensor::checkSynchronize( outputStream2,
                                    sensorSpec2,
                                    acqs2,
                                    outputStream3,
                                    sensorSpec3,
                                    acqs3,
                                    inputStream2,
                                    inputStream3 );

    TEST_END()
}

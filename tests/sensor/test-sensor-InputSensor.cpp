// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT
// #define HUB_DEBUG_INPUT_OUTPUT

#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

// #undef HUB_BUILD_SERVER

TEST_CASE( "InputSensor test" ) {

#ifdef HUB_BUILD_SERVER
    INIT_SERVER
#else
    hub::io::Archive archive;
#endif

    hub::sensor::SensorSpec::MetaData metaData;
    metaData["parent"] = "parentName";

    using Resolution = hub::sensor::format::BGR8;
    // auto resolution = hub::make_matrix<hub::sensor::format::BGR8>();

    const hub::sensor::SensorSpec sensorSpec( "sensorName", metaData );
    // const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );

#ifdef HUB_BUILD_SERVER
    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec,
    // hub::sensor::OutputSensor outputSensor( sensorSpec,
                                            hub::output::OutputStream( FILE_NAME, SERVER_PORT ) );
#else
    // hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, archive );
    hub::sensor::OutputSensor outputSensor( sensorSpec, archive );
#endif
    std::cout << "[test] outputSensor inited" << std::endl;

    auto acq = outputSensor.acq();
    outputSensor.getOutput().setRetain( true );

    // auto& start = acq.start();
    auto& start = acq.get<hub::sensor::Clock&>();
    // auto& end   = acq.end();
    auto& end   = acq.get<hub::sensor::Clock&, 1>();
    auto& bgr8  = acq.get<hub::sensor::format::BGR8&>();
    // auto& intRef  = acq.get<int&>();
    for ( int i = 0; i < 10; ++i ) {
        start  = i;
        end    = i;
        bgr8.b = i;
        bgr8.g = i;
        bgr8.r = i;
        outputSensor << acq;
    }

    //////////////////////////////////////////////////////////////////////////////////

#ifdef HUB_BUILD_SERVER
    hub::sensor::InputSensor inputSensor( hub::input::InputStream( FILE_NAME, SERVER_PORT ) );
#else
    hub::sensor::InputSensor inputSensor( archive );
#endif
    std::cout << "[test] inputSensor inited" << std::endl;

    assert( outputSensor.getSpec() == inputSensor.getSpec() );
    assert( inputSensor.getSpec().getResolution().hasType<hub::sensor::format::BGR8>() );

    auto acq_read = inputSensor.acq();
    // acq.clear();
    // assert( acq == acq_read );
    auto& start_read = acq_read.start();
    auto& end_read   = acq_read.end();
    auto& bgr8_read  = acq_read.get<hub::sensor::format::BGR8&>();
    for ( int i = 0; i < 10; ++i ) {
        // std::cout << "read acq " << i << std::endl;
        inputSensor >> acq_read;
        CHECK( start_read == i );
        CHECK( end_read == i );
        CHECK( bgr8_read.b == i );
        CHECK( bgr8_read.g == i );
        CHECK( bgr8_read.r == i );
    }

    // std::thread inputThread([&]() {
    //     std::cout << "[test][InputThread] reading acq" << std::endl;
    //     inputSensor >> acq_read;
    //     std::cout << "read acq " << acq_read << std::endl;
    // });

    // inputThread.join();

    // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    return;
}

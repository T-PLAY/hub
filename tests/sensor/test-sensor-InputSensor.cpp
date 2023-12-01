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

TEST_CASE( "InputOutputSensor test" ) {

#ifdef HUB_BUILD_SERVER
    INIT_SERVER
#else
    hub::io::Archive archive;
#endif

    hub::sensor::SensorSpec::MetaData metaData;
    metaData["parent"] = "parentName";

    // using Resolution = hub::sensor::format::BGR8;
    auto resolution = hub::make_matrix<hub::sensor::format::BGR8>();

    const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );

#ifdef HUB_BUILD_SERVER
    // hub::sensor::OutputSensorT<Resolution> outputSensor(
        // sensorSpec, hub::output::OutputStream( FILE_NAME, SERVER_PORT ) );
    hub::sensor::OutputSensor outputSensor(
        sensorSpec, hub::output::OutputStream( FILE_NAME, SERVER_PORT ) );
#else
    // hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, archive );
    hub::sensor::OutputSensor outputSensor( sensorSpec, archive );
#endif

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "---------------------------------- outputSensor inited --------------------------------------------" << std::endl;

    std::cout << "[test] initing inputSensor ..." << std::endl;
#ifdef HUB_BUILD_SERVER
    hub::sensor::InputSensor inputSensor(hub::input::InputStream(FILE_NAME, SERVER_PORT));
#else
    hub::sensor::InputSensor inputSensor(archive);
#endif
    std::cout << "[test] inputSensor inited" << std::endl;

    // assert(outputSensor.getSpec() == inputSensor.getSpec());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "----------------------------------- inputSensor inited -------------------------------------------" << std::endl;

    // hub::sensor::AcquisitionT<Resolution> acq;

    // auto& start = acq.start();
    // auto& end   = acq.end();
    // auto& bgr8  = acq.get<hub::sensor::format::BGR8&>();
    // // hub::Data_t * data = acq.data();
    // // hub::Size_t size = acq.size();
    // for ( int i = 0; i < 10; ++i ) {
    //     start  = i;
    //     end    = i;
    //     bgr8.b = i;
    //     bgr8.g = i;
    //     bgr8.r = i;
    //     // outputSensor.
    //     outputSensor << acq;
    //     // auto & output = outputSensor.getOutput();
    //     // dynamic_cast<hub::output::OutputStream&>(output).write(data, size);
    // }

    // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    return;
}

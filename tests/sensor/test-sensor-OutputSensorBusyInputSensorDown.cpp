// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "OutputSensor sending acq with disconnect test" ) {
    TEST_BEGIN()

// #ifdef HUB_BUILD_SERVER
    INIT_SERVER
// #else
    // hub::io::Archive archive;
// #endif

    {
        hub::MetaData metaData;
        metaData["parent"] = "parentName";

        using Resolution = hub::sensor::format::BGR8;
        // auto resolution = hub::make_matrix<hub::sensor::format::BGR8>();

        const hub::sensor::SensorSpec sensorSpec( "sensorName", hub::make_matrix<Resolution>(), metaData );
        // const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );

// #ifdef HUB_BUILD_SERVER
        hub::sensor::OutputSensorT<Resolution> outputSensor(
            // sensorSpec,
            // hub::sensor::OutputSensor outputSensor( sensorSpec,
            hub::output::OutputStream( hub::io::make_header(sensorSpec),  FILE_NAME, SERVER_PORT ) );
// #else
        // hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, archive );
        // hub::sensor::OutputSensor outputSensor( sensorSpec, archive );
// #endif
        std::cout << "[test] outputSensor inited" << std::endl;

        auto acq = outputSensor.acqMsg();
        // outputSensor.getOutput().setRetain( true );

        // auto& start = acq.start();
        auto& start = acq.get<hub::sensor::Clock&>();
        // auto& end   = acq.end();
        auto& end  = acq.get<hub::sensor::Clock&, 1>();
        auto& bgr8 = acq.get<hub::sensor::format::BGR8&>();
        // auto& intRef  = acq.get<int&>();
        std::atomic<bool> exit = false;
        auto outputThread      = std::thread( [&]() {
            int iAcq = 0;
            while ( !exit ) {
                // for ( int i = 0; i < 10; ++i ) {
                start  = iAcq;
                end    = iAcq;
                bgr8.b = iAcq;
                bgr8.g = iAcq;
                bgr8.r = iAcq;
                outputSensor << acq;
                // }
                ++iAcq;
                if ( iAcq >= 10 ) {
                    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                }
            }
        } );

        //////////////////////////////////////////////////////////////////////////////////

        {
// #ifdef HUB_BUILD_SERVER
            hub::sensor::InputSensor inputSensor(
                hub::input::InputStream( FILE_NAME, SERVER_PORT ) );
// #else
            // hub::sensor::InputSensor inputSensor( archive );
// #endif
            std::cout << "[test] inputSensor inited" << std::endl;

            assert( outputSensor.getSpec() == inputSensor.getSpec() );
            assert( inputSensor.getSpec().getResolution().hasType<hub::sensor::format::BGR8>() );

            auto acq_read = inputSensor.acqMsg();
            // acq.clear();
            // assert( acq == acq_read );
            auto& start_read = acq_read.start();
            auto& end_read   = acq_read.end();
            auto& bgr8_read  = acq_read.get<hub::sensor::format::BGR8&>();
            for ( int i = 0; i < 10; ++i ) {
                inputSensor >> acq_read;
                std::cout << "read acq " << acq_read << std::endl;
                // CHECK( start_read == i );
                // CHECK( end_read == i );
                // CHECK( bgr8_read.b == i );
                // CHECK( bgr8_read.g == i );
                // CHECK( bgr8_read.r == i );
            }
            std::cout << "[test] end inputSensor ------------------------------" << std::endl;
        } // end inputSensor
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        exit = true;
        outputThread.join();

        std::cout << "[test] end outputSensor ------------------------------" << std::endl;
    } // end outputSensor
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    // hub::io::Archive archive;
    // test::sensor::inputOutputSensorBench( archive, archive );
    TEST_END()
}

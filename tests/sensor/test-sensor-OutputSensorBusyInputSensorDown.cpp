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

    INIT_SERVER

    {
        hub::MetaData metaData;
        metaData["parent"] = "parentName";

        using Resolution = hub::sensor::format::BGR8;
        // auto resolution = hub::make_matrix<hub::sensor::format::BGR8>();

        const hub::sensor::SensorSpec sensorSpec(
            "sensorName", hub::make_matrix<Resolution>(), metaData );
        // const hub::sensor::SensorSpec sensorSpec( "sensorName", resolution, metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor( hub::output::OutputStream(
            hub::io::make_header( sensorSpec ), FILE_NAME, SERVER_PORT ) );
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
                start  = iAcq;
                end    = iAcq;
                bgr8.b = iAcq;
                bgr8.g = iAcq;
                bgr8.r = iAcq;
                outputSensor << acq;
                ++iAcq;
                std::cout << "\033[31m-\033[0m";
                if ( iAcq >= 100 ) {
                    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
                }
            }
            std::cout << std::endl;
        } );

        //////////////////////////////////////////////////////////////////////////////////

        {
            hub::sensor::InputSensor inputSensor(
                hub::input::InputStream( FILE_NAME, SERVER_PORT ) );
            std::cout << "[test] inputSensor inited" << std::endl;

            assert( outputSensor.getSpec() == inputSensor.getSpec() );
            assert( inputSensor.getSpec().getResolution().hasType<hub::sensor::format::BGR8>() );

            auto acq_read    = inputSensor.acqMsg();
            auto& start_read = acq_read.start();
            auto& end_read   = acq_read.end();
            auto& bgr8_read  = acq_read.get<hub::sensor::format::BGR8&>();
            for ( int i = 0; i < 100; ++i ) {
                inputSensor >> acq_read;
                // std::cout << "read acq " << acq_read << std::endl;
                std::cout << "\033[32m+\033[0m";
                const int value = start_read;
                // CHECK( start_read == i );
                CHECK( end_read == value );
                CHECK( bgr8_read.b == value );
                CHECK( bgr8_read.g == value );
                CHECK( bgr8_read.r == value );
                    // std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
            }
            std::cout << std::endl;
            std::cout << "[test] end inputSensor ------------------------------" << std::endl;
        } // end inputSensor
        // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

        exit = true;
        outputThread.join();

        std::cout << "[test] end outputSensor ------------------------------" << std::endl;
    } // end outputSensor
    // std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    // hub::io::Archive archive;
    // test::sensor::inputOutputSensorBench( archive, archive );
    TEST_END()
}

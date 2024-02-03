
#include "io/test_io_common.hpp"
#include "sensor/test_sensor_common.hpp"
#include "test_common.hpp"

#include <core/io/Archive.hpp>
#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

TEST_CASE( "OutputSensor sending acq with disconnect test" ) {
    TEST_BEGIN()

    INIT_SERVER

    constexpr auto nAcq = 100;

    {
        hub::MetaData metaData;
        metaData["parent"] = "parentName";

        using Resolution = hub::format::BGR8;

        const hub::sensor::SensorSpec sensorSpec(
            "sensorName", hub::make_matrix<Resolution>(), metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME, SERVER_PORT );
        std::cout << "[test] outputSensor inited" << std::endl;

        auto acq = outputSensor.acqMsg();

        auto& start            = acq.get<hub::sensor::Clock&>();
        auto& end              = acq.get<hub::sensor::Clock&, 1>();
        auto& bgr8             = acq.get<hub::format::BGR8&>();
        std::atomic<bool> exit = false;
        auto outputThread      = std::thread( [&]() {
            int iAcq = 0;
            while ( !exit ) {
                start  = iAcq;
                end    = iAcq;
                bgr8.b = iAcq % 256;
                bgr8.g = iAcq % 256;
                bgr8.r = iAcq % 256;
                outputSensor << acq;
                ++iAcq;
                std::cout << "\033[31m-\033[0m";
                std::this_thread::sleep_for( std::chrono::microseconds( 100 ) );
            }
            std::cout << std::endl;
        } );

        //////////////////////////////////////////////////////////////////////////////////

        {
            hub::sensor::InputSensor inputSensor(
                hub::input::InputStream( FILE_NAME, SERVER_PORT ) );
            std::cout << "[test] inputSensor inited" << std::endl;

            assert( outputSensor.getSpec() == inputSensor.getSpec() );
            assert( inputSensor.getSpec().getResolution().hasType<hub::format::BGR8>() );

            auto acq_read    = inputSensor.acqMsg();
            auto& start_read = acq_read.start();
            auto& end_read   = acq_read.end();
            auto& bgr8_read  = acq_read.get<hub::format::BGR8&>();
            for ( int i = 0; i < nAcq; ++i ) {
                inputSensor >> acq_read;
                std::cout << "\033[32m+\033[0m";
                const auto start = start_read;
                // CHECK( start_read == i );
                CHECK( end_read == start );
                CHECK( bgr8_read.b == start % 256 );
                CHECK( bgr8_read.g == start % 256 );
                CHECK( bgr8_read.r == start % 256 );
            }
            std::cout << std::endl;
            std::cout << "[test] end inputSensor ------------------------------" << std::endl;
        } // end inputSensor

        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        exit = true;
        outputThread.join();

        std::cout << "[test] end outputSensor ------------------------------" << std::endl;
    } // end outputSensor

    TEST_END()
}


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
        for ( int i = 0; i < 10; ++i ) {
            start  = i;
            end    = i;
            bgr8.b = i;
            bgr8.g = i;
            bgr8.r = i;
            outputSensor << acq;
        }
    }

    {
        hub::MetaData metaData;
        metaData["manufactor"] = "UPS";
        constexpr int width    = 512;
        constexpr int height   = 512;
        using ResolutionCam    = hub::MatrixXD<hub::format::RGB8, width, height>;
        using Resolution       = hub::MatrixTs<hub::format::Dof6, ResolutionCam>;
        const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution(), metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME, SERVER_PORT );
        auto acq                 = outputSensor.acqMsg();
        auto& start              = acq.start();
        auto& end                = acq.end();
        auto* imgData            = acq.get<hub::format::RGB8*>();
        constexpr size_t imgSize = width * height;
        auto& dof6               = acq.get<hub::format::Dof6&>();
        dof6                     = hub::format::Dof6();
        std::cout << "offSet dof6 : " << acq.getOffset<hub::format::Dof6>() << std::endl;
        std::cout << "offSet image : " << acq.getOffset<hub::format::RGB8>() << std::endl;
        static_assert( acq.getOffset<hub::format::Dof6>() == 16 );
        static_assert( acq.getOffset<hub::format::RGB8>() == 44 );
        //static_assert( acq.size() == 8 + 8 + 24 + 512 * 512 * 3 );
        //static_assert( acq.size() == 8 + 8 + 24 + 512 * 512 * 3 );
        std::cout << "acq size : " << acq.size() << std::endl;
    }


    TEST_END()
}

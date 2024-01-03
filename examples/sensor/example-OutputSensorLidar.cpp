
#include <sensor/OutputSensor.hpp>

int main() {

    constexpr auto fps = 40.0;

    constexpr auto width  = 640;
    constexpr auto height = 480;
    using ResRGB          = hub::MatrixXD<hub::format::RGB8, width, height>;
    using ResPoint        = hub::MatrixXD<hub::format::XYZ32F, width, height>;
    using Resolution      = hub::MatrixTs<ResRGB, ResPoint>;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    // using Resolution = ResPoint;
    // using Resolution = ResRGB;
    // using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    Resolution().getMatrix();

    hub::MetaData metaData;
    metaData["manufactor"] = "My company";
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution().getMatrix(), metaData );
    // hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, "Lidar stream" );
    auto acq        = outputSensor.acqMsg();
    auto& start     = acq.start();
    auto& end       = acq.end();
    // auto* imgData   = acq.get<hub::format::RGB8*>();
    auto* pointData = acq.get<hub::format::XYZ32F*>();
    // auto* imgData = acq.get<hub::format::BGR8*>();

    // hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, "streamName" );

    // auto acq      = outputSensor.acqMsg();
    // auto& start   = acq.start();
    // auto& end     = acq.end();
    // static_assert( acq.size() == 2 * sizeof( hub::sensor::Clock ) + width * height * 3 );

    constexpr size_t imgSize = width * height;

    size_t dec = 0;
    while ( true ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                const auto idx = i * width + j;
                assert( idx < imgSize );
                // imgData[idx].b = ( i + j + dec ) % 256;
                // imgData[idx].g = ( i + j + dec ) % 256;
                // imgData[idx].r = ( i + j + dec ) % 256;
            }
        }
        end = hub::sensor::getClock();
        outputSensor << acq;

        ++dec;

        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / fps ) );
        std::this_thread::sleep_until( endClock );
    }

    return 0;
}

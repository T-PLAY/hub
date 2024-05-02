

#include <sensor/OutputSensor.hpp>

int main() {

    constexpr auto fps = 40.0;

    constexpr auto width  = 640;
    constexpr auto height = 480;
    constexpr size_t imgSize = width * height;

    using Resolution      = hub::MatrixXD<hub::format::BGR8, width, height>;
    hub::MetaData metaData;
    metaData["manufactor"] = "Your company";
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, "streamName" );

    auto acq      = outputSensor.acqMsg();
    auto& start   = acq.start();
    auto& end     = acq.end();
    auto* imgData = acq.get<hub::format::BGR8*>();

    size_t dec = 0;
    while ( true ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                const auto idx = i * width + j;
                assert( idx < imgSize );
                imgData[idx].b = ( i + j + dec ) % 256;
                imgData[idx].g = ( i + j + dec ) % 256;
                imgData[idx].r = ( i + j + dec ) % 256;
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

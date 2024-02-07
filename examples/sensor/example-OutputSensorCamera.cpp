
#include <thread>

#include <sensor/OutputSensor.hpp>

int main() {

    hub::MetaData metaData;
    metaData["manufactor"] = "UPS";
    constexpr int width    = 640;
    constexpr int height   = 480;
    using Resolution       = hub::MatrixXD<hub::format::RGB8, 640, 480>;
    const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution(), metaData );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME );
    auto acq                 = outputSensor.acqMsg();
    auto& start              = acq.start();
    auto& end                = acq.end();
    auto* imgData            = acq.get<hub::format::RGB8*>();
    constexpr size_t imgSize = width * height;

    constexpr auto maxFps = 40.0;

    int dec = 0;
    while ( 1 ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                const auto idx = i * width + j;
                assert( idx < imgSize );
                imgData[idx].r = ( i + j + dec ) / 5 % 256;
                imgData[idx].g = ( i + dec ) % 256;
                imgData[idx].b = ( j + dec ) * 2 % 256;
            }
        }
        end = hub::sensor::getClock();
        dec += 50;

        // std::cout << "acq: " << acq << std::endl;
        outputSensor << acq;

        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
        std::this_thread::sleep_until( endClock );
    }

    return 0;
}

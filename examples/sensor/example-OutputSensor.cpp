

#include <queue>
#include <string>

#include <sensor/OutputSensor.hpp>

int main() {

#if defined( WIN32 ) || defined( DEBUG )
    constexpr auto maxFps = 40.0;
#else
    constexpr auto maxFps = 40.0;
#endif

    constexpr auto width  = 640;
    constexpr auto height = 480;

    using Resolution = hub::MatrixXD<hub::sensor::format::BGR8, width, height>;
    hub::MetaData metaData;
    metaData["manufactor"] = "My company";
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );
    std::cout << "sensor spec : " << sensorSpec << std::endl;

    hub::sensor::OutputSensorT<Resolution> outputSensor(
        hub::output::OutputStream( hub::io::make_header( sensorSpec ), "streamName" ) );
    // hub::sensor::OutputSensorT<Resolution> outputSensor(
        // hub::output::OutputStream( hub::io::make_header( sensorSpec ), 5000 ) );

    std::cout << "sensor spec : " << outputSensor.getSpec() << std::endl;

    auto acq                           = outputSensor.acqMsg();
    auto& start                        = acq.start();
    auto& end                          = acq.end();
    hub::sensor::format::BGR8* imgData = acq.get<hub::sensor::format::BGR8*>();
    static_assert( acq.Size == 2 * sizeof( hub::sensor::Clock ) + width * height * 3 );

    constexpr size_t imgSize = width * height;

    int dec = 0;
    while ( true ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                assert( i * width + j < imgSize );
                imgData[i * width + j].b = ( i + j + dec ) % 256;
                imgData[i * width + j].g = ( i + j + dec ) % 256;
                imgData[i * width + j].r = ( i + j + dec ) % 256;
            }
        }
        // end = hub::sensor::getClock();
        end = start;
        outputSensor << acq;

        // dec += 50;
        ++dec;
        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
        std::this_thread::sleep_until( endClock );
    }

    return 0;
}

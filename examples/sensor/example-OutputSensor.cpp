

#include <queue>
#include <string>

#include <sensor/OutputSensor.hpp>

int main() {

    static_assert(hub::isMatrix<hub::MatrixXD<int, 640, 480>>);
    static_assert(hub::isMatrix<hub::MatrixTs<int, double>>);
    static_assert(hub::isMatrix<hub::Matrix>);

    constexpr auto width  = 640;
    constexpr auto height = 480;

    using Resolution = hub::MatrixXD<hub::sensor::format::BGR8, width, height>;
    hub::MetaData metaData;
    metaData["manufactor"] = "My company";
    // hub::sensor::SensorSpec sensorSpec( "sensorName", hub::make_matrix<Resolution>(), metaData );
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

    hub::sensor::OutputSensorT<Resolution> outputSensor(
        hub::output::OutputStream( hub::io::make_header( sensorSpec ), FILE_NAME ) );

    auto acq                           = outputSensor.acqMsg();
    auto& start                        = acq.start();
    auto& end                          = acq.end();
    hub::sensor::format::BGR8* imgData = acq.get<hub::sensor::format::BGR8*>();
    static_assert( acq.Size == 2 * sizeof( hub::sensor::Clock ) + width * height * 3 );

    constexpr size_t imgSize = width * height;

    int dec = 0;
    while ( true ) {

        const auto startClock = std::chrono::high_resolution_clock::now();

        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                assert( i * width + j < imgSize );
                imgData[i * width + j].b = ( i + j + dec ) / 5 % 256;
                imgData[i * width + j].g = ( i + j + dec ) / 5 % 256;
                imgData[i * width + j].r = ( i + j + dec ) / 5 % 256;
            }
        }

#if defined( WIN32 ) || defined( DEBUG )
        const auto maxFps = 40.0;
#else
        const auto maxFps = 40.0;
#endif

        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );

        start =
            std::chrono::duration_cast<std::chrono::microseconds>( startClock.time_since_epoch() )
                .count();
        end = std::chrono::duration_cast<std::chrono::microseconds>( endClock.time_since_epoch() )
                  .count();
        dec += 50;

        outputSensor << acq;

        std::this_thread::sleep_until( endClock );
    }

    return 0;
}

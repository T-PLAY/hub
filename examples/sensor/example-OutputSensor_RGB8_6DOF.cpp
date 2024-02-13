
#include <thread>

#include <sensor/OutputSensor.hpp>

int main() {

    hub::MetaData metaData;
    metaData["manufactor"] = "UPS";
    constexpr int width    = 512;
    constexpr int height   = 512;
    using ResolutionCam       = hub::MatrixXD<hub::format::RGB8, width, height>;
    static_assert( ResolutionCam::Size == 512 * 512 * 3 );
    using Resolution = hub::MatrixTs<ResolutionCam, hub::format::Dof6>;
    static_assert( Resolution::Size == 512 * 512 * 3 + 28 );
    const hub::sensor::SensorSpec sensorSpec( FILE_NAME, Resolution(), metaData );

    const auto matrix = Resolution().getMatrix();
    assert( matrix.size() == Resolution::Size );

    hub::sensor::OutputSensorT<Resolution> outputSensor( sensorSpec, FILE_NAME );
    auto acq                 = outputSensor.acqMsg();
    auto& start              = acq.start();
    auto& end                = acq.end();
    auto* imgData            = acq.get<hub::format::RGB8*>();
    constexpr size_t imgSize = width * height;
    auto& dof6               = acq.get<hub::format::Dof6&>();
    dof6                     = hub::format::Dof6();

    constexpr auto maxFps = 40.0;

    int dec = 0;
    const auto third = width / 3;

    while ( 1 ) {
        const auto startClock = std::chrono::high_resolution_clock::now();

        start = hub::sensor::getClock();
        dof6.y = - dec % 100;
        assert( dof6.w0 == 1 );
        assert( dof6.w1 == 0 );
        assert( dof6.w2 == 0 );
        assert( dof6.w3 == 0 );
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                const auto idx = i * width + j;
                assert( idx < imgSize );
                imgData[idx].r = ( i + j + dec ) % 128;
                imgData[idx].g = ( i + j + dec ) % 128;
                imgData[idx].b = ( i + j + dec ) % 128;
                if ( j < third ) { 
                    imgData[idx].r += 128; 
                }
                else if (j < 2 * third) {
                    imgData[idx].r += 128;
                    imgData[idx].g += 128;
                    imgData[idx].b += 128;
                }
                else 
                {
                    imgData[idx].b += 128; 
                }
            }
        }
        end = hub::sensor::getClock();
        ++dec;

        // std::cout << "acq: " << acq << std::endl;
        outputSensor << acq;

        const auto endClock = startClock + std::chrono::microseconds( (int)( 1'000'000 / maxFps ) );
        std::this_thread::sleep_until( endClock );
    }

    return 0;
}
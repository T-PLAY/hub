
#include <string>

#include <sensor/InputSensor.hpp>

int main() {

    constexpr auto width  = 640;
    constexpr auto height = 480;

    using Resolution = hub::MatrixXD<hub::format::BGR8, width, height>;
    hub::MetaData metaData;
    metaData["manufactor"] = "My company";
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

    ////////////////

    hub::sensor::InputSensor inputSensor( hub::input::InputStream( "streamName" ) );

    std::cout << "sensor spec : " << inputSensor.getSpec() << std::endl;
    assert( sensorSpec == inputSensor.getSpec() );

    auto acq                   = inputSensor.acqMsg();
    auto& start                = acq.start();
    auto& end                  = acq.end();
    hub::format::BGR8* imgData = acq.get<hub::format::BGR8*>();
    assert( acq.size() == 2 * sizeof( hub::sensor::Clock ) + width * height * 3 );

    constexpr size_t imgSize = width * height;
    int dec                  = 0;

    while ( true ) {
        inputSensor >> acq;
        std::cout << "receive acq : " << acq << std::endl;

        dec = imgData[0].b;
        assert( start == end );
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                assert( i * width + j < imgSize );
                assert( imgData[i * width + j].b == ( i + j + dec ) % 256 );
                assert( imgData[i * width + j].g == ( i + j + dec ) % 256 );
                assert( imgData[i * width + j].r == ( i + j + dec ) % 256 );
            }
        }
    }
}

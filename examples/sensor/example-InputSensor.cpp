
#include <string>

#include <sensor/InputSensor.hpp>

int main() {

    constexpr auto width  = 640;
    constexpr auto height = 480;

    using Resolution = hub::MatrixXD<hub::sensor::format::BGR8, width, height>;
    hub::MetaData metaData;
    metaData["manufactor"] = "My company";
    hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

    ////////////////

    // hub::sensor::InputSensor inputSensor(hub::input::InputStream(5000));
    hub::sensor::InputSensor inputSensor(hub::input::InputStream("streamName"));

    std::cout << "sensor spec : " << inputSensor.getSpec() << std::endl;
    assert(sensorSpec == inputSensor.getSpec());

    auto acq = inputSensor.acqMsg();
    auto& start                        = acq.start();
    auto& end                          = acq.end();
    hub::sensor::format::BGR8* imgData = acq.get<hub::sensor::format::BGR8*>();
    assert( acq.size() == 2 * sizeof( hub::sensor::Clock ) + width * height * 3 );

    constexpr size_t imgSize = width * height;
    int dec = 0;

    while ( true ) {
        inputSensor >> acq;
        std::cout << "receive acq : " << acq << std::endl;

        dec = imgData[0].b;
        assert(start == end);
        for ( int i = 0; i < height; ++i ) {
            for ( int j = 0; j < width; ++j ) {
                assert( i * width + j < imgSize );
                assert(imgData[i * width + j].b == ( i + j + dec ) % 256);
                assert(imgData[i * width + j].g == ( i + j + dec ) % 256);
                assert(imgData[i * width + j].r == ( i + j + dec ) % 256);
            }
        }
    }



    // return 0;

    // const std::string ipv4 = "127.0.0.1";
    // const int port = 4042;


////    const hub::Input & input = hub::Input(); // abstract class
//    hub::sensor::InputSensor inputSensor2(hub::input::InputStream("streamName", ipv4, port));
//    hub::sensor::InputSensor inputSensor3(hub::input::InputStreamServer("streamName", ipv4, port));
////    const hub::Input & input4 = hub::input::InputStreamInterface("streamName", ipv4, port); // abstract class
//    hub::sensor::InputSensor inputSensor5(hub::input::InputSyncStream("streamName", "streamName2", ipv4, port));
//    hub::sensor::InputSensor inputSensor6(hub::input::InputSyncStreamInterface<hub::input::InputStreamServer>("streamName", "streamName2", ipv4, port));
//#ifdef HUB_BUILD_MQTT
//    hub::sensor::InputSensor inputSensor7(hub::input::InputSyncStreamInterface<hub::input::InputStreamMqtt>("streamName", "streamName2", ipv4, port));
//#endif
//    hub::sensor::InputSensor inputSensor8(hub::input::InputFile("filePath"));
//    std::vector<int> vs;
//#if CPLUSPLUS_VERSION < 17
//    const auto & inputSensor9 = hub::sensor::InputSensor(hub::input::InputMemory<decltype(vs)>(vs));
//#else
//    const auto & inputSensor9 = hub::sensor::InputSensor(hub::input::InputMemory(vs));
//#endif

}

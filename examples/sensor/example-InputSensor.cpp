
#include <string>

//#include <sensor/InputSensor.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


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



#include <InputSensor.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


//    const hub::Input & input = hub::Input(); // abstract class
    hub::InputSensor inputSensor2(hub::input::InputStream("streamName", ipv4, port));
    hub::InputSensor inputSensor3(hub::input::InputStreamServer("streamName", ipv4, port));
//    const hub::Input & input4 = hub::input::InputStreamInterface("streamName", ipv4, port); // abstract class
    hub::InputSensor inputSensor5(hub::input::InputSyncStream<>("streamName", "streamName2", ipv4, port));
    hub::InputSensor inputSensor6(hub::input::InputSyncStream<hub::input::InputStreamServer>("streamName", "streamName2", ipv4, port));
    hub::InputSensor inputSensor7(hub::input::InputSyncStream<hub::input::InputStreamMqtt>("streamName", "streamName2", ipv4, port));
    hub::InputSensor inputSensor8(hub::input::InputFile("filePath"));
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    const auto & inputSensor9 = hub::InputSensor(hub::input::InputMemory<decltype(vs)>(vs));
#else
    const auto & inputSensor9 = hub::InputSensor(hub::input::InputMemory(vs));
#endif

}

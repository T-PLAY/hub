

#include <io/input/Input.hpp>

#include <io/input/InputStream.hpp>
//#include <io/input/InputStreamInterface.hpp>
#include <io/input/InputStreamServer.hpp>
#include <io/input/InputSyncStream.hpp>
#include <io/input/InputFile.hpp>
#include <io/input/InputMemory.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


//    const hub::Input & input = hub::Input(); // abstract class
    const hub::Input & input2 = hub::input::InputStream("streamName", ipv4, port);
    const hub::Input & input3 = hub::input::InputStreamServer("streamName", ipv4, port);
//    const hub::Input & input4 = hub::input::InputStreamInterface("streamName", ipv4, port); // abstract class
    const hub::Input & input5 = hub::input::InputSyncStream("streamName", "streamName2", ipv4, port);
    const hub::Input & input6 = hub::input::InputSyncStreamInterface<hub::input::InputStreamServer>("streamName", "streamName2", ipv4, port);
#ifdef HUB_BUILD_MQTT
    const hub::Input & input7 = hub::input::InputSyncStreamInterface<hub::input::InputStreamMqtt>("streamName", "streamName2", ipv4, port);
#endif
    const hub::Input & input8 = hub::input::InputFile("filePath");
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    const hub::Input & input9 = hub::input::InputMemory<decltype(vs)>(vs);
#else
    const hub::Input & input9 = hub::input::InputMemory(vs);
#endif
}

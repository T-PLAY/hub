

#include <io/input/Input.hpp>

#include <io/input/InputStream.hpp>
#include <io/input/InputStreamInterface.hpp>
#include <io/input/InputStreamServer.hpp>
#include <io/input/InputSyncStream.hpp>
#include <io/input/InputFile.hpp>
#include <io/input/InputMemory.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


//    const hub::io::Input & input = hub::Input(); // abstract class
    const hub::io::Input & input2 = hub::input::InputStream("streamName", ipv4, port);
    const hub::io::Input & input3 = hub::input::InputStreamServer("streamName", ipv4, port);
//    const hub::io::Input & input4 = hub::input::InputStreamInterface("streamName", ipv4, port); // abstract class
    const hub::io::Input & input5 = hub::input::InputSyncStream<>("streamName", "streamName2", ipv4, port);
    const hub::io::Input & input6 = hub::input::InputSyncStream<hub::input::InputStreamServer>("streamName", "streamName2", ipv4, port);
    const hub::io::Input & input7 = hub::input::InputSyncStream<hub::input::InputStreamMqtt>("streamName", "streamName2", ipv4, port);
    const hub::io::Input & input8 = hub::input::InputFile("filePath");
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    const hub::io::Input & input9 = hub::input::InputMemory<decltype(vs)>(vs);
#else
    const hub::io::Input & input9 = hub::input::InputMemory(vs);
#endif
}

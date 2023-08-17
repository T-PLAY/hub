

#include <io/input/Input.hpp>

#include <io/input/InputStream.hpp>
#include <io/input/InputStreamInterface.hpp>
#include <io/input/InputStreamServer.hpp>
#include <io/input/InputSyncStream.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;


//    const hub::io::Input & input = hub::Input(); // abstract class
    const hub::io::Input & input2 = hub::input::InputStream("streamName", ipv4, port);
    const hub::io::Input & input3 = hub::input::InputStreamServer("streamName", ipv4, port);
//    const hub::io::Input & input4 = hub::input::InputStreamInterface("streamName", ipv4, port); // abstract class
    const hub::io::Input & input5 = hub::input::InputSyncStream("streamName", "streamName2", ipv4, port);

}

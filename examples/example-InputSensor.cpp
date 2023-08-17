

#include <InputSensor.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;

    hub::InputSensor inputSensor(hub::input::InputStream("streamName", ipv4, port));
    hub::InputSensor inputSensor2(hub::input::InputSyncStream("streamName", "streamName2", ipv4, port));

}

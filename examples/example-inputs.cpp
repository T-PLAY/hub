

#include <InputSensor.hpp>


int main() {



    int port = 4042;
    hub::Resolution res({1}, hub::Format::BGR8);
    hub::SensorSpec sensorSpec("sensorName", {res});

//    hub::io::InputStream inputStream("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(inputStream));
    hub::InputSensor inputSensor(hub::io::InputStream("streamName", "127.0.0.1", port));
//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(clientSocket));

//    hub::io::InputStreamInterface inputStreamInterface("streamName", "fuck", 5);
//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
//    auto & input = inputSensor.getInput();
//    assert(input.isEnd());



	return 0;
}

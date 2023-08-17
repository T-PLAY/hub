
#include <OutputSensor.hpp>
#include <InputSensor.hpp>

//#include <server/Server.hpp>

//#include <io/InputStreamInterface.hpp>

//#include <io/Memory.hpp>
//#include <io/MemoryInterface.hpp>
//#include <io/input/InputMemory.hpp>
//#include <io/output/OutputMemory.hpp>


int main() {


//    srand( (unsigned)time( NULL ) );
//    int port = rand() % 60000;
    const int port = 1883;


//    hub::Server server(port);
//    server.setMaxClients(2);
//    server.asyncRun();

    ////////////////////////////////////////////////////////////

    hub::Resolution res({1}, hub::Format::BGR8);
    hub::SensorSpec sensorSpec("sensorName", {res});
//    hub::SensorSpec sensorSpec("sensorName", {{{1}, hub::Format::BGR8}});
//    hub::net::ClientSocket clientSocket;
//    hub::OutputSensor outputSensor(sensorSpec, std::move(clientSocket));
//    hub::OutputSensor outputSensor(sensorSpec, "streamName", hub::net::ClientSocket("127.0.0.1", port));
//    hub::io::OutputStream outputStream("streamName", "127.0.0.1", port);
//    hub::OutputSensor outputSensor(sensorSpec, std::move(outputStream));
//    hub::OutputSensor outputSensor(sensorSpec, hub::io::OutputStream("streamName", "127.0.0.1", port));
//    hub::OutputSensor outputSensor(sensorSpec, "streamName", "127.0.0.1", port);
//    hub::OutputSensor outputSensor(sensorSpec, "streamName", "127.0.0.1", port);
//    hub::OutputSensor outputSensor({"sensorName", {res}}, "streamName", "127.0.0.1", port);
//    hub::OutputSensor outputSensor(sensorSpec, "streamName", "127.0.0.1", port);
//    hub::OutputSensor outputSensor({"sensorName", {{{1}, hub::Format::BGR8}}}, "streamName", "127.0.0.1", port);

    hub::OutputSensor outputSensor(sensorSpec, hub::output::OutputStreamMqtt("streamName", "127.0.0.1", port));

    hub::InputSensor inputSensor(hub::input::InputStreamMqtt("streamName", "127.0.0.1", port));

//    hub::io::InputStream inputStream("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(inputStream));
//    hub::InputSensor inputSensor(hub::input::InputStream("streamName", "127.0.0.1", port));
//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(clientSocket));

//    hub::io::InputStreamInterface inputStreamInterface("streamName", "fuck", 5);
//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
    auto & input = inputSensor.getInput();
    assert(input.isEnd());


    hub::Acquisition acq(1, 1);
    unsigned char data[3] = { 1, 2, 3};
    hub::data::Measure measure(data, 3, res);
    acq << std::move(measure);


//    assert(! input.isEnd());
//    hub::Acquisition acq2;
//    inputSensor >> acq2;

//    std::cout << acq2 << std::endl;

//    std::vector<char> buff;
//    hub::input::InputMemory<std::vector<char>> memory(buff);
//    hub::output::OutputMemory<std::vector<char>> memory2(buff);
//    hub::io::Memory<std::vector<char>> memory3( buff );

//    assert(memory.isEnd());
//    assert(memory3.isEnd());

//    int a = 5;
//    memory2.put(a);

//    assert(! memory.isEnd());
//    assert(! memory3.isEnd());

//    int b;
//    memory.read(b);
//    assert(b == a);


	return 0;
}

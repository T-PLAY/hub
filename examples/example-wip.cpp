
#include <OutputSensor.hpp>
#include <InputSensor.hpp>

//#include <server/Server.hpp>

//#include <io/InputStreamInterface.hpp>

//#include <io/Memory.hpp>
//#include <io/MemoryInterface.hpp>
//#include <io/input/InputMemory.hpp>
//#include <io/output/OutputMemory.hpp>

class user_callback : public virtual mqtt::callback
{
    void connected(const std::string & cause) override {
        std::cout << "connected" << std::endl;
    }
    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
        std::cout << "message_arrived" << std::endl;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        std::cout << "\n\t[Delivery complete for token: "
                  << (tok ? tok->get_message_id() : -1) << "]" << std::endl;
    }


  public:
};


int main() {

//    mqtt::client client("tcp://localhost:1883", "viewer", mqtt::create_options(MQTTVERSION_5));
    mqtt::async_client client("tcp://localhost:1883", "viewer", mqtt::create_options(MQTTVERSION_5));
//    mqtt::async_client client()

    user_callback cb;
    client.set_callback(cb);

    client.connect();

    std::cout << "press key to quit" << std::endl;
    getchar();

    client.disconnect();

    return 0;


    srand( (unsigned)time( NULL ) );
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

//    assert(outputSensor.getSpec() == inputSensor.getSpec());

//    hub::io::InputStream inputStream("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(inputStream));
//    hub::InputSensor inputSensor(hub::input::InputStream("streamName", "127.0.0.1", port));
//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
//    hub::InputSensor inputSensor(std::move(clientSocket));

//    hub::InputSensor inputSensor("streamName", "127.0.0.1", port);
    auto & input = inputSensor.getInput();
//    assert(input.isEnd());


    hub::Acquisition acq(1, 1);
    unsigned char data[3];
    for (int i= 0; i <3; ++i) {
        data[i] = rand();
    }
    hub::data::Measure measure(data, 3, res);
    acq << std::move(measure);

    outputSensor << acq;



    assert(! input.isEnd());
    hub::Acquisition acq2;
    inputSensor >> acq2;

    std::cout << acq2 << std::endl;

    assert(acq == acq2);

//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

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

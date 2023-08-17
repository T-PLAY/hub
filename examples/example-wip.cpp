
#include <OutputSensor.hpp>
#include <server/Server.hpp>


int main() {


    srand( (unsigned)time( NULL ) );
    int port = rand() % 60000;

    hub::server::Server server(port);
    server.setMaxClients(2);
    server.asyncRun();

    ////////////////////////////////////////////////////////////

    hub::Resolution res({1}, hub::Format::BGR8);
    hub::SensorSpec sensorSpec("sensorName", {res});
//    hub::SensorSpec sensorSpec("sensorName", {{{1}, hub::Format::BGR8}});
//    hub::net::ClientSocket clientSocket;
//    hub::OutputSensor outputSensor(sensorSpec, std::move(clientSocket));
    hub::OutputSensor outputSensor(sensorSpec, "streamName", hub::net::ClientSocket("127.0.0.1", port));

    hub::InputSensor inputSensor(hub::io::InputStream("streamName", hub::net::ClientSocket("127.0.0.1", port)));
//    auto & input = inputSensor.getInput();
//    assert(input.isEnd());


    hub::Acquisition acq(1, 1);
    unsigned char data[3] = { 1, 2, 3};
    hub::data::Measure measure(data, 3, res);
    acq << std::move(measure);
    outputSensor << acq;

//    assert(! input.isEnd());
    hub::Acquisition acq2;
    inputSensor >> acq2;

    std::cout << acq2 << std::endl;

	return 0;
}

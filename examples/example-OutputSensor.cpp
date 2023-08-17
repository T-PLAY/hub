



#include <OutputSensor.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;

    hub::SensorSpec sensorSpec;


//    const hub::io::Output & output = hub::Output(); // abstract class
//    const auto & outputSensor2 = hub::OutputSensor(sensorSpec, hub::output::OutputStream("streamName", ipv4, port));
    hub::OutputSensor outputSensor2(sensorSpec, "streamName", ipv4, port);
    hub::OutputSensor outputSensor3(sensorSpec, "streamName", ipv4, port);
//    const hub::io::Output & output4 = hub::output::OutputStreamInterface("streamName", ipv4, port); // abstract class
    hub::OutputSensor outputSensor5(sensorSpec, hub::output::OutputFile("filePath"));
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    hub::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory<decltype(vs)>(vs));
#else
    hub::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory(vs));
#endif


}

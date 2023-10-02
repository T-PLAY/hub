



#include <sensor/OutputSensor.hpp>

int main() {
    return 0;

    const std::string ipv4 = "127.0.0.1";
    const int port = 4042;

    hub::sensor::SensorSpec sensorSpec;


//    const hub::Output & output = hub::Output(); // abstract class
//    const auto & outputSensor2 = hub::sensor::OutputSensor(sensorSpec, hub::output::OutputStream("streamName", ipv4, port));
    hub::sensor::OutputSensor outputSensor2(sensorSpec, hub::output::OutputStream("streamName", ipv4, port));
    hub::sensor::OutputSensor outputSensor3(sensorSpec, hub::output::OutputStream("streamName", ipv4, port));
//    const hub::Output & output4 = hub::output::OutputStreamInterface("streamName", ipv4, port); // abstract class
    hub::sensor::OutputSensor outputSensor5(sensorSpec, hub::output::OutputFile("filePath"));
    std::vector<int> vs;
#if CPLUSPLUS_VERSION < 17
    hub::sensor::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory<decltype(vs)>(vs));
#else
    hub::sensor::OutputSensor outputSensor6(sensorSpec, hub::output::OutputMemory(vs));
#endif


}

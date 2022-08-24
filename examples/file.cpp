
#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <IO/Ram.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Net/ClientSocket.hpp>

int main() {
    return 0;
    // not execute this, this is an example of how to use hub library

    {
//        hub::net::ClientSocket clientSocket = hub::net::ClientSocket();
//        hub::Sensor sensor = hub::Sensor(hub::SensorSpec(), clientSocket);

        hub::SensorSpec::MetaData metaData;
        {
            hub::OutputSensor outputSensor2(
                hub::SensorSpec { "sensorName",
                                  hub::SensorSpec::Format::BGR8,
                                  { 1 },
                                  hub::SensorSpec::MetaData() },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::OutputSensor outputSensor3(
                { "sensorName", hub::SensorSpec::Format::BGR8, { 1 } },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::SensorSpec sensorSpec(
                "sensorName", hub::SensorSpec::Format::BGR8, { 1 }, metaData );
            hub::OutputSensor outputSensor(
                sensorSpec, hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );

            std::fstream file( "file.txt", std::ios::in );
            hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );
        }

        {
            hub::SensorSpec sensorSpec(
                "sensorName", hub::SensorSpec::Format::BGR8, { 1 }, metaData );
            hub::io::CyclicBuff cyclicBuff;
            hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( cyclicBuff ) );

            hub::InputSensor inputSensor( ( hub::io::Ram( cyclicBuff ) ) );
        }

        {
            hub::SensorSpec sensorSpec(
                "sensorName", hub::SensorSpec::Format::BGR8, { 1 }, metaData );
            hub::OutputSensor outputSensor(
                sensorSpec, hub::io::OutputStream( "streamName", hub::net::ClientSocket() ) );
            //            hub::OutputSensor outputSensor2(
            //                sensorSpec, hub::io::StreamViewer( "streamName", "",
            //                hub::net::ClientSocket() ) );
            hub::InputSensor inputSensor(
                hub::io::InputStream( "streamName", "", hub::net::ClientSocket() ) );
        }
    }

    {
        //        unsigned char data[3] = { 0, 1, 2 };
        //        outputSensor << hub::Acquisition { 0, 0, data, 3 };
        hub::InputSensor inputSensor( hub::io::File( std::fstream( "file.txt", std::ios::in ) ) );
        const auto& acq = inputSensor.getAcquisition();

        std::cout << "acq = " << acq << std::endl;

        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { 1 } },
            hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
        outputSensor << acq;
    }



    //    hub::OutputSensor outputSensor2(hub::SensorSpec("hello2"), std::fstream(filename,
    //    std::ios::out));

    return 0;
}

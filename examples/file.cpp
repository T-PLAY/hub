
#include <IO/File.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Net/ClientSocket.hpp>
#include <IO/NetClient.hpp>

int main() {

    //    const std::string filename = "file.txt";

    //    std::fstream fileStream( filename, std::ios::out );
    //    assert( fileStream.is_open() );

    //    hub::io::File file( std::move( fileStream ) );

    //    const hub::SensorSpec sensorSpec( "hello", hub::SensorSpec::Format::BGR8, { 1 } );
    //    hub::OutputSensor outputSensor( std::move( sensorSpec ), std::move( file ) );

    hub::io::StreamViewer streamViewer(hub::net::ClientSocket);

    {
        hub::OutputSensor outputSensor(
            { "sensorName", hub::SensorSpec::Format::BGR8, { 1 } },
            hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );

        unsigned char data[3] = { 0, 1, 2 };
        outputSensor << hub::Acquisition { 0, 0, data, 3 };
    }

    {
        hub::InputSensor inputSensor( hub::io::File( std::fstream( "file.txt", std::ios::in ) ) );
        const auto & acq = inputSensor.getAcquisition();
        std::cout << "acq = " << acq << std::endl;
    }

    //    hub::OutputSensor outputSensor2(hub::SensorSpec("hello2"), std::fstream(filename,
    //    std::ios::out));

    return 0;
}

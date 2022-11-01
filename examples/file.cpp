
#include <IO/File.hpp>
#include <IO/Ram.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <Net/ClientSocket.hpp>

int main() {
    return 0;
    // not execute this, this is an example of how to use hub library

    {

        hub::SensorSpec::MetaData metaData;
        hub::SensorSpec::Format format = hub::SensorSpec::Format::BGR8;
        hub::SensorSpec::Dims dims     = { 1 };
        hub::SensorSpec::Resolutions resolutions { { dims, format } };
        hub::SensorSpec sensorSpec {
            "sensorName", { { dims, hub::SensorSpec::Format::BGR8 } }, metaData };
        hub::SensorSpec sensorSpec2 {
            "sensorName",
            { { { 1 }, hub::SensorSpec::Format::BGR8 }, { { 1, 2, 3 }, format } },
            metaData };
        hub::SensorSpec sensorSpec3 { "sensorName", {}, metaData };
        {
            hub::OutputSensor outputSensor2(
                hub::SensorSpec { "sensorName",
                                  { { { 1 }, hub::SensorSpec::Format::BGR8 } },
                                  hub::SensorSpec::MetaData() },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::OutputSensor outputSensor3(
                { "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } } },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::SensorSpec sensorSpec(
                "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } }, metaData );
            hub::OutputSensor outputSensor(
                sensorSpec, hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );

            const unsigned char bgrArray[3] = { 0, 1, 2 };
            const unsigned char dof6Array[28] { 0 };
            hub::Dof6 dof6;
            outputSensor2 << ( hub::Acquisition { 0, 0 }
                               << hub::Measure( bgrArray, 3 ) << hub::Measure { dof6Array, 28 }
                               << std::move( hub::Dof6() ) << std::move( dof6 ) );


            std::fstream file( "file.txt", std::ios::in );
            hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );
            const hub::SensorSpec::Resolutions& resolutions = inputSensor.m_spec.m_resolutions;

            auto acq = inputSensor.getAcquisition();

            const hub::Measure& measure = acq.getMeasures().at( 0 );
            const hub::Dof6& dof62      = acq.getMeasures().at( 0 );
        }

        {
            const hub::SensorSpec sensorSpec(
                "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } }, metaData );
            hub::io::CyclicBuff cyclicBuff;
            hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( cyclicBuff ) );

            hub::InputSensor inputSensor( ( hub::io::Ram( cyclicBuff ) ) );
        }

        {
            hub::SensorSpec sensorSpec(
                "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } }, metaData );
            sensorSpec.m_sensorName = "hello";
            hub::OutputSensor outputSensor(
                sensorSpec, hub::io::OutputStream( "streamName", hub::net::ClientSocket() ) );
            hub::InputSensor inputSensor(
                hub::io::InputStream( "streamName", "", hub::net::ClientSocket() ) );
        }
    }

    {
        hub::InputSensor inputSensor( hub::io::File( std::fstream( "file.txt", std::ios::in ) ) );
        const auto& acq = inputSensor.getAcquisition();

        std::cout << "acq = " << acq << std::endl;

        hub::OutputSensor outputSensor(
            { "sensorName", { { { 1 }, hub::SensorSpec::Format::BGR8 } } },
            hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
        outputSensor << acq;
    }


    return 0;
}

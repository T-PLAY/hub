
#include <InputSensor.hpp>
#include <OutputSensor.hpp>

#include <filesystem>
#include <thread>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    {
        // clang-format off
        // startConstruction
hub::io::File outputFile(
    std::fstream( "file.txt", std::ios::binary | std::ios::out | std::ios::trunc ) );
hub::io::File inputFile( std::fstream( "file.txt", std::ios::binary | std::ios::in ) );
        // endConstruction
        // clang-format on
    }

    // do not execute this code, this is an example of how to use hub library
    {

        hub::SensorSpec::MetaData metaData;
        hub::Format format = hub::Format::BGR8;
        hub::Dims dims     = { 1 };
        hub::Resolutions resolutions { { dims, format } };
        hub::SensorSpec sensorSpec { "sensorName", { { dims, hub::Format::BGR8 } }, metaData };
        hub::SensorSpec sensorSpec2 {
            "sensorName", { { { 1 }, hub::Format::BGR8 }, { { 1, 2, 3 }, format } }, metaData };
        hub::SensorSpec sensorSpec3 { "sensorName", {}, metaData };
        {
            hub::sensor::OutputSensor outputSensor2(
                hub::SensorSpec {
                    "sensorName", { { { 1 }, hub::Format::BGR8 } }, hub::SensorSpec::MetaData() },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::sensor::OutputSensor outputSensor3(
                hub::SensorSpec { "sensorName", { { { 1 }, hub::Format::BGR8 } } },
                hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
            hub::SensorSpec sensorSpec( "sensorName", { { { 1 }, hub::Format::BGR8 } }, metaData );
            hub::sensor::OutputSensor outputSensor(
                sensorSpec, hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );

            const unsigned char bgrArray[3] = { 0, 1, 2 };
            const unsigned char dof6Array[28] { 0 };
            hub::data::Dof6 dof6;
            outputSensor2 << ( hub::sensor::Acquisition { 0, 0 }
                               << hub::Measure { bgrArray, 3, { { 1 }, hub::Format::BGR8 } }
                               << hub::Measure { dof6Array, 28, { { 1 }, hub::Format::DOF6 } }
                               << std::move( hub::data::Dof6() ) << std::move( dof6 ) );

            std::fstream file( "file.txt", std::ios::in );
            hub::sensor::InputSensor inputSensor( hub::io::File( std::move( file ) ) );
            const hub::Resolutions& resolutions = inputSensor.getSpec().getResolutions();

            hub::sensor::Acquisition acq;
            inputSensor >> acq;

            const hub::Measure& measure = acq.getMeasures().at( 0 );
            const hub::data::Dof6 dof62( measure );
        }

        {
            const hub::SensorSpec sensorSpec(
                "sensorName", { { { 1 }, hub::Format::BGR8 } }, metaData );
        }

        {
            hub::SensorSpec sensorSpec( "sensorName", { { { 1 }, hub::Format::BGR8 } }, metaData );
            hub::sensor::OutputSensor outputSensor( sensorSpec, "streamName", hub::net::ClientSocket() );
            hub::sensor::InputSensor inputSensor(
                hub::io::InputStream( "streamName", hub::net::ClientSocket() ) );
        }
    }

    {
        hub::sensor::InputSensor inputSensor( hub::io::File( std::fstream( "file.txt", std::ios::in ) ) );
        hub::sensor::Acquisition acq;
        inputSensor >> acq;

        std::cout << "acq = " << acq << std::endl;

        hub::sensor::OutputSensor outputSensor(
            hub::SensorSpec { "sensorName", { { { 1 }, hub::Format::BGR8 } } },
            hub::io::File( std::fstream( "file.txt", std::ios::out ) ) );
        outputSensor << acq;
    }

    return 0;
}


#include <sensor/InputSensor.hpp>
#include <sensor/OutputSensor.hpp>

#include <filesystem>
#include <thread>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    /// startConstruction
    /// file
    /// endConstruction

    // {
    //     hub::input::InputFile outputFile(
    //         std::fstream( "file.txt", std::ios::binary | std::ios::out | std::ios::trunc ) );
    //     hub::input::InputFile inputFile(
    //         std::fstream( "file.txt", std::ios::binary | std::ios::in ) );
    // }

    // {

    //     hub::MetaData metaData;
    //     hub::format format = hub::format::BGR8;
    //     hub::NDim nDim     = { 1 };
    //     hub::Resolutions resolutions { { nDim, format } };
    //     hub::sensor::SensorSpec sensorSpec {
    //         "sensorName", { { nDim, hub::format::BGR8 } }, metaData };
    //     hub::sensor::SensorSpec sensorSpec2 {
    //         "sensorName", { { { 1 }, hub::format::BGR8 }, { { 1, 2, 3 }, format } }, metaData };
    //     hub::sensor::SensorSpec sensorSpec3 { "sensorName", {}, metaData };
    //     {
    //         hub::sensor::OutputSensor outputSensor2(
    //             hub::sensor::SensorSpec {
    //                 "sensorName", { { { 1 }, hub::format::BGR8 } }, hub::MetaData() },
    //             hub::input::InputFile( std::fstream( "file.txt", std::ios::out ) ) );
    //         hub::sensor::OutputSensor outputSensor3(
    //             hub::sensor::SensorSpec { "sensorName", { { { 1 }, hub::format::BGR8 } } },
    //             hub::input::InputFile( std::fstream( "file.txt", std::ios::out ) ) );
    //         hub::sensor::SensorSpec sensorSpec(
    //             "sensorName", { { { 1 }, hub::format::BGR8 } }, metaData );
    //         hub::sensor::OutputSensor outputSensor(
    //             sensorSpec, hub::input::InputFile( std::fstream( "file.txt", std::ios::out ) ) );

    //         const unsigned char bgrArray[3] = { 0, 1, 2 };
    //         const unsigned char dof6Array[28] { 0 };
    //         hub::data::Dof6 dof6;
    //         outputSensor2 << ( hub::sensor::Acquisition { 0, 0 }
    //                            << hub::Measure { bgrArray, 3, { { 1 }, hub::format::BGR8 } }
    //                            << hub::Measure { dof6Array, 28, { { 1 }, hub::format::Dof6 } }
    //                            << std::move( hub::data::Dof6() ) << std::move( dof6 ) );

    //         std::fstream file( "file.txt", std::ios::in );
    //         hub::sensor::InputSensor inputSensor( hub::input::InputFile( std::move( file ) ) );
    //         const hub::Resolutions& resolutions = inputSensor.getSpec().getResolutions();

    //         hub::sensor::Acquisition acq;
    //         inputSensor >> acq;

    //         const hub::Measure& measure = acq.getMeasures().at( 0 );
    //         const hub::data::Dof6 dof62( measure );
    //     }

    //     {
    //         const hub::sensor::SensorSpec sensorSpec(
    //             "sensorName", { { { 1 }, hub::format::BGR8 } }, metaData );
    //     }

    //     {
    //         hub::sensor::SensorSpec sensorSpec(
    //             "sensorName", { { { 1 }, hub::format::BGR8 } }, metaData );
    //         hub::sensor::OutputSensor outputSensor(
    //             sensorSpec, "streamName", hub::net::ClientSocket() );
    //         hub::sensor::InputSensor inputSensor(
    //             hub::input::InputStream( "streamName", hub::net::ClientSocket() ) );
    //     }
    // }

    // {
    //     hub::sensor::InputSensor inputSensor(
    //         hub::input::InputFile( std::fstream( "file.txt", std::ios::in ) ) );
    //     hub::sensor::Acquisition acq;
    //     inputSensor >> acq;

    //     std::cout << "acq = " << acq << std::endl;

    //     hub::sensor::OutputSensor outputSensor(
    //         hub::sensor::SensorSpec { "sensorName", { { { 1 }, hub::format::BGR8 } } },
    //         hub::input::InputFile( std::fstream( "file.txt", std::ios::out ) ) );
    //     outputSensor << acq;
    // }

    return 0;
}

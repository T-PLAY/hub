

#include <sensor/OutputSensor.hpp>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    /// startConstruction
    /// outputSensor
    /// endConstruction

    /// startConstruction2
    /// outputSensor2
    /// endConstruction2

    /// startConstruction3
    /// outputSensor3
    /// endConstruction3

    /// startFunctional
    /// outputSensor
    /// endFunctional

    constexpr auto width  = 512;
    constexpr auto depth  = 512;
    constexpr auto height = 100;

    // hub::format format = hub::format::DENSITY;
    // hub::NDim nDim     = { width, depth, height };
    // hub::Resolution resolution( { nDim, format } );
    // hub::Resolutions resolutions( { resolution } );
    using Format = hub::format::Density;
    using Resolution = hub::MatrixXD<Format, width, depth, height>;
    using OutputSensor = hub::sensor::OutputSensorT<Resolution>;
    // using Acq = OutputSensor::Acquisition;
    hub::sensor::SensorSpec sensorSpec2( "sensorName", Resolution() );

    OutputSensor outputSensor(sensorSpec2, "streamName");
    // hub::client::Streamer streamer( "127.0.0.1", 4042 );
    // streamer.addStream( "streamName", sensorSpec2 );

    auto acq = outputSensor.acqMsg();
    // Acq acq;
    auto [start, end] = acq.clocks();
    auto * densities = acq.get<Format*>();
    // acq.ca

    for (auto i = 0; i < width; ++i) {
        for (auto j = 0; j < depth; ++j) {
            for (auto k = 0; k < height; ++k) {


            }
        }
    }

    // streamer.newAcquisition( "streamName", acq );

    // std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );

    return 0;

    // {
    //     hub::sensor::SensorSpec sensorSpec;
    //     {
    //         hub::sensor::OutputSensor outputSensor(
    //             sensorSpec, "myStream", hub::net::ClientSocket( "127.0.0.1", 4042 ) );

    //         long long start = 0, end = 0;
    //         unsigned char* data = nullptr;
    //         uint64_t size       = 0;
    //         hub::Resolution resolution;
    //         hub::Measure measure( data, size, resolution );
    //         hub::Measure measure2( data, size, resolution );
    //         hub::Measure measure3( data, size, resolution );

    //         outputSensor << ( hub::sensor::Acquisition { start, end }
    //                           << hub::Measure { data, size, resolution } );
    //     }

    //     {
    //         hub::sensor::OutputSensor outputSensor(
    //             sensorSpec,
    //             hub::input::InputFile( std::fstream(
    //                 "file.txt", std::ios::binary | std::ios::out | std::ios::trunc ) ) );
    //     }

    //     {}
    // }

    return 0;
}

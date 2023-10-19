

#include <OutputSensor.hpp>
#include <client/Streamer.hpp>

#include <filesystem>
#include <thread>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    constexpr int width  = 512;
    constexpr int depth  = 512;
    constexpr int height = 100;

    hub::Format format = hub::Format::DENSITY;
    hub::Dims dims     = { width, depth, height };
    hub::Resolution resolution( { dims, format } );
    hub::Resolutions resolutions( { resolution } );
    hub::SensorSpec sensorSpec2( "sensorName", resolutions );
    std::cout << sensorSpec2.to_string() << std::endl;

    hub::client::Streamer streamer( "127.0.0.1", 4042 );
    streamer.addStream( "streamName", sensorSpec2 );

    hub::sensor::Acquisition acq( 1, 1 );
    const int dataSize = width * depth * height;
    float* data        = new float[dataSize];
    for ( int i = 0; i < dataSize; ++i ) {
        data[i] = 1.0;
    }
    hub::Measure measure( (unsigned char*)data, dataSize * 4, resolution );
    delete[] data;
    acq << std::move( measure );

    streamer.newAcquisition( "streamName", acq );

    std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    return 0;

    {
        hub::SensorSpec sensorSpec;
        // clang-format off
        {
            // startConstruction
// bin/server is running locally on port 4042
// the stream id name 'myStream' is not used within the server
hub::sensor::OutputSensor outputSensor(
    sensorSpec,
    "myStream", hub::net::ClientSocket( "127.0.0.1", 4042 ) );
            // endConstruction

            long long start = 0, end = 0;
            unsigned char * data = nullptr;
            uint64_t size = 0;
            hub::Resolution resolution;
            hub::Measure measure(data, size, resolution);
            hub::Measure measure2(data, size, resolution);
            hub::Measure measure3(data, size, resolution);

            // clang-format on
            // startFunctional
            outputSensor << ( hub::sensor::Acquisition { start, end }
                              << hub::Measure { data, size, resolution } );
            // endFunctional
            // assert( outputSensor.m_spec.getResolutions().at(0) == resolution );
            // clang-format off
        }

        {
            // startConstruction2
// save acquisitions to file
hub::sensor::OutputSensor outputSensor(
    sensorSpec,
    hub::io::File( std::fstream(
        "file.txt", std::ios::binary | std::ios::out | std::ios::trunc ) ) );
            // endConstruction2
        }

        {
            // startConstruction3
// create pear buffer to shared data between different threads
            // endConstruction3
        }
        // clang-format on
    }

    return 0;
}

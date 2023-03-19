

#include <OutputSensor.hpp>

#include <filesystem>
#include <thread>
// #include <cassert>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    {
        hub::SensorSpec sensorSpec;
        // clang-format off
        {
            // startConstruction
// bin/server is running locally on port 4042
// the stream id name 'myStream' is not used within the server
hub::OutputSensor outputSensor(
    sensorSpec,
    hub::io::OutputStream( "myStream", hub::net::ClientSocket( "127.0.0.1", 4042 ) ) );
            // endConstruction

            long long start = 0, end = 0;
            unsigned char * data = nullptr;
            uint64_t size = 0;
            hub::Resolution resolution;
            hub::data::Measure measure(data, size, resolution);
            hub::data::Measure measure2(data, size, resolution);
            hub::data::Measure measure3(data, size, resolution);

            // clang-format on
            // startFunctional
            outputSensor << ( hub::Acquisition { start, end }
                              << hub::data::Measure { data, size, resolution } );
            // endFunctional
            // assert( outputSensor.m_spec.getResolutions().at(0) == resolution );
            // clang-format off
        }

        {
            // startConstruction2
// save acquisitions to file
hub::OutputSensor outputSensor(
    sensorSpec,
    hub::io::File( std::fstream(
        "file.txt", std::ios::binary | std::ios::out | std::ios::trunc ) ) );
            // endConstruction2
        }

        {
            // startConstruction3
// create pear buffer to shared data between different threads
//hub::io::CyclicBuff buff;
//hub::OutputSensor outputSensor( sensorSpec, hub::io::Ram( buff ) );
            // endConstruction3
        }
        // clang-format on
    }

    return 0;
}


#include <InputSensor.hpp>

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
// the stream id name 'myStream' is streaming to the server
hub::InputSensor inputSensor(
    hub::io::InputStream( "myStream", "", hub::net::ClientSocket( "127.0.0.1", 4042 ) ) );
            // endConstruction

            // startFunctional
auto acq = inputSensor.getAcquisition();
auto acqs = inputSensor.getAllAcquisitions();
            // endFunctional
        }

        {
            // startConstruction2
hub::InputSensor inputSensor(
    hub::io::File( std::fstream("file.txt", std::ios::binary | std::ios::in ) ) );
            // endConstruction2
        }

        {
            // startConstruction3
hub::io::CyclicBuff buff;
auto inputSensor = hub::InputSensor( hub::io::Ram( buff ) );
            // endConstruction3
        }
        // clang-format on
    }

    return 0;
}

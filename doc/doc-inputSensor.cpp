
#include <sensor/InputSensor.hpp>

#include <filesystem>
#include <thread>

#include <sensor/InputSensor.hpp>
#include <server/Server.hpp>

/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    // const int ref_offset    = 5;
    // constexpr int ref_nAcqs = 10;

    // const int ref2_offset    = 0;
    // constexpr int ref_nAcqs2 = 10;

    // const hub::Resolution ref_resolution( { { 1 }, hub::format::Y8 } );
    // const hub::sensor::SensorSpec ref_sensorSpec( "sensorName", { ref_resolution } );
    // const int ref_dataSize = hub::sensor::resolution::computeAcquisitionSize( ref_resolution );

    // const hub::Resolution ref_resolution2( { { 1 }, hub::format::Dof6 } );
    // const hub::sensor::SensorSpec ref_sensorSpec2( "sensorName2", { ref_resolution2 } );
    // const int ref_dataSize2 = hub::sensor::resolution::computeAcquisitionSize( ref_resolution2 );

    // const std::string ipv4 = "127.0.0.1";
    // const int port         = GET_RANDOM_PORT;

    //// bin/server is running locally on port 4042
    //// the stream id name 'myStream' is streaming to the server

    //// read saved acquisitions from file

    //// get acqusitions from different threads

    return 0;
}

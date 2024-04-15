

#include <sensor/OutputSensor.hpp>

namespace sensorAPI {
auto getTimestamp() {
    struct s_ret {
        long long start;
        long long end;
    };
    return s_ret { 0, 0 };
}

void updateTimestamp(long long &start, long long & end) {

}

void fetchData(unsigned char* data) {

}

auto getData() {
    struct s_ret {
        unsigned char* data;
        uint64_t size;
    };
    return s_ret { 0, 0 };
}
} // namespace sensorAPI


/// \file
/// \brief  Main function
/// describes how use a file to init input/output sensor stream
///
int main() {
    /// Comments I would like to be documented in as well

    {
/// startConstruction
using ResolutionCam   = hub::MatrixXD<hub::format::RGB8, 640, 480>;
hub::sensor::SensorSpec sensorSpec2( "rgbCam", ResolutionCam() );

hub::sensor::OutputSensor outputSensor( sensorSpec2, "streamName" );

auto acq          = outputSensor.acqMsg();
auto [start, end] = acq.clocks();
auto* const imgData   = acq.get<hub::format::RGB8*>();

while (1) {
    sensorAPI::updateTimestamp(start, end); // get latest clocks from sensor api
    sensorAPI::fetchData((unsigned char*)imgData); // get latest image data from sensor api

    outputSensor << acq;
}
/// endConstruction
    }

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

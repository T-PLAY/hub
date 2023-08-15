
#include <client/Streamer.hpp>

/// \file

namespace sensorAPI {
auto getTimestamp() {
    struct s_ret {
        long long start;
        long long end;
    };
    return s_ret { 0, 0 };
}

auto getData() {
    struct s_ret {
        unsigned char* data;
        uint64_t size;
    };
    return s_ret { 0, 0 };
}
} // namespace sensorAPI

// clang-format off
int main() {

// startConstruction
// link to local server at port 4042
hub::client::Streamer streamer( "127.0.0.1", 4042 );
// endConstruction

hub::SensorSpec sensorSpec;
hub::Resolution resolution;
bool receivingDataFromSensor = true;

// startFunctional
// init stream
streamer.addStream( "myStream", std::move( sensorSpec ) );

while ( receivingDataFromSensor ) {

#if ( __cplusplus >= 201703L )
    auto [start, end] = sensorAPI::getTimestamp();
    auto [data, size] = sensorAPI::getData();
#else
    auto start = sensorAPI::getTimestamp().start;
    auto end = sensorAPI::getTimestamp().end;
    auto data = sensorAPI::getData().data;
    auto size = sensorAPI::getData().size;
#endif

    auto&& acq {hub::Acquisition { start, end } << hub::data::Measure { data, size, resolution }};
    // share new sensor data (only if connected to the server)
    streamer.newAcquisition( "myStream", std::move( acq ) );
}
// endFunctional

return 0;
}
// clang-format on

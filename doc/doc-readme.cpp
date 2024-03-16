

#include <sensor/OutputSensor.hpp>

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

namespace clientApp {
enum class Format { BGR888, NONE };
void drawImage( const unsigned char* data, uint64_t size, int width, int height, Format format ) {

};

} // namespace clientApp

#include <sensor/InputSensor.hpp>

int main() {

    constexpr auto serverPort = 4042;
    {
        constexpr auto width  = 640;
        constexpr auto height = 480;

        using Format     = hub::format::BGR8;
        using Resolution = hub::MatrixXD<Format, width, height>;
        // const hub::Resolution imageResolution { { width, height }, hub::format::BGR8 };
        hub::MetaData metaData;
        metaData["fov"]  = 60.0;
        metaData["iso"]  = 200;
        metaData["date"] = "now";
        const hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor { sensorSpec, "streamName" };
        auto acq          = outputSensor.acqMsg();
        auto [start, end] = acq.clocks();
        auto* const bgr8s = acq.get<Format*>();

        while ( 1 ) {
            auto [start, end] = sensorAPI::getTimestamp();
            auto [data, size] = sensorAPI::getData();

            outputSensor << acq;
        }
    }

    return 0;
}

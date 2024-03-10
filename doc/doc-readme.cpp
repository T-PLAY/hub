

// #include <io/OutputStream.hpp>
// #include <io/input/InputStream.hpp>
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

        using Format = hub::format::BGR8;
        using Resolution = hub::MatrixXD<Format, width, height>;
        // const hub::Resolution imageResolution { { width, height }, hub::format::BGR8 };
        hub::MetaData metaData;
        metaData["fov"]  = 60.0;
        metaData["iso"]  = 200;
        metaData["date"] = "now";
        const hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

        hub::sensor::OutputSensorT<Resolution> outputSensor {
                                                              sensorSpec, "streamName"};
        auto acq = outputSensor.acqMsg();
        auto [start, end] = acq.clocks();
        auto * const bgr8s = acq.get<Format*>();

        while ( 1 ) {
            auto [start, end] = sensorAPI::getTimestamp();
            auto [data, size] = sensorAPI::getData();

            outputSensor << acq;
        }
    }

//     {
//         hub::sensor::InputSensor inputSensor { hub::input::InputStream {
//             "streamName", hub::net::ClientSocket { "serverIp", serverPort } } };

//         const auto& resolutions = inputSensor.getSpec().getResolutions();
//         if ( resolutions.size() == 1 ) {
// #if ( __cplusplus >= 201703L )
//             const auto& [nDim, format] = resolutions.at( 0 );
// #else
//             const auto& nDim   = resolutions.at( 0 ).first;
//             const auto& format = resolutions.at( 0 ).second;
// #endif

//             if ( nDim.size() == 2 && format == hub::format::BGR8 ) {
//                 const auto& width  = nDim.at( 0 );
//                 const auto& height = nDim.at( 1 );

//                 while ( 1 ) {
//                     hub::sensor::Acquisition acq;
//                     inputSensor >> acq;
//                     const auto& measure = acq.getMeasures().at( 0 );

//                     clientApp::drawImage( measure.getData(),
//                                           measure.getSize(),
//                                           width,
//                                           height,
//                                           clientApp::Format::BGR888 );
//                 }
//             }
//         }
//     }

    return 0;
}

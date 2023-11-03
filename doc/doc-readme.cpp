
// code in README.md of this project
// streamer code

#include <OutputSensor.hpp>
#include <io/InputStream.hpp>
#include <io/OutputStream.hpp>

/// \file

namespace sensorAPI {
// class sensorAPI {
// public:
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

#include <InputSensor.hpp>

int main() {

    constexpr int serverPort = 4042;
    {
        constexpr int imageWidth  = 640;
        constexpr int imageHeight = 480;

        // init output sensor
        const hub::Resolution imageResolution { { imageWidth, imageHeight }, hub::Format::BGR8 };
        hub::SensorSpec::MetaData metaData;
        metaData["fov"]  = 60.0;
        metaData["iso"]  = 200;
        metaData["date"] = "now";
        const hub::SensorSpec sensorSpec( "sensorName", { imageResolution }, metaData );

        hub::sensor::OutputSensor outputSensor {
            sensorSpec, "streamName", hub::net::ClientSocket { "serverIp", serverPort } };

        while ( 1 ) {
#if ( __cplusplus >= 201703L )
            auto [start, end] = sensorAPI::getTimestamp();
            auto [data, size] = sensorAPI::getData();
#else
            auto start         = sensorAPI::getTimestamp().start;
            auto end           = sensorAPI::getTimestamp().end;
            auto data          = sensorAPI::getData().data;
            auto size          = sensorAPI::getData().size;
#endif

            // send data
            outputSensor << ( hub::sensor::Acquisition { start, end }
                              << hub::Measure { data, size, imageResolution } );
        }
    }

    {
        // init input sensor
        hub::sensor::InputSensor inputSensor { hub::io::InputStream {
            "streamName", hub::net::ClientSocket { "serverIp", serverPort } } };

        const auto& resolutions = inputSensor.getSpec().getResolutions();
        if ( resolutions.size() == 1 ) {
#if ( __cplusplus >= 201703L )
            const auto& [nDim, format] = resolutions.at( 0 );
#else
            const auto& nDim   = resolutions.at( 0 ).first;
            const auto& format = resolutions.at( 0 ).second;
#endif

            // if compatible resolution for the client application
            if ( nDim.size() == 2 && format == hub::Format::BGR8 ) {
                const auto& imageWidth  = nDim.at( 0 );
                const auto& imageHeight = nDim.at( 1 );

                while ( 1 ) {
                    // receive data
                    hub::sensor::Acquisition acq;
                    inputSensor >> acq;
                    const auto& measure = acq.getMeasures().at( 0 );

                    // draw image
                    clientApp::drawImage( measure.getData(),
                                          measure.getSize(),
                                          imageWidth,
                                          imageHeight,
                                          clientApp::Format::BGR888 );
                }
            }
        }
    }

    return 0;
}



#include <io/OutputStream.hpp>
#include <io/input/InputStream.hpp>
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

    constexpr int serverPort = 4042;
    {
        constexpr int imageWidth  = 640;
        constexpr int imageHeight = 480;

        const hub::Resolution imageResolution { { imageWidth, imageHeight }, hub::format::BGR8 };
        hub::MetaData metaData;
        metaData["fov"]  = 60.0;
        metaData["iso"]  = 200;
        metaData["date"] = "now";
        const hub::sensor::SensorSpec sensorSpec( "sensorName", { imageResolution }, metaData );

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

            outputSensor << ( hub::sensor::Acquisition { start, end }
                              << hub::Measure { data, size, imageResolution } );
        }
    }

    {
        hub::sensor::InputSensor inputSensor { hub::input::InputStream {
            "streamName", hub::net::ClientSocket { "serverIp", serverPort } } };

        const auto& resolutions = inputSensor.getSpec().getResolutions();
        if ( resolutions.size() == 1 ) {
#if ( __cplusplus >= 201703L )
            const auto& [nDim, format] = resolutions.at( 0 );
#else
            const auto& nDim   = resolutions.at( 0 ).first;
            const auto& format = resolutions.at( 0 ).second;
#endif

            if ( nDim.size() == 2 && format == hub::format::BGR8 ) {
                const auto& imageWidth  = nDim.at( 0 );
                const auto& imageHeight = nDim.at( 1 );

                while ( 1 ) {
                    hub::sensor::Acquisition acq;
                    inputSensor >> acq;
                    const auto& measure = acq.getMeasures().at( 0 );

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

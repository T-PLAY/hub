
// code in README.md of this project
// streamer code

#include <IO/InputStream.hpp>
#include <IO/OutputStream.hpp>
#include <OutputSensor.hpp>

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
//;

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

        hub::OutputSensor outputSensor {
            sensorSpec,
            hub::io::OutputStream { "streamName",
                                    hub::net::ClientSocket { "serverIp", serverPort } } };

        while ( 1 ) {
            auto [start, end] = sensorAPI::getTimestamp();
            auto [data, size] = sensorAPI::getData();

            // send data
            outputSensor << ( hub::Acquisition { start, end }
                              << hub::Measure { data, size, imageResolution } );
        }
    }

    {
        // init input sensor
        hub::InputSensor inputSensor { hub::io::InputStream {
            "streamName", "", hub::net::ClientSocket { "serverIp", serverPort } } };

        const auto& resolutions = inputSensor.m_spec.getResolutions();
        if ( resolutions.size() == 1 ) {
            const auto& [dims, format] = resolutions.at( 0 );

            // if compatible resolution for the client application
            if ( dims.size() == 2 && format == hub::Format::BGR8 ) {
                const auto& imageWidth  = dims.at( 0 );
                const auto& imageHeight = dims.at( 1 );

                while ( 1 ) {
                    // receive data
                    auto acq            = inputSensor.getAcquisition();
                    const auto& measure = acq.getMeasures().at( 0 );

                    // draw image
                    clientApp::drawImage( measure.m_data,
                                          measure.m_size,
                                          imageWidth,
                                          imageHeight,
                                          clientApp::Format::BGR888 );
                }
            }
        }
    }

    return 0;
}

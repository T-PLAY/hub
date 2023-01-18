# Hub

## Description
This library describes a data flow coming from a sensor by proposing an abstraction using different communication bus (file, socket, memory) to the different applications using it. This allows the different client applications to avoid using directly the communication drivers of each sensor.

We thus propose a central server (hub) allowing to separate the applications retrieving the data from each of the sensors from their APIs, which we called streamer. Different from the applications looking at the data that we will called viewer. In this project we focus on two 3D viewers which are Radium and Unity. We propose 2D viewers as well as codes allowing to treat the data streams. Our project consists in gathering all the information coming from the different sensors looking at a physical scene and thus to reconstitute this information in a virtual 3D scene. 

## Installation
```
mkdir build
cmake -B build
cmake --build build
cmake --install build
```

## Usage
### Streamer
```cpp
// init output sensor
const hub::Resolution imageResolution { { imageWidth, imageHeight }, hub::Format::BGR8 };
hub::SensorSpec::MetaData metaData;
metaData["fov"]  = 60.0;
metaData["iso"]  = 200;
metaData["date"] = "now";
const hub::SensorSpec sensorSpec( "sensorName", { imageResolution }, metaData );

hub::OutputSensor outputSensor {
    sensorSpec, hub::io::OutputStream { "streamName", hub::net::ClientSocket { "serverIp", serverPort } } };

while ( 1 ) {
    auto [start, end] = sensorAPI::getTimestamp();
    auto [data, size] = sensorAPI::getData();

    // send data
    outputSensor << ( hub::Acquisition { start, end } << hub::Measure { data, size, imageResolution } );
}
```

### Viewer
```cpp
// init input sensor
hub::InputSensor inputSensor {
    hub::io::InputStream { "streamName", "", hub::net::ClientSocket { "serverIp", serverPort } } };

const auto& resolutions = inputSensor.m_spec.getResolutions();
if ( resolutions.size() == 1 ) {
    const auto& [dims, format] = resolutions.at( 0 );

    // if compatible resolution for the client application
    if ( dims.size() == 2 && format == hub::Format::BGR8 ) {
        const auto& imageWidth  = dims.at( 0 );
        const auto& imageHeight = dims.at( 1 );

        while ( 1 ) {
            // receive data
            auto acq = inputSensor.getAcquisition();
            const auto& measure = acq.getMeasures().at( 0 );

            // draw image
            clientApp::drawImage(
                measure.m_data, measure.m_size, imageWidth, imageHeight, clientApp::Format::BGR888 );
        }
    }
}
```

## Authors and acknowledgment
Gauthier BOUYJOU (IRIT).

## License
No license.

## Project status
Under development (2022-2023).

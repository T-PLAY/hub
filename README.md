# Hub

## Description
This library describes what can be a data flow coming from a sensor by proposing an abstraction of what is a sensor flow using different communication devices (file, socket, memory) to the different applications using it. This allows the different client applications to avoid using directly the communication drivers of each sensor. 

We thus propose a central server (hub) allowing to separate the applications retrieving the data from each of the sensors from their APIs, which we called streamer. Different from the applications looking at the data that we will called viewer. In this project we focus on two 3D viewers which are Radium and Unity. We propose 2D viewers as well as codes allowing to treat the data streams. Our project consists in gathering all the information coming from the different sensors looking at a physical scene and thus to reconstitute this information in a virtual 3D scene. 

## Installation
```
mkdir build
cmake -B build
cmake --build build
```

## Usage
### Streamer
```cpp
// init output sensor
const hub::Resolution imageResolution { { width, height }, hub::Format::BGR8 };
hub::MetaData metaData;
metaData["fov"] = 60.0;
metaData["iso"] = 200;
metaData["date"] = "UTC+now";
const hub::SensorSpec sensorSpec( "sensorName", { imageResolution }, metaData );

hub::net::ClientSocket sock { "ipV4Server", port }; // use stream network with running server
hub::OutputSensor outputSensor( sensorSpec, hub::io::OutputStream( "streamName", std::move ( sock ) ) );

std::fstream outputFile( "filepath", std::ios::out | std::ios::binary | std::ios::trunc ); // use file
hub::OutputSensor outputSensor2( sensorSpec, hub::io::File( std::move( outputFile ) ) );

while ( 1 ) {}
    [start, end] = sensorAPI.getTimestamps(); // correspond of the acquisition date in microseconds
    [data, size] = sensorAPI.getData(); // get data from the sensor

    // send data
    outputSensor << ( hub::Acquisition { start, end } << hub::Measure { data, size, imageResolution } );
}
```

### Viewer
```cpp
// init input sensor
hub::net::ClientSocket sock { "ipV4Server", port }; // use stream network with running server
hub::InputSensor inputSensor( hub::io::InputStream( "streamName", "", std::move( sock ) ) );

std::fstream inputFile( "filepath", std::ios::in | std::ios::binary ); // use file
hub::InputSensor inputSensor2( hub::io::File( std::move( inputFile ) ) );

// A resolutions size strictly greater than 1 corresponds to sensors that measuring information at the same time with similar 
// acquisition times or may correspond to a synchronization of different sensor streams that is proposed by our server.
const auto& resolutions = inputSensor.m_spec.getResolutions();
if ( resolutions.size() == 1 ) {
    const auto& [dims, format] = resolutions.at( 0 );

    // get acquisition only if client application is able to use compatible data format and dimension of data.
    if ( dims.size() == 2 && format == hub::Format::BGR8 ) {
        const auto& width = dims.at( 0 );
        const auto& height = dims.at ( 1 );

        while ( 1 ) {
            // get acquisition from the opening stream and perform something with the sensor data 
            auto acq = inputSensor.getAcquisition();
            const auto & measure = acq.getMeasures().at( 0 );

            // draw image with acquisition data
            ClientApp.draw2DImage( measure.m_data, measure.m_size, width, height, ClientApp.Format.BGR888 );
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

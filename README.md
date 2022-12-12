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
const hub::Resolution resolution { { width, height }, hub::Format::BGR8 };
const hub::SensorSpec sensorSpec( "sensorName", { resolution } );

// use stream network with running server
hub::net::ClientSocket sock { "ipV4Server", port }; // port corresponds to the number of oppened service within the server
hub::OutputSensor outputSensor( sensorSpec, hub::io::OutputStream( "streamName", std::move ( sock ) ) );

// or use file
std::fstream outputFile( "filepath", std::ios::out | std::ios::binary | std::ios::trunc );
hub::OutputSensor outputSensor( sensorSpec, hub::io::File( std::move( outputFile ) ) );
// end init output sensor

// get acquisition from driver and send it through the stream
// long long start and end correspond of the date of acquisition in microseconds (timestamp)
// unsigned char * data corresponds of the raw data comming from sensor (get by the sensor manufacturer's api)
// size corresponds to the size of raw data
while ( 1 ) {
    // get data from sensor
    // start, end = sensorAPI.getTimestamps()
    // data, size = sensorAPI.getData()

    // send data
    outputSensor << ( hub::Acquisition { start, end } << hub::Measure { data, size, resolution });
}
```

### Viewer
```cpp
// init input sensor
// use stream network with running server
hub::net::ClientSocket sock { "ipV4Server", port };
hub::InputSensor inputSensor( hub::io::InputStream( "streamName", "", std::move( sock ) ) );

// or use file
std::fstream inputFile( "filepath", std::ios::in | std::ios::binary );
hub::InputSensor inputSensor( hub::io::File( std::move( inputFile )));
// end init input sensor

// get acquisition from the opening stream and perform something with the sensor data 
while ( 1 ) {
    auto acq = inputSensor.getAcquisition();
    // process something with acquisition
    // examples : draw 2D image, 3D volume, move object in the 3D scene, apply transformation matrix, etc.
    for ( const auto& measure : measures ) {
        const auto resolution = measure.getResolution();
	    const auto dims = resolution.first;
        
        if ( resolution.second == hub::Format::BGR8 ) {
            
            if ( dims.size() == 2 ) {
                const auto width  = dims.at( 0 );
                const auto height = dims.at( 1 );
                // draw image with acqusition data
                ClientApp.draw2DImage( measure.m_data, measure.m_size );
            }
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

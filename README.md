# Hub

## Description
This library describes the communication protocol of [TPlay](https://github.com/T-PLAY): a flow of data coming from a sensor by proposing an abstraction using different communication bus (file, socket, memory) to the different applications using it. This allows the different client applications to avoid using directly the communication drivers of each sensor.

We thus propose a central server (hub) allowing to separate the applications retrieving the data from each of the sensors from their APIs, which we called streamer. Different from the applications looking at the data that we will called viewer. In this project we focus on two 3D viewers which are Radium and Unity. We propose 2D viewers as well as codes allowing to treat the data streams. Our project consists in gathering all the information coming from the different sensors looking at a physical scene and thus to reconstitute this information in a virtual 3D scene. 

## Supported platforms
* Linux
* Windows (10, 11)
* UWP (Universal Windows)
* OSX (11.6)

## Supported compilers
* Clang 15 and above
* GCC 12 and above
* VS2019 and above

## Languages
* C/C++ API documentation

## Installation
```
mkdir build
cmake -B build -DCMAKE_INSTALL_PREFIX=hub-install
cmake --build build
cmake --install build
```

## Usage
### Streamer
```cpp
constexpr auto width  = 640;
constexpr auto height = 480;

using Format     = hub::format::BGR8;
using Resolution = hub::MatrixXD<Format, width, height>;
hub::MetaData metaData;
metaData["fov"]  = 60.0;
metaData["iso"]  = 200;
metaData["date"] = "now";
const hub::sensor::SensorSpec sensorSpec( "sensorName", Resolution(), metaData );

hub::sensor::OutputSensorT<Resolution> outputSensor { sensorSpec, "streamName" };
auto acq          = outputSensor.acqMsg();
auto [start, end] = acq.clocks();
auto* const imgData = acq.get<Format*>();
constexpr auto imgSize = acq.getSize<hub::format::BGR8>();

while ( 1 ) {
    start = sensorAPI::getStart();
    end = sensorAPI::getEnd();
    sensorAPI::fetchData((unsigned char*)imgData, imgSize);

    outputSensor << acq;
}
```

### Viewer
```cpp
// init input sensor
hub::InputSensor inputSensor { hub::input::InputStream( "streamName" ) };

auto acq                   = inputSensor.acqMsg();
auto& start                = acq.start();
auto& end                  = acq.end();
auto* const imgData = acq.get<hub::format::BGR8*>();
const auto & dims = acq.getDims<hub::format::BGR8>();
const auto width = dims.at(0);
const auto height = dims.at(1);

while ( 1 ) {
    // receive data
    inputSensor >> acq;

    // draw image
    clientApp::drawImage(
        (unsigned char*)imgData, width, height, clientApp::Format::BGR888 );
}
```

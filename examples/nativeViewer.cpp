
#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>
#include <Viewer.hpp>

// #include <Server.hpp>

#include <Native.hpp>
#include <filesystem>
#include <iostream>

// needs server running

// void onNewStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//}
// void onDelStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//}

int main() {

    auto onNewStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onNewStreamer : " << streamName << std::endl;
        char sensorName[80] = { 0 };
        int strLen          = 0;
        hub::native::sensorSpec_getSensorName( sensorSpec, sensorName, &strLen );
        std::cout << "[Example][Viewer] sensorName : '" << sensorName << "' size = " << strLen
                  << std::endl;
        const int resolutionsSize = hub::native::sensorSpec_getResolutionsSize( sensorSpec );
        std::cout << "[Example][Viewer] resolutions size : " << resolutionsSize << std::endl;
        for ( int iResolution = 0; iResolution < resolutionsSize; ++iResolution ) {
            std::cout << "[Example][Viewer] resolutions[" << iResolution << "] format : "
                      << hub::native::sensorSpec_getFormat( sensorSpec, iResolution ) << std::endl;
            const int dimensionsSize =
                hub::native::sensorSpec_getDimensionsSize( sensorSpec, iResolution );
            std::cout << "[Example][Viewer] resolutions[" << iResolution
                      << "] dimensions size : " << dimensionsSize << std::endl;

            for ( int iDimension = 0; iDimension < dimensionsSize; ++iDimension ) {
                std::cout << "[Example][Viewer] resolutions[" << iResolution << "] dimensions["
                          << iDimension << "] size : "
                          << hub::native::sensorSpec_getDimension(
                                 sensorSpec, iResolution, iDimension )
                          << std::endl;
            }
        }
        std::cout << "[Example][Viewer] acquisitionsSize : "
                  << hub::native::sensorSpec_getAcquisitionSize( sensorSpec ) << std::endl;

        char resolutionsStr[80] = { 0 };
        hub::native::sensorSpec_getResolutionsStr( sensorSpec, resolutionsStr );
        std::cout << "[Example][Viewer] resolutionsStr : '" << resolutionsStr << "'" << std::endl;

        char metaDataStr[80] = { 0 };
        hub::native::sensorSpec_getMetaDataStr( sensorSpec, metaDataStr );
        std::cout << "[Example][Viewer] metaDataStr : '" << metaDataStr << "'" << std::endl;
        return true;
    };
    auto onDelStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onDelStreamer " << streamName << std::endl;
    };
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerConnected " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << "[Example][Viewer] onServerDisconnected " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const char* streamName, const hub::Acquisition* acq ) {
        std::cout << "[Example][Viewer] onNewAcquisition " << streamName << " " << *acq
                  << std::endl;
    };
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << "[Example][Viewer] onLogMessage " << logMessage << " " << *logMessage
                  << std::endl;
    };

    auto viewer = hub::native::createViewer( onNewStreamer,
                                             onDelStreamer,
                                             onServerConnected,
                                             onServerDisconnected,
                                             onNewAcquisition,
                                             hub::net::s_defaultServiceIp.c_str(),
                                             hub::net::s_defaultServicePort,
                                             onLogMessage );

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
}

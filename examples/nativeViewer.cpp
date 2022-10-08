
#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>
#include <Viewer.hpp>

#include <Server.hpp>

#include <Native.hpp>
#include <filesystem>
#include <iostream>

// needs server running

// void onNewStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//    std::cout << "[Example][Viewer] onNewStreamer" << std::endl;
//}
// void onDelStreamer( const char* sensorName, const hub::SensorSpec* sensorSpec ) {

//    std::cout << "[Example][Viewer] onDelStreamer" << std::endl;
//}

int main() {

    auto onNewStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onNewStreamer : " << streamName << std::endl;
        char sensorName[80] = {0};
//        const char * sensorName = nullptr;
        int strLen = 0;
        hub::native::sensorSpec_getSensorName(sensorSpec, sensorName, &strLen);
//        sensorName = hub::native::sensorSpec_getSensorName(sensorSpec);
        std::cout << "[Example][Viewer] sensorName : '"
                  << sensorName << "' size = " << strLen << std::endl;
//        delete sensorName;
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

        char resolutionsStr[80] = {0};
        hub::native::sensorSpec_getResolutionsStr(sensorSpec, resolutionsStr);
        std::cout << "[Example][Viewer] resolutionsStr : '"
                  << resolutionsStr << "'" << std::endl;

        char metaDataStr[80] = {0};
        hub::native::sensorSpec_getMetaDataStr(sensorSpec, metaDataStr);
        std::cout << "[Example][Viewer] metaDataStr : '"
                  << metaDataStr << "'" << std::endl;

    };
    auto onDelStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "[Example][Viewer] onDelStreamer" << std::endl;
    };
    auto onServerConnected = [](const std::string& ipv4, int port) {
        std::cout << "[Example][Viewer] onServerConnected" << std::endl;
    };
    auto onServerDisconnected = [](const std::string& ipv4, int port) {
        std::cout << "[Example][Viewer] onServerDisconnected" << std::endl;
    };

    auto viewer = hub::native::createViewer(
        //        onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected,
        onNewStreamer,
        onDelStreamer,
        onServerConnected,
        onServerDisconnected
//                );
                , hub::net::s_defaultServiceIp.c_str(), hub::net::s_defaultServicePort );

//    while ( true ) {
//        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
//    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

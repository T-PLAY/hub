
#include <iostream>

#include <Native.hpp>

int main() {

    //    auto onNewStreamer = [=]( const std::string& streamName, const hub::SensorSpec& sensorSpec
    //    ) {
    auto onNewStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "\t[nativeViewer] onNewStreamer : " << streamName << std::endl;
        return true;
    };
    //    auto onDelStreamer = []( const std::string& streamName, const hub::SensorSpec& sensorSpec
    //    ) {
    auto onDelStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << "\t[nativeVviewer] onDelStreamer : " << streamName << std::endl;
    };
    //    auto onServerNotFound = []( const std::string& ipv4, int port ) {
    auto onServerNotFound = []( const char* ipv4, int port ) {
        std::cout << "\t[nativeVviewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    //    auto onServerConnected = []( const std::string& ipv4, int port ) {
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << "\t[nativeVviewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    //    auto onServerDisconnected = []( const std::string& ipv4, int port ) {
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << "\t[nativeVviewer] onServerDisconnected : " << ipv4 << " " << port
                  << std::endl;
    };
    //    auto onNewAcquisition = []( const std::string& streamName, const hub::Acquisition& acq ) {
    auto onNewAcquisition = []( const char* streamName, const hub::Acquisition* acq ) {
        std::cout << "\t[nativeVviewer] onNewAcquisition : " << acq << std::endl;
    };
    //    auto onSetProperty = []( const std::string& streamName,
    //                             const std::string& objectName,
    //                             int property,
    //                             const hub::Any& value ) {
    auto onSetProperty =
        []( const char* streamName, const char* objectName, int property, const hub::Any* value ) {
            std::cout << "\t[nativeViewer] onSetProperty " << streamName << " " << objectName << " "
                      << property << " " << value << std::endl;
        };
    //    auto onLogMessage = []( const std::string& logMessage ) {
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << "\t[nativeVviewer] onLogMessage '" << logMessage << "'" << std::endl;
    };

    auto* viewer = hub::native::createViewer( __FILE_NAME__,
                                              onNewStreamer,
                                              onDelStreamer,
                                              onServerNotFound,
                                              onServerConnected,
                                              onServerDisconnected,
                                              onNewAcquisition,
                                              onSetProperty,
                                              onLogMessage );

    std::cout << "\t[[nativeVviewer] Ctrl+C to exit" << std::endl;

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}

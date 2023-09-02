
#include <iostream>

#include <Native.hpp>

#define HEADER_MSG "\t[NativeViewer] "

int main() {

    auto onNewStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << HEADER_MSG "onNewStreamer : " << streamName << std::endl;
        return true;
    };
    auto onDelStreamer = []( const char* streamName, const hub::SensorSpec* sensorSpec ) {
        std::cout << HEADER_MSG "onDelStreamer : " << streamName << std::endl;
    };
    auto onServerNotFound = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const char* streamName, const hub::Acquisition* acq ) {
        std::cout << HEADER_MSG "onNewAcquisition : " << acq << std::endl;
    };
    auto onSetProperty =
        []( const char* streamName, const char* objectName, int property, const hub::Any* value ) {
            std::cout << HEADER_MSG "onSetProperty " << streamName << " " << objectName << " "
                      << property << " " << value << std::endl;
        };
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << HEADER_MSG "onLogMessage '" << logMessage << "'" << std::endl;
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

    std::cout << HEADER_MSG "Ctrl+C to exit" << std::endl;

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}

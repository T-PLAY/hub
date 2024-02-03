

#include <client/Viewer.hpp>

/// \file
/// \warning
/// Needs running server.

int main() {

    auto onNewStreamer = [=]( const std::string& streamName,
                              const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;
        return true;
    };
    auto onDelStreamer = []( const std::string& streamName,
                             const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
    };
    auto onServerNotFound = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    auto onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const std::string& streamName,
                                const hub::sensor::Acquisition& acq ) {
        std::cout << "[example-viewer] onNewAcquisition : " << acq << std::endl;
    };
    auto onSetProperty = []( const std::string& streamName,
                             const std::string& objectName,
                             int property,
                             const hub::Any& value ) {
        std::cout << "[example-viewer] onSetProperty " << streamName << std::endl;
    };
    auto onLogMessage = []( const std::string& logMessage ) {
        std::cout << "[example-viewer] onLogMessage '" << logMessage << "'" << std::endl;
    };

    std::string ipServer = "192.168.2.1";

    hub::client::Viewer viewer { onNewStreamer,
                                 onDelStreamer,
                                 onServerNotFound,
                                 onServerConnected,
                                 onServerDisconnected,
                                 onNewAcquisition,
                                 onSetProperty,
                                 ipServer.c_str(),
                                 HUB_SERVICE_PORT,
                                 false,
                                 onLogMessage };

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
    std::cout << "Starting viewer listening" << std::endl
              << "Press any key to terminate" << std::endl;
    getchar();

    return 0;
}

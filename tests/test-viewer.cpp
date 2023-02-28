

// #include <stdio.h>
// #include <cstdio>
// #include <stdlib.h>
// #include <stdio.h>
// #include <iostream>

#include <Viewer.hpp>

#include <Server.hpp>

// #include <filesystem>
// #include <iostream>

//clang-format off
int main() {

    const std::string ipv4 = "127.0.0.1";
    constexpr int port     = 9002;

    std::cout << "[Test] ############################### server start" << std::endl;
    Server server( port );
    server.setMaxClients( 1 );
    //    server.setAcqPing( false );
    server.asyncRun();

    // startConstruction
    auto onNewStreamer = [=]( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onNewStreamer : " << streamName << std::endl;
        // accept all stream to run
        return true;
    };
    auto onDelStreamer = []( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[example-viewer] onDelStreamer : " << streamName << std::endl;
    };
    auto onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << "[example-viewer] onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const std::string& streamName, const hub::Acquisition& acq ) {
        std::cout << "[example-viewer] onNewAcquisition : " << acq << std::endl;
    };

    auto viewer = hub::Viewer(
        onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected, onNewAcquisition );

    viewer.setAutoSync(false);
    // endConstruction

//    std::cout << "Starting viewer listening" << std::endl
//              << "Press any key to terminate" << std::endl;
//    getchar();

    return 0;
}
//clang-format on

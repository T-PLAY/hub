
#include <IO/File.hpp>
#include <IO/Stream.hpp>
#include <InputSensor.hpp>
#include <Net/ClientSocket.hpp>
#include <OutputSensor.hpp>
#include <Viewer.hpp>

#include <Server.hpp>

#include <filesystem>
#include <iostream>

// needs server running

int main() {

    auto onNewStreamer = [=]( const std::string& sensorName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[Example][Viewer] onNewStreamer" << std::endl;
    };
    auto onDelStreamer = []( const std::string& sensorName, const hub::SensorSpec& sensorSpec ) {
        std::cout << "[Example][Viewer] onDelStreamer" << std::endl;
    };
    auto onServerConnected = []() {
        std::cout << "[Example][Viewer] onServerConnected" << std::endl;
    };
    auto onServerDisconnected = []() {
        std::cout << "[Example][Viewer] onServerDisconnected" << std::endl;
    };

    auto viewer = hub::Viewer(
        onNewStreamer, onDelStreamer, onServerConnected, onServerDisconnected );

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }
}

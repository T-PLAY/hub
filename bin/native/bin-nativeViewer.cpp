
#include <iostream>
#include <thread>
#include <string>

// #include <io/Stream.hpp>
#include <native/Native.hpp>

#define COLOR "\033[44m"
#define HEADER_MSG "\t\t\t" COLOR "[NativeViewer]\033[0m "

int main( int argc, char* argv[] ) {

    static bool exitWhenServerLost = false;

    std::vector<std::string> args( argv + 1, argv + argc );

    // int port = hub::io::Stream::s_defaultPort;
    int port = HUB_SERVICE_PORT;

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: [--port <int>] [--exitWhenServerLost]" << std::endl;
            return 0;
        }
        else if ( arg == "--exitWhenServerLost" ) { exitWhenServerLost = true; }
        else if ( arg == "--port" ) {
            assert( it + 1 != args.end() );
            const auto& nextArg = *( it + 1 );
            port                = std::atoi( nextArg.c_str() );
            ++it;
        }
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--exitWhenServerLost]" << std::endl;
            return 0;
        }
        ++it;
    }

    static bool exit = false;

    auto onServerNotFound = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
        // if ( exitWhenServerLost ) { exit = true; }
    };
    auto onServerConnected = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const char* ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
        if ( exitWhenServerLost ) { exit = true; }
    };
    // auto onNewStream = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
    auto onNewStream = []( const char* streamName, const hub::io::Header* header ) {
        std::cout << HEADER_MSG "onNewStream : " << streamName << ", " << header << std::endl;
        return true;
    };
#ifndef HUB_NON_BUILD_SENSOR
    auto onNewSensor = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
        // std::cout << HEADER_MSG "onNewSensor : " << streamName << ", " << *sensorSpec <<
        // std::endl;
        std::cout << HEADER_MSG "onNewSensor : " << streamName << ", "
                  << hub::native::to_string( sensorSpec ) << std::endl;
        return true;
    };
#endif
    // auto onDelStream = []( const char* streamName, const hub::sensor::SensorSpec* sensorSpec ) {
    auto onDelStream = []( const char* streamName ) {
        std::cout << HEADER_MSG "onDelStream : " << streamName << std::endl;
    };
    auto onNewData = []( const char* streamName, const hub::Datas_t* datas ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "m-\033[0m";
    };
#ifndef HUB_NON_BUILD_SENSOR
    auto onNewAcq = []( const char* streamName, const hub::sensor::Acquisition* acq ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "mA\033[0m";
    };
#endif
    auto onSetProperty =
        []( const char* streamName, const char* objectName, int property, const hub::Any* value ) {
            std::cout << HEADER_MSG "onSetProperty " << streamName << " " << objectName << " "
                      << property << " " << value << std::endl;
        };
    auto onLogMessage = []( const char* logMessage ) {
        std::cout << HEADER_MSG "onLogMessage '" << logMessage << "'" << std::endl;
    };

    auto* viewerHandler = hub::native::createViewerHandler( onServerNotFound,
                                                            onServerConnected,
                                                            onServerDisconnected,
                                                            onNewStream,
#ifndef HUB_NON_BUILD_SENSOR
                                                            onNewSensor,
#endif
                                                            onNewData,
#ifndef HUB_NON_BUILD_SENSOR
                                                            onNewAcq,
#endif
                                                            onDelStream,
                                                            onSetProperty,
                                                            onLogMessage );

    auto* viewer = hub::native::createViewer( FILE_NAME.c_str(), viewerHandler, "127.0.0.1", port );

    std::cout << HEADER_MSG "Ctrl+C to exit" << std::endl;

    while ( !exit ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    hub::native::freeViewer( viewer );
}

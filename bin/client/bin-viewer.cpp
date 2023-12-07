
#include <iostream>

#include <client/Viewer.hpp>

#define COLOR "\033[41m"
#define HEADER_MSG "\t\t" COLOR "[Viewer]\033[0m "

// int main() {
int main( int argc, char* argv[] ) {

    bool exitWhenServerLost = false;

    std::vector<std::string> args( argv + 1, argv + argc );

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: [--exitWhenServerLost]" << std::endl;
            return 0;
        }
        else if ( arg == "--exitWhenServerLost" ) { exitWhenServerLost = true; }
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--exitWhenServerLost]" << std::endl;
            return 0;
        }
        ++it;
    }

    bool exit = false;

    hub::client::ViewerHandler viewerHandler;
    // auto onNewStreamer = [=]( const std::string& streamName, const hub::sensor::SensorSpec&
    // sensorSpec ) {
    viewerHandler.onNewStreamer = [=]( const std::string& streamName,
                                       const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onNewStreamer : " << streamName << ", " << sensorSpec << std::endl;
        return true;
    };
    viewerHandler.onDelStreamer = []( const std::string& streamName,
                                      const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onDelStreamer : " << streamName << ", " << sensorSpec << std::endl;
    };
    viewerHandler.onServerNotFound = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
        if ( exitWhenServerLost ) { exit = true; }
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
        if ( exitWhenServerLost ) { exit = true; }
    };
    viewerHandler.onNewAcquisition = []( const std::string& streamName,
                                         const hub::sensor::Acquisition& acq ) {
        //        std::cout << HEADER_MSG "onNewAcquisition : " << acq << std::endl;
        //        std::cout << "+";
        std::cout << COLOR "+\033[0m";
    };
    viewerHandler.onSetProperty = []( const std::string& streamName,
                                      const std::string& objectName,
                                      int property,
                                      const hub::Any& value ) {
        std::cout << HEADER_MSG "onSetProperty " << streamName << " " << objectName << " "
                  << property << " " << value << std::endl;
    };
    viewerHandler.onLogMessage = []( const std::string& logMessage ) {
        std::cout << HEADER_MSG "onLogMessage '" << logMessage << "'" << std::endl;
    };

    hub::client::Viewer viewer( FILE_NAME, std::move( viewerHandler ));
                                // onNewStreamer,
                                // onDelStreamer,
                                // onServerNotFound,
                                // onServerConnected,
                                // onServerDisconnected,
                                // onNewAcquisition,
                                // onSetProperty,
                                // onLogMessage
    // );

    std::cout << "\t[viewer] Ctrl+C to exit" << std::endl;

    while ( !exit ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}

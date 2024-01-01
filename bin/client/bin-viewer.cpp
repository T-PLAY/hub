
#include <iostream>

#include <client/Viewer.hpp>
#include <io/Stream.hpp>

#define COLOR "\033[41m"
#define HEADER_MSG "\t\t" COLOR "[Viewer]\033[0m "

int main( int argc, char* argv[] ) {

    bool exitWhenServerLost = false;

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

    bool exit = false;

    hub::client::ViewerHandler viewerHandler;
    viewerHandler.onServerNotFound = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
        // if ( exitWhenServerLost ) { exit = true; }
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
        if ( exitWhenServerLost ) { exit = true; }
    };
    // auto onNewStream = [=]( const std::string& streamName, const hub::sensor::SensorSpec&
    // sensorSpec ) {
    viewerHandler.onNewStream = [=]( const std::string& streamName,
                                     const hub::io::Header& header ) {
        std::cout << HEADER_MSG "onNewStream : " << streamName << ", " << header << std::endl;
        return true;
    };
#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler.onNewSensor = [=]( const std::string& streamName,
                                     const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onNewSensor : " << streamName << ", " << sensorSpec << std::endl;
        return true;
    };
#endif
    viewerHandler.onNewData = []( const std::string& streamName, const hub::Datas_t& datas ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "m+\033[0m" << std::flush;
    };
#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler.onNewAcq = []( const std::string& streamName,
                                 const hub::sensor::Acquisition& acq ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "ma\033[0m" << std::flush;
    };
#endif
    // viewerHandler.onNewAcquisition = []( const std::string& streamName,
    //                                      const hub::sensor::Acquisition& acq ) {
    //     //        std::cout << HEADER_MSG "onNewAcquisition : " << acq << std::endl;
    //     //        std::cout << "+";
    //     std::cout << COLOR "+\033[0m";
    // };
    viewerHandler.onDelStream = []( const std::string& streamName ) {
        // const hub::io::Header & header ) {
        // std::cout << HEADER_MSG "onDelStream : " << streamName << ", " << header << std::endl;
        std::cout << HEADER_MSG "onDelStream : " << streamName << std::endl;
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

    hub::client::Viewer viewer( FILE_NAME, std::move( viewerHandler ), "127.0.0.1", port );

    std::cout << "\t[viewer] Ctrl+C to exit" << std::endl;

    if ( exitWhenServerLost ) {
        while ( !exit ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
        }
    }
    else {
        // std::cout << "Ctrl+C or Escape to exit" << std::endl;
        // while ( getchar() != 27 ) { // ESC to quit
        // viewer.printStatus();
        // }
        const auto helperMsg = std::string(argv[0]) + " info: [.|Esc] -> exit, [F5|' '] -> print stats, h -> print this helper message";
        std::cout << helperMsg << std::endl;
        // while(true) {
        // }
        // while ( server.running() && getchar() != 27 ) { // ESC to quit
        bool exit = false;
        while ( !exit ) {                             // ESC to quit
            const auto key = hub::utils::key_press(); // blocks until a key is pressed
            // std::cout << "Input is: " << std::to_string( key ) << ", \"" << (char)key << "\""
            // std::cout << "Input is: " << key << std::endl;
            switch ( key ) {
            case hub::utils::Key::F5:
            case hub::utils::Key::Space:
                viewer.printStatus();
                break;
            case hub::utils::Key::Escape:
            case hub::utils::Key::Dot:
                exit = true;
                break;
            case hub::utils::Key::h:
                std::cout << helperMsg << std::endl;
                break;
            default:
                std::cout << "unrecognized key : " << key << std::endl;
            }

            // server.printStatus();
        }
        std::cout << "exiting" << std::endl;
    }
}

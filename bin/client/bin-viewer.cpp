
#include <iostream>

#include <client/Viewer.hpp>
#include <io/Stream.hpp>

#define COLOR "\033[41m"
#define HEADER_MSG "\t\t" COLOR "[Viewer]\033[0m "

int main( int argc, char* argv[] ) {

    bool exitWhenServerLost = false;

    std::vector<std::string> args( argv + 1, argv + argc );

    int port = hub::io::Stream::s_defaultPort;

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
    viewerHandler.onNewStream = [=]( const std::string& streamName, const hub::io::Header& header ) {
        std::cout << HEADER_MSG "onNewStream : " << streamName << ", " << header << std::endl;
        return true;
        // return header.getDataSize() > 0;
    };
    viewerHandler.onNewData = []( const std::string& streamName,
                                  const hub::Datas_t& datas ) {
                                  // hub::input::InputStream& inputStream ) {
        // std::cout << COLOR "+\033[0m";
        // int a;
        // inputStream.read( a );
        std::cout << "[test-client-Viewer] '" << streamName << "', onNewData : " << datas << std::endl;
    };
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

    while ( !exit ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}

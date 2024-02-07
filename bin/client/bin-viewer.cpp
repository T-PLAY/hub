
#include <iostream>

#include <client/Viewer.hpp>
#include <io/Stream.hpp>

#define COLOR "\033[41m"
#define HEADER_MSG "\t\t" COLOR "[Viewer]\033[0m "

int main( int argc, char* argv[] ) {

    std::vector<std::string> args( argv + 1, argv + argc );

    bool exitWhenServerLost = false;
    int port                = HUB_SERVICE_PORT;

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

    class StreamLife
    {
      public:
        StreamLife( const std::string& streamName, size_t dataSize ) :
            m_streamName( streamName ),
            m_dataSize( dataSize ),
            m_counterAcq( 0 ),
            m_lastUpdateClock( std::chrono::high_resolution_clock::now() ) {}

        void newData() {
            ++m_counterAcq;
            const auto now = std::chrono::high_resolution_clock::now();
            const auto period =
                std::chrono::duration_cast<std::chrono::milliseconds>( now - m_lastUpdateClock )
                    .count();

            if ( period > 1'000 ) { // 1 sec
                const auto bytePerSecond = ( 1000.0 * m_counterAcq * m_dataSize ) / period;
                std::cout << HEADER_MSG << m_streamName
                          << " data rate : " << PRETTY_BYTES( bytePerSecond ) << "/s" << std::endl;
                m_lastUpdateClock = now;
                m_counterAcq      = 0;
            }
        }

      private:
        std::string m_streamName;
        size_t m_dataSize;
        size_t m_counterAcq;
        std::chrono::high_resolution_clock::time_point m_lastUpdateClock;
    };
    std::map<std::string, std::unique_ptr<StreamLife>> m_streamLives;

    hub::client::ViewerHandler viewerHandler;
    viewerHandler.onServerNotFound = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    viewerHandler.onServerDisconnected = [&]( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
        if ( exitWhenServerLost ) { exit = true; }
    };
    viewerHandler.onNewStream = [&]( const std::string& streamName,
                                     const hub::io::Header& header ) {
        std::cout << HEADER_MSG "onNewStream : " << streamName << ", " << header << std::endl;
        assert( m_streamLives.find( streamName ) == m_streamLives.end() );
        m_streamLives[streamName] =
            std::make_unique<StreamLife>( streamName, header.getDataSize() );
        return true;
    };

#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler.onNewSensor = [&]( const std::string& streamName,
                                     const hub::sensor::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onNewSensor : " << streamName << ", " << sensorSpec << std::endl;
        assert( m_streamLives.find( streamName ) == m_streamLives.end() );
        m_streamLives[streamName] =
            std::make_unique<StreamLife>( streamName, sensorSpec.dataSize() );
        return true;
    };
#endif
    viewerHandler.onNewData = [&]( const std::string& streamName, const hub::Datas_t& datas ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "m+\033[0m" << std::flush;
        m_streamLives.at( streamName )->newData();
    };
#ifndef HUB_NON_BUILD_SENSOR
    viewerHandler.onNewAcq = [&]( const std::string& streamName,
                                  const hub::sensor::Acquisition& acq ) {
        std::cout << "\033[" << std::to_string( std::hash<std::string> {}( streamName ) % 10 + 40 )
                  << "ma\033[0m" << std::flush;
        m_streamLives.at( streamName )->newData();
    };
#endif
    viewerHandler.onDelStream = [&]( const std::string& streamName ) {
        std::cout << HEADER_MSG "onDelStream : " << streamName << std::endl;
        assert( m_streamLives.find( streamName ) != m_streamLives.end() );
        m_streamLives.erase( streamName );
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
        const auto helperMsg =
            std::string( argv[0] ) +
            " info: [.|Esc] -> exit, [F5|' '] -> print stats, h -> print this helper message";
        std::cout << helperMsg << std::endl;
        bool exit = false;
        while ( !exit ) {                             // ESC to quit
            const auto key = hub::utils::key_press(); // blocks until a key is pressed
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
        }
        std::cout << "exiting" << std::endl;
    }
}

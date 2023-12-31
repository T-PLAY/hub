

// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/StreamServer.hpp>
// #include <io/Server.hpp>
#include <io/Stream.hpp>
#include <server/Server.hpp>

int main( int argc, char* argv[] ) {

    // for (int i = 0; i < 50; ++i) {
    //     std::cout << std::to_string(i) << ": " << "\033[" << i << "mhello\033[0m" << std::endl;
    // }
    // return 0;

    // int maxClient = -1;
    int port = HUB_SERVICE_PORT;
    // int port = hub::io::Stream::s_defaultPort;
    // #ifdef HUB_SERVICE_PORT
    // #else
    // int port = 0;
    // #endif

    bool daemon = false;

    std::vector<std::string> args( argv + 1, argv + argc );

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: [--port <int>]" << std::endl;
            return 0;
        }
        else if ( arg == "--port" ) {
            assert( it + 1 != args.end() );
            const auto& nextArg = *( it + 1 );
            port                = std::atoi( nextArg.c_str() );
            ++it;
        }
        else if ( arg == "--daemon" ) {
            daemon = true;
        }
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--maxClient <int>]" << std::endl;
            return 0;
        }
        ++it;
    }

    // hub::Server server( port );

    hub::Server server( port );

    if ( daemon ) { server.run(); }
    else {
        // if ( maxClient != -1 ) { server.setMaxClients( maxClient ); }

        server.asyncRun();
        std::cout << "Ctrl+C or Escape to exit" << std::endl;
        while ( server.running() && getchar() != 27 ) { // ESC to quit
            server.printStatus();
        }
        std::cout << "exiting" << std::endl;
    }

    return 0;
}

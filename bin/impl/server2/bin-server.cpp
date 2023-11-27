

#include <impl/server2/Server.hpp>
// #include <impl/server2/io/StreamServer.hpp>
#include <io/Stream.hpp>
#include <io/Server.hpp>

int main( int argc, char* argv[] ) {

    // int maxClient = -1;
    int port = hub::io::Stream::s_defaultPort;

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
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--maxClient <int>]" << std::endl;
            return 0;
        }
        ++it;
    }

    hub::Server server( port );

    // if ( maxClient != -1 ) { server.setMaxClients( maxClient ); }

    server.run();

    // server.asyncRun();
    // std::cout << "Ctrl+C or Escape to exit" << std::endl;
    // while ( server.running() && getchar() != 27 ) { // ESC to quit
    //     server.printStatus();
    // }
    // std::cout << "exiting" << std::endl;

    return 0;
}

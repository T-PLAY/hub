

#include <server/Server.hpp>

int main( int argc, char* argv[] ) {

    int maxClient = -1;
//    int maxClient = 3;

    std::vector<std::string> args( argv + 1, argv + argc );

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: [--maxClient <int>]" << std::endl;
            return 0;
        }
        else if ( arg == "--maxClient" ) {
            assert(it + 1 != args.end());
            const auto& nextArg = *( it + 1 );
            maxClient           = std::atoi( nextArg.c_str() );
            ++it;
        }
        else {
            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: [--maxClient <int>]" << std::endl;
            return 0;
        }
        ++it;
    }

    hub::Server server;
    if ( maxClient != -1 ) { server.setMaxClients( maxClient ); }
//    server.run();
    server.asyncRun();


    std::cout << "Ctrl+C to exit" << std::endl;

//    char c;
    while ( getchar() != 27 ) { // ESC to quit
//        getchar();
        server.printStatus();
//        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }

    std::cout << "exiting" << std::endl;

    return 0;
}



#include <server/Server.hpp>

int main( int argc, char* argv[] ) {

    std::vector<std::string> args( argv + 1, argv + argc );

    int maxClient = -1;
//    int maxClient = 3;

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: --maxClient <int>" << std::endl;
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
            std::cout << argv[0] << " usage: --maxClient <int>" << std::endl;
            return 0;
        }
        ++it;
    }

    hub::Server server;
    if ( maxClient != -1 ) { server.setMaxClients( maxClient ); }
    server.run();

    return 0;
}

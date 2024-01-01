

// #include <impl/server2/Server.hpp>
// #include <impl/server2/io/StreamServer.hpp>
// #include <io/Server.hpp>
#include <core/Utils.hpp>
#include <io/Stream.hpp>
#include <server/Server.hpp>

int main( int argc, char* argv[] ) {
    // while ( true ) {                              // ESC to quit
    //     const auto key = hub::utils::key_press(); // blocks until a key is pressed
    //     // std::cout << "Input is: " << std::to_string( key ) << ", \"" << (char)key << "\""
    //     std::cout << "Input is: " << key << std::endl;
    //     switch ( key ) {
    //     case hub::utils::Key::F5:
    //         std::cout << "printStatus" << std::endl;
    //         break;
    //     case hub::utils::Key::Escape:
    //         std::cout << "exit" << std::endl;
    //         break;
    //     default:
    //         std::cout << "unrecognized key : " << key << std::endl;
    //     }

    //     // server.printStatus();
    // }
    // return 0;

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
        else if ( arg == "--daemon" ) { daemon = true; }
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
        const auto helperMsg = std::string(argv[0]) + " info: [.|Esc] -> exit, [F5|' '] -> print stats, h -> print this helper message";
        std::cout << helperMsg << std::endl;
        // while(true) {
        // }
        // while ( server.running() && getchar() != 27 ) { // ESC to quit
        bool exit = false;
        while ( !exit && server.running() ) {         // ESC to quit
            const auto key = hub::utils::key_press(); // blocks until a key is pressed
            // std::cout << "Input is: " << std::to_string( key ) << ", \"" << (char)key << "\""
            // std::cout << "Input is: " << key << std::endl;
            switch ( key ) {
            case hub::utils::Key::F5:
            case hub::utils::Key::Space:
                server.printStatus();
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

    return 0;
}

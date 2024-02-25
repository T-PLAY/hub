

#include <core/Utils.hpp>
#include <io/Stream.hpp>
#include <server/Server.hpp>

int main( int argc, char* argv[] ) {

    int port = HUB_SERVICE_PORT;

    bool daemon = false;

    std::vector<std::string> args( argv + 1, argv + argc );

    const auto helperMessage = "bin-server usage: [--port <int>]";

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
//            std::cout << argv[0] << " usage: [--port <int>]" << std::endl;
            std::cout << helperMessage << std::endl;
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
//            std::cout << argv[0] << " usage: [--maxClient <int>]" << std::endl;
            std::cout << helperMessage << std::endl;
            return 0;
        }
        ++it;
    }

    hub::Server server( port );

    if ( daemon ) { server.run(); }
    else {

        server.asyncRun();
        const auto helperMsg =
            std::string( argv[0] ) +
            " info: [.|Esc] -> exit, [F5|' '] -> print stats, h -> print this helper message";
        std::cout << helperMsg << std::endl;
        bool exit = false;
        while ( !exit && server.running() ) {         // ESC to quit
            const auto key = hub::utils::key_press(); // blocks until a key is pressed
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
        }
        std::cout << "exiting" << std::endl;
    }

    return 0;
}

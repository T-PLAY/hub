
#include <filesystem>

#include <sensor/SensorUtils.hpp>


int main( int argc, char* argv[] ) {

    /** Description
Run server.
    End description */

    std::vector<std::string> args( argv + 1, argv + argc );

    std::string filePath = "";

    if (argc != 2) {
//            std::cout << "unrecognized argument: " << arg << std::endl;
            std::cout << argv[0] << " usage: filePath (." << HUB_EXTENSION << ")" << std::endl;
            return 0;
        }

    auto it = args.begin();
    while ( it != args.end() ) {
        const auto& arg = *it;

        if ( arg == "-h" || arg == "--help" ) {
            std::cout << argv[0] << " usage: filePath (." << HUB_EXTENSION << ")" << std::endl;
            return 0;
        }
        ++it;
    }

    filePath = argv[1];

    std::cout << "filePath = '" << filePath << "'" << std::endl;
    assert(std::filesystem::exists(filePath));


    hub::sensor::utils::synchronizePath(filePath);

    return 0;
}


#include <filesystem>

#include <sensor/InputSensor.hpp>


int main( int argc, char* argv[] ) {


    std::vector<std::string> args( argv + 1, argv + argc );

    std::string filePath = "";
//    std::cout << "argc = " << argc << std::endl;

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

    std::cout << "filePath: '" << filePath << "'" << std::endl;
    assert(std::filesystem::exists(filePath));

    hub::input::InputFile inputFile{filePath};
    std::cout << "header: " << inputFile.getHeader() << std::endl;
    hub::sensor::InputSensor inputSensor(inputFile);
    std::cout << "sensorSpec: " << inputSensor.getSpec() << std::endl;
    auto acqs = inputSensor.getAllAcquisitions();
    int iAcq = 0;
    for (const auto & acq : acqs) {
        std::cout << iAcq << ": " << acq << std::endl;
        ++iAcq;
    }

    return 0;
}

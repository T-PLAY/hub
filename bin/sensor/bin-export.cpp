
#include <filesystem>

#include <sensor/InputSensor.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main( int argc, char* argv[] ) {

    std::vector<std::string> args( argv + 1, argv + argc );

    std::string filePath = "";
    //    std::cout << "argc = " << argc << std::endl;

    if ( argc != 2 ) {
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
    assert( std::filesystem::exists( filePath ) );

    std::filesystem::path path( filePath );
    auto parentPath = path.parent_path().string();
    std::cout << "parentPath: '" << parentPath << "'" << std::endl;

    hub::input::InputFile inputFile { filePath };
    std::cout << "header: " << inputFile.getHeader() << std::endl;
    hub::sensor::InputSensor inputSensor( inputFile );
    std::cout << "sensorSpec: " << inputSensor.getSpec() << std::endl;
    auto acqs = inputSensor.getAllAcquisitions();

    // std::string recordDir = APPLICATIONS_DIR "data/records/latest/";
    // assert( std::filesystem::exists( recordDir ) );

    const std::string exportDir = parentPath + "export/";
    std::filesystem::create_directories( exportDir );

    int iAcq = 0;
    for ( const auto& acq : acqs ) {
        std::cout << iAcq << ": " << acq << std::endl;

        const auto& resolutions = acq.getResolution();
        const auto& measures    = resolutions.getNodes();

        int iNode = 0;
        for ( const auto& measure : measures ) {

            // const auto& resolution = measure.getResolution();
            const auto& dims   = measure.getDims();
            const auto& format = measure.getTypeId();

            if ( dims.size() == 2 ) {
                assert( dims.size() == 2 );
                const int width  = dims.at( 0 );
                const int height = dims.at( 1 );

                auto* data = acq.getData( 2 + iNode );

                if ( format == hub::Types::Y8 ) {

                    const int channels = 1;

                    char filename[80];
                    sprintf( filename, "image%d.png", iAcq ); // Todo fix C4996 sprintf unsafe

                    const std::string filePath = exportDir + filename;
                    std::cout << filePath << std::endl;
                    stbi_write_jpg( filePath.c_str(), width, height, channels, data, 100 );

                    // acqOut << measure.clone();
                }
            }

            ++iNode;
        } // for ( const auto& measure : acq.getMeasures() )

        ++iAcq;
    }

    return 0;
}

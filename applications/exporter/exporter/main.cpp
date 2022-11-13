
#include <cassert>
#include <filesystem>
#include <iostream>
//#include <thread>

//#include <Player.h>

#include <InputSensor.hpp>
//#include <OutputSensor.hpp>

#include <IO/File.hpp>
//#include <IO/Stream.hpp>

#include <filesystem>
//#include <set>
//#include <Loader.h>

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

int main( int argc, char* argv[] ) {

//    std::string recordPath = PROJECT_DIR "data/records/latest/";
    std::string recordPath = PROJECT_DIR "data/records/40HzAxial3Sync/";

    for ( const auto& fileDir : std::filesystem::directory_iterator( recordPath ) ) {

        const auto& filepath = fileDir.path().string();
        const auto& filename = fileDir.path().filename();
        if ( filename == "export" ) continue;
        if (fileDir.path().extension() != ".txt")
            continue;

        std::cout << "read '" << filename << "' record" << std::endl;
        assert( std::filesystem::exists( filepath ) );

        std::fstream file( filepath, std::ios::binary | std::ios::in );
        assert( file.is_open() );

        const std::string streamName = filepath;

        hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );

        auto acqs = inputSensor.getAllAcquisitions();

        const auto& resolutions = inputSensor.m_spec.m_resolutions;
        assert( resolutions.at( 0 ).second == hub::SensorSpec::Format::DOF6 );
        assert( resolutions.at( 1 ).second == hub::SensorSpec::Format::Y8 );

        const int width    = resolutions.at( 1 ).first.at( 0 );
        const int height   = resolutions.at( 1 ).first.at( 1 );
        const int channels = 1;

        const auto& metaData = inputSensor.m_spec.m_metaData;
        glm::mat4 localTransform;
        if ( metaData.find( "transform" ) != metaData.end() ) {
            const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
            localTransform     = glm::make_mat4( array );
        }

        const std::string exportDir = recordPath + "export/";
        std::filesystem::create_directories( exportDir );

        std::ofstream fstream( exportDir + "acq.dat" );
        assert( fstream.is_open() );

        int iAcq = 0;
        for ( const auto& acq : acqs ) {
            const auto& measures = acq.getMeasures();

            assert( measures.size() == 2 );
            const hub::Dof6& dof6 = measures.at( 0 );

            std::cout << dof6 << std::endl;
            glm::vec3 position    = glm::vec3( dof6.m_x, dof6.m_y, dof6.m_z );
            glm::quat orientation = glm::quat( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose( localTransform, scale, rotation, translation, skew, perspective );
            rotation = glm::conjugate( rotation );

            position += translation;
            orientation *= rotation;

            fstream << position.x << " " << position.y << " " << position.z << " " << orientation.w
                    << " " << orientation.x << " " << orientation.y << " " << orientation.z
                    << std::endl;

            char filename[80];
            sprintf( filename, "us%d.jpg", iAcq );

            const std::string filePath = exportDir + filename;
            std::cout << filePath << std::endl;
            stbi_write_jpg(
                filePath.c_str(), width, height, channels, measures.at( 1 ).m_data, 100 );

            ++iAcq;
        }
        fstream.close();

        std::cout << acqs.size() << " acquisitions exported." << std::endl;
    }
}

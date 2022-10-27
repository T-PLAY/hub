
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

    //    std::map<std::string, std::unique_ptr<hub::OutputSensor>> outputStreams;
    //    std::vector<std::unique_ptr<hub::OutputSensor>> outputStreams;
    //    std::vector<hub::OutputSensor> outputStreams;
    //    outputStreams.reserve(10);
    //    std::set<Snap> snaps;

    std::string recordPath = PROJECT_DIR "data/records/latest/";
    //    std::vector<std::string> streamNames { "Keyboard", "ProceduralStreamer" };
    //    std::vector<std::string> streamNames { "Polhemus Patriot (sensor 2)" };
    //    const std::string streamName = "Keyboard";

    //    for (const auto& streamName : streamNames) {
    //        std::fstream recordFile(recordPath + streamName + ".txt", std::ios::in |
    //        std::ios::binary | std::ios::beg); assert(recordFile.is_open());

    for ( const auto& fileDir : std::filesystem::directory_iterator( recordPath ) ) {
        //        if ( iSensor != -1 && iFile != iSensor ) continue;

        const auto& filepath = fileDir.path().string();
        const auto& filename = fileDir.path().filename();
        if ( filename == "export" ) continue;

        //        const auto filename = fileDir.path().string();
        std::cout << "read '" << filename << "' record" << std::endl;
        assert( std::filesystem::exists( filepath ) );

        //        std::fstream file(filepath, std::ios::binary | std::ios::in | std::ios::beg);
        std::fstream file( filepath, std::ios::binary | std::ios::in );
        assert( file.is_open() );
        //        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        //        file.seekg(0, std::ios::end);
        //        //        std::cout << "tellg" << file.tellg() << std::endl;
        //        //        file.seekg(0, std::ios::beg);

        //        assert( !file.eof() );
        //        //        assert(sizeof(int) == 4);

        const std::string streamName = filepath;

        hub::InputSensor inputSensor( hub::io::File( std::move( file ) ) );
        //        hub::InputSensor inputSensor(hub::io::File(std::move(recordFile)));

        auto acqs = inputSensor.getAllAcquisitions();

        const auto& resolutions = inputSensor.m_spec.m_resolutions;
        assert( resolutions.at( 0 ).second == hub::SensorSpec::Format::DOF6 );
        assert( resolutions.at( 1 ).second == hub::SensorSpec::Format::Y8 );

        const int width    = resolutions.at( 1 ).first.at( 0 );
        const int height   = resolutions.at( 1 ).first.at( 1 );
        const int channels = 1;

        const auto& metaData = inputSensor.m_spec.m_metaData;
        //        float localTransform[16];
        glm::mat4 localTransform;
        if ( metaData.find( "transform" ) != metaData.end() ) {
            const float* array = std::any_cast<const float*>( metaData.at( "transform" ) );
            localTransform          = glm::make_mat4( array );
        }

        //        hub::OutputSensor outputSensor2(inputSensor.m_spec, hub::io::OutputStream("Player
        //        (" + streamName + ")"));
        //        outputStreams[streamName] =
        //        std::make_unique<hub::OutputSensor>(inputSensor.m_spec,
        //        hub::io::OutputStream("Player (" + streamName + ")"));
        //        outputStreams.push_back(std::make_unique<hub::OutputSensor>(inputSensor.m_spec,
        //        hub::io::OutputStream("Player : " + inputSensor.m_spec.m_sensorName)));
        //        outputStreams.emplace_back(inputSensor.m_spec, hub::io::OutputStream("Player (" +
        //        streamName + ")"));

        const std::string exportDir = recordPath + "export/";
        std::filesystem::create_directories( exportDir );

        std::ofstream fstream( exportDir + "posQuat.txt" );
        assert( fstream.is_open() );

        int iAcq = 0;
        for ( const auto& acq : acqs ) {
            const auto& measures = acq.getMeasures();

            assert( measures.size() == 2 );
            const hub::Dof6& dof6 = measures.at( 0 );

            std::cout << dof6 << std::endl;
            glm::vec3 position    = glm::vec3( dof6.m_x, dof6.m_y, dof6.m_z );
            glm::quat orientation = glm::quat( dof6.m_w0, dof6.m_w1, dof6.m_w2, dof6.m_w3 );

//            glm::vec3 translation = glm::vec3(localTransform[3][0], localTransform[3][1], localTransform[3][2]); // column major
//            glm::vec3 translation = glm::vec3(localTransform[0][3], localTransform[1][3], localTransform[2][3]); // row major
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(localTransform, scale, rotation, translation, skew, perspective);
            rotation = glm::conjugate(rotation);

            position += translation;
            orientation *= rotation;

            fstream << position.x << " " << position.y << " " << position.z << " " << orientation.w
                    << " " << orientation.x << " " << orientation.y << " " << orientation.z
                    << std::endl;

            char filename[80];
            sprintf( filename, "us$%d.jpg", iAcq );

            const std::string filePath = exportDir + filename;
            std::cout << filePath << std::endl;
            stbi_write_jpg(
                filePath.c_str(), width, height, channels, measures.at( 1 ).m_data, 100 );
            //            stbi_write_jpg(filePath.c_str(), width, height, channels,
            //            measures.at(1).m_data, width);

            //            std::cout << "read acq : " << acq << std::endl;
            //            Snap snap { acq.clone(), (int)(outputStreams.size() - 1) };
            //            snaps.insert(std::move(snap));
            ++iAcq;
        }
        fstream.close();

        std::cout << acqs.size() << " acquisitions exported." << std::endl;
    }

    //    hub::SensorSpec sensorSpec = inputSensor.m_spec;
    //    auto& metaData             = sensorSpec.m_metaData;
    //    metaData["type"]           = "record";
    //    metaData["nAcqs"]          = (unsigned int)acqs.size();

    //    hub::OutputSensor outputSensor( inputSensor.m_spec, hub::io::OutputStream( "Player
    //    (record)" ) );

    //    for ( const auto& acq : acqs ) {
    //        outputSensor << acq;
    //    }

    //        while (true) {
    //            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    //        }

    //    const long long duration = acqs.back().m_start - acqs.front().m_start;
    //    int iLoop = 0;
    //    while (true) {
    //        //        const auto startRecord = acqs.front().m_start;
    //        const auto startRecord = snaps.begin()->m_acq.m_start;
    //        const auto& startChrono = std::chrono::high_resolution_clock::now();

    //        //        long long dec = iLoop * duration;

    //        auto it = snaps.begin();
    //        //            while (it != m_snapshots.end()) {
    //        while (it != snaps.end()) {
    //            const auto& snap = *it;

    //            std::this_thread::sleep_until(startChrono +
    //            std::chrono::microseconds(snap.m_acq.m_start - startRecord));
    //            //            std::this_thread::sleep_for(
    //            //                                           std::chrono::milliseconds( 1000));

    //            //                const auto& acq = snapshot.getAcq();
    //            //                hub::Acquisition acq2 { acq.m_start + dec,
    //            //                                        acq.m_end + dec
    //            //                                        };
    //            //                acq2 << acq.getMeasures();

    //            //                *m_outputs.at( snapshot.getSensorName() ) << acq2;
    //            //                    << snapshot.getAcq();
    //            //            outputSensor2 << snap;
    //            *outputStreams.at(snap.iStream) << snap.m_acq;
    ////            outputStreams.at(snap.iStream) << snap.m_acq;

    //            ++it;
    //            //                m_isPlaying = m_futureObj.wait_for(std::chrono::milliseconds(1))
    //            //                == std::future_status::timeout;
    //        }

    //        //            if ( !m_isPlaying ) { std::cout << "end record" << std::endl; }
    //        //            else { std::cout << "auto loop " << iLoop << std::endl; }
    //        std::cout << "auto loop " << iLoop << std::endl;
    //        ++iLoop;
    //    }

    //    std::string recordFolder;
    //    int iSensor = -1;
    //    if (argc == 2) {
    ////        recordFolder = argv[1];
    //        iSensor = atoi(argv[1]);
    //    } else {
    ////        recordFolder = PROJECT_DIR "data/records/latest/";
    //    }
    //        recordFolder = PROJECT_DIR "data/records/latest/";

    //    std::cout << "record folder : " << recordFolder << std::endl;
    //    assert(std::filesystem::exists(recordFolder));

    //    Player player;
    //        player.load(recordFolder, iSensor);
    //        player.play();
    //        std::this_thread::sleep_for(std::chrono::seconds(3600));

    ////    for (int i = 0; i < 10; ++i) {
    ////        player.load(recordFolder);
    ////        player.play();
    //////        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    ////        player.stop();
    ////        player.unload();
    ////    }
    //    return 0;
}
